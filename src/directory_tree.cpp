#include "directory_tree.h"
#include "chm_engine.h"
#include "karin_std.h"

#include <MList>
#include <QGraphicsAnchorLayout>
#include <MLabel>
#include <MButton>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MSeparator>
#include <MToolBar>
#include <MWidgetAction>
#include <MPannableViewport>

#ifdef _SIMLATOR
#include <QDebug>
#endif

static const int TreeWidthUnit = 30;

karin::tree_node::tree_node(QGraphicsItem *parent)
	:MListItem(parent),
	layout(new QGraphicsAnchorLayout),
	title(new MLabel),
	checkbox(new MButton),
	separator(new MSeparator),
	separator2(new MSeparator),
	separator3(new MSeparator)
{
	//layout -> setHorizontalSpacing(0.0);
	MPannableViewport *pan = new MPannableViewport;
	title -> setWrapMode(QTextOption::NoWrap);
	separator2 -> setOrientation(Qt::Vertical);
	separator2 -> setPreferredHeight(preferredHeight() / 2);
	separator -> setVisible(false);
	/*
	separator -> setPreferredHeight(1);
	separator -> setMinimumHeight(1);
	separator -> setMaximumHeight(1);
	*/
	separator3 -> setPreferredHeight(6);
	separator3 -> setMinimumHeight(6);
	separator3 -> setMaximumHeight(6);
	separator3 -> setPreferredWidth(TreeWidthUnit);
	separator3 -> setMinimumWidth(TreeWidthUnit);
	separator3 -> setMaximumWidth(TreeWidthUnit);
	separator2 -> setPreferredWidth(6);
	separator2 -> setMinimumWidth(6);
	separator2 -> setMaximumWidth(6);
	pan -> setWidget(title);
	pan -> setPreferredSize(300, preferredHeight());
	pan -> setMinimumHeight(minimumHeight());
	pan -> setMaximumHeight(maximumHeight());
	pan -> setPanDirection(Qt::Horizontal);
	checkbox -> setViewType(MButton::checkboxType);
	checkbox -> setCheckable(true);
	layout -> addAnchor(checkbox, Qt::AnchorLeft, layout, Qt::AnchorLeft);
	layout -> addAnchor(checkbox, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(separator, Qt::AnchorLeft, checkbox, Qt::AnchorRight);
	layout -> addAnchor(separator, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(separator2, Qt::AnchorLeft, separator, Qt::AnchorRight);
	layout -> addAnchor(separator2, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(separator3, Qt::AnchorLeft, separator2, Qt::AnchorRight);
	layout -> addAnchor(separator3, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	layout -> addAnchor(pan, Qt::AnchorLeft, separator3, Qt::AnchorRight);
	layout -> addAnchor(pan, Qt::AnchorRight, layout, Qt::AnchorRight);
	layout -> addAnchor(pan, Qt::AnchorVerticalCenter, layout, Qt::AnchorVerticalCenter);
	connect(checkbox, SIGNAL(clicked(bool)), this, SLOT(thisLongTap(bool)));
	layout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	layout -> setSpacing(0.0);
	setLayout(layout);
}

karin::tree_node::~tree_node()
{
}

void karin::tree_node::thisLongTap(bool b)
{
	Q_UNUSED(b);
	MListItem::longTap(checkbox -> scenePos());
}

void karin::tree_node::setTitle(const QString &t)
{
	title -> setText(t);
}

void karin::tree_node::setCheckable(bool b)
{
	checkbox -> setVisible(b);
}

void karin::tree_node::setChecked(bool b)
{
	checkbox -> setChecked(b);
}

void karin::tree_node::setIndent(int len)
{
	if(len <= 0)
		indent = 1;
	else
		indent = len;
	int s = layout -> horizontalSpacing() * 2;
	int w = separator2 -> preferredWidth();
	separator -> setPreferredWidth((indent - 1) * TreeWidthUnit + w + s);
	separator -> setMinimumWidth((indent - 1) * TreeWidthUnit + w + s);
	separator -> setMaximumWidth((indent - 1) * TreeWidthUnit + w + s);
}

karin::directory_delegate::directory_delegate()
	:MAbstractCellCreator<karin::tree_node>()
{
}

karin::directory_delegate::~directory_delegate()
{
}

void karin::directory_delegate::updateCell(const QModelIndex &index, MWidget *cell) const
{
	karin::tree_node *contentItem = dynamic_cast<karin::tree_node *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QVariantMap rowData = data.value<QVariantMap>();
	contentItem -> setTitle(rowData["name"].toString());
	contentItem -> setCheckable(rowData["isdir"].toBool());
	contentItem -> setChecked(rowData["opened"].toBool());
	contentItem -> setIndent(rowData["indent"].toInt() + 1);
}

karin::directory_treeview::directory_treeview(QGraphicsItem *parent)
	:MContainer(parent),
	list(new MList),
	doIn(new MButton(tr("Collapse All"))),
	doOut(new MButton(tr("Expand All"))),
	model(0)
{
	MLayout *layout = new MLayout;
	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
	MPannableViewport *pan = new MPannableViewport;
	list -> setCellCreator(new karin::directory_delegate);
	pan -> setWidget(list);
	MWidgetAction *collapse = new MWidgetAction(this);
	collapse -> setWidget(doIn);
	MWidgetAction *expand = new MWidgetAction(this);
	expand -> setWidget(doOut);
	MToolBar *bar = new MToolBar;
	collapse -> setLocation(MAction::ToolBarLocation);
	expand -> setLocation(MAction::ToolBarLocation);
	bar -> addAction(collapse);
	bar -> addAction(expand);
	policy -> addItem(bar);
	policy -> addItem(new MSeparator);
	policy -> addItem(pan);
	layout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	setHeaderVisible(false);
	connect(list, SIGNAL(itemLongTapped(const QModelIndex &)), this, SLOT(openDir(const QModelIndex &)));
	connect(list, SIGNAL(itemClicked (const QModelIndex &)), this, SLOT(open(const QModelIndex &)));
	centralWidget() -> setLayout(layout);
}

karin::directory_treeview::~directory_treeview()
{
	model = 0;
}

void karin::directory_treeview::openDir(const QModelIndex &index)
{
	QVariantMap data = index.data(Qt::DisplayRole).value<QVariantMap>();
	if(data["isdir"].toBool())
		model -> open(index);
}

void karin::directory_treeview::open(const QModelIndex &index)
{
		emit openUrl(index.data(Qt::DisplayRole).value<QVariantMap>()["url"].toString());
}

void karin::directory_treeview::setModel(directory_model *m)
{
	model = m;
	list -> setItemModel(model);
	if(model)
	{
		connect(doOut, SIGNAL(clicked(bool)), model, SLOT(all()));
		connect(doIn, SIGNAL(clicked(bool)), model, SLOT(init()));
	}
}

void karin::directory_treeview::init(bool b)
{
	doIn -> setEnabled(b);
	doOut -> setEnabled(b);
	if(model && b)
		list -> scrollTo(model -> index(0, 0), MList::PositionAtTopHint);
}
