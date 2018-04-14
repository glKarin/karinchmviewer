#include "file_chooser.h"
#include "file_engine.h"
#include "karin_std.h"
#include "button_with_label.h"
#include "header_view.h"
#include "karin_ut.h"

#include <QModelIndex>
#include <QVariant>
#include <MContentItem>
#include <MAction>
#include <MPannableViewport>
#include <MWidgetAction>
#include <MList>
#include <MContainer>
#include <MLabel>
#include <QImage>
#include <QFileInfo>
#include <MLayout>
#include <MLinearLayoutPolicy>

#ifdef _SIMLATOR
#include <QDebug>
#endif

karin::file_model::file_model(QObject *parent)
	:QAbstractListModel(parent),
	engine(new karin::file_engine(this))
{
	engine -> setPath(karin::ut::Instance() -> getSetting<QString>(FILECHOOSER_LASTDIR));
	if(karin::ut::Instance() -> getSetting<bool>(FILECHOOSER_CHMONLY))
		engine -> setFilters(QStringList()<<"*.chm");
	engine -> setShowHidden(karin::ut::Instance() -> getSetting<bool>(FILECHOOSER_SHOWHIDDEN));
	connect(engine, SIGNAL(filelistChanged()), this, SIGNAL(layoutChanged()));
	connect(engine, SIGNAL(pathChanged()), this, SLOT(setBuddy()));
}

karin::file_model::~file_model()
{
}

void karin::file_model::setBuddy()
{
	emit requestBuddy(engine -> path());
}

QVariant karin::file_model::data(const QModelIndex &index, int role) const
{
	QVariantMap data;
	if(role == Qt::DisplayRole)
	{
		int i = index.row();
		if(i >= engine -> filelist() -> size())
			return QVariant(data);
		karin::file_info info = engine -> filelist() -> at(i);
		data.insert("name", QVariant(info.name));
		data.insert("path", QVariant(info.path));
		data.insert("size", QVariant(info.size));
		data.insert("owner", QVariant(info.owner));
		data.insert("group", QVariant(info.group));
		data.insert("permission", QVariant(info.permission));
		data.insert("type", QVariant(info.type));
	}
	return QVariant(data);
}

int karin::file_model::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return engine -> filelist() -> size();
}

void karin::file_model::setPath(const QString &dir)
{
	engine -> setPath(dir);
}

void karin::file_model::showHiddenFile(bool yes)
{
	engine -> setShowHidden(yes);
	karin::ut::Instance() -> setSetting<bool>(FILECHOOSER_SHOWHIDDEN, yes);
}

void karin::file_model::onlyShowChm(bool yes)
{
	if(yes)
		engine -> setFilters(QStringList()<<"*.chm");
	else
		engine -> setFilters(QStringList());
	karin::ut::Instance() -> setSetting<bool>(FILECHOOSER_CHMONLY, yes);
}

karin::file_delegate::file_delegate()
	:MAbstractCellCreator<MContentItem>()
{
}

karin::file_delegate::~file_delegate()
{
}

void karin::file_delegate::updateCell (const QModelIndex &index, MWidget *cell) const
{
	MContentItem * contentItem = dynamic_cast<MContentItem *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QVariantMap rowData = data.value<QVariantMap>();
	contentItem -> setTitle(rowData["name"].toString());
	if(rowData["type"].toBool())
	{
		contentItem -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-m-directory.png"));
	}
	else
	{
		contentItem -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-m-file.png"));
	}
	static const QString format("%1  %2:%3  %4");
	contentItem -> setSubtitle(format.arg(rowData["size"].toString()).arg(rowData["owner"].toString()).arg(rowData["group"].toString()).arg(rowData["permission"].toString()));
}

//virtual QSizeF	cellSize () const;

karin::file_chooser::file_chooser(QGraphicsItem *parent)
	:MApplicationPage(parent),
	list(new MList),
	model(new karin::file_model(this)),
	myShowHidden(new karin::button_with_label(karin::button_with_label::switchType)),
	myChmOnly(new karin::button_with_label)
{
	karin::header_view *header = new karin::header_view;
	MLayout *layout = new MLayout;
	layout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
	MContainer *container = new MContainer;
	MWidgetAction *showHiddenAction = new MWidgetAction(this);
	MWidgetAction *chmOnlyAction = new MWidgetAction(this);
	MPannableViewport *pan = new MPannableViewport;
	setTitle(tr("File Chooser") + " - Karin CHM Viewer");
	container -> setTitle(karin::ut::Instance() -> getSetting<QString>(FILECHOOSER_LASTDIR));
	setPannable(false);
	//list -> setSelectionMode(MList::SingleSelection);
	header -> setTitle(tr("Choose a chm file"));
	header -> setTitleFont(karin::getConstantFont(karin::VeryLarge));
	list -> setCellCreator(new karin::file_delegate);
	list -> setItemModel(model);
	pan -> setWidget(list);
	container -> setCentralWidget(pan);
	policy -> addItem(header);
	policy -> addItem(container);
	centralWidget() -> setLayout(layout);
	myShowHidden -> setChecked(karin::ut::Instance() -> getSetting<bool>(FILECHOOSER_SHOWHIDDEN));
	myChmOnly -> setChecked(karin::ut::Instance() -> getSetting<bool>(FILECHOOSER_CHMONLY));
	myShowHidden -> setTitle(tr("Show Hidden"));
	myChmOnly -> setTitle(tr("Chm Only"));
	myShowHidden -> setTitleFont(karin::getConstantFont(karin::VerySmall, QFont::Light));
	myChmOnly -> setTitleFont(karin::getConstantFont(karin::VerySmall, QFont::Light));
	showHiddenAction -> setLocation(MAction::ToolBarLocation);
	chmOnlyAction -> setLocation(MAction::ToolBarLocation);
	showHiddenAction -> setWidget(myShowHidden);
	chmOnlyAction -> setWidget(myChmOnly);
	clearActions();
	addAction(showHiddenAction);
	addAction(chmOnlyAction);
	connect(list, SIGNAL(itemClicked (const QModelIndex &)), this, SLOT(open(const QModelIndex &)));
	connect(myShowHidden, SIGNAL(clicked(bool)), model, SLOT(showHiddenFile(bool)));
	connect(myChmOnly, SIGNAL(clicked(bool)), model, SLOT(onlyShowChm(bool)));
	connect(model, SIGNAL(requestBuddy(const QString &)), container, SLOT(setTitle(const QString &)));
}

karin::file_chooser::~file_chooser()
{
}

void karin::file_chooser::open(const QModelIndex &index)
{
	QVariant var = index.data(Qt::DisplayRole);
	QVariantMap info = var.value<QVariantMap>();
	QString path = info["path"].toString();
	if(!info["type"].toBool())
	{
		karin::ut::Instance() -> setSetting<QString>(FILECHOOSER_LASTDIR, QFileInfo(path).absolutePath());
		emit openChm(path);
	}
	else
	{
		list -> scrollTo(model -> index(0, 0), MList::PositionAtTopHint);
		model -> setPath(path);
	}
}
