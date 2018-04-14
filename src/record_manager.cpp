#include "record_manager.h"
#include "karin_std.h"
#include "karin_ut.h"
#include "editable_delegate.h"
#include "header_view.h"

#include <QDateTime>
#include <QModelIndex>
#include <QVariant>
#include <MPannableViewport>
#include <MList>
#include <MContainer>
#include <MWidgetAction>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>

#ifdef _SIMLATOR
#include <QDebug>
#endif

karin::record_model::record_model(QObject *parent)
	:QAbstractListModel(parent)
{
}

karin::record_model::~record_model()
{
}

QVariant karin::record_model::data(const QModelIndex &index, int role) const
{
	QVariant data;
	if(role == Qt::DisplayRole)
	{
		int i = index.row();
		if(i >= list.size())
			return data;
		data = list.at(i);
	}
	return data;
}

int karin::record_model::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return list.size();
}

void karin::record_model::getData(const QString &table)
{
	list.clear();
	list = karin::ut::Instance() -> getRecordTableData(table);
	emit layoutChanged();
}

karin::record_delegate::record_delegate()
	:MAbstractCellCreator<karin::editable_delegate>()
{
}

karin::record_delegate::~record_delegate()
{
}

void karin::record_delegate::updateCell (const QModelIndex &index, MWidget *cell) const
{
	karin::editable_delegate * contentItem = dynamic_cast<karin::editable_delegate *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QVariantMap rowData = data.value<QVariantMap>();
	contentItem -> setTitle(rowData[RCD_NAME].toString() + " - " + QDateTime::fromMSecsSinceEpoch(rowData[TIMESTAMP].value<qint64>()).toString());
	contentItem -> setSubtitle(rowData[RCD_VALUE].toString());
}

//virtual QSizeF	cellSize () const;

karin::record_manager::record_manager(const QString &name, QGraphicsItem *parent)
	:MApplicationPage(parent),
	tableName(name),
	header(new karin::header_view),
	model(new karin::record_model(this)),
	container(new MContainer)
{
	MLayout *layout = new MLayout;
	layout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
	MList *list = new MList;
	MWidgetAction *action = new MWidgetAction(this);
	MLabel *label = new MLabel;
	MPannableViewport *pan = new MPannableViewport;
	setPannable(false);
	label -> setText(tr("Long tap also can remove a item"));
	action -> setLocation(MAction::ToolBarLocation);
	action -> setWidget(label);
	clearActions();
	addAction(action);
	list -> setCellCreator(new karin::record_delegate);
	list -> setItemModel(model);
	pan -> setWidget(list);
	model -> getData(tableName);
	container -> setCentralWidget(pan);
	header -> setTitleFont(karin::getConstantFont(karin::VeryLarge));
	policy -> addItem(header);
	policy -> addItem(container);
	centralWidget() -> setLayout(layout);
	connect(list, SIGNAL(itemClicked (const QModelIndex &)), this, SLOT(open(const QModelIndex &)));
	connect(list, SIGNAL(itemLongTapped (const QModelIndex &)), this, SLOT(close(const QModelIndex &)));
}

karin::record_manager::~record_manager()
{
}

void karin::record_manager::open(const QModelIndex &index)
{
	QVariant var = index.data(Qt::DisplayRole);
	QVariantMap info = var.value<QVariantMap>();
	emit openChm(info[RCD_VALUE].toString());
}

void karin::record_manager::init()
{
	if(!tableName.isEmpty())
		model -> getData(tableName);
	int row = model -> rowCount();
	if(row > 0)
		container -> setTitle(QString::number(row) + " " + tr("items"));
	else
		container -> setTitle(tr("No item"));
}

void karin::record_manager::setTitle(const QString &t)
{
	if(!t.isEmpty())
	{
		header -> setTitle(t);
		MApplicationPage::setTitle(t + " - Karin CHM Viewer");
	}
}

void karin::record_manager::close(const QModelIndex &index)
{
	if(tableName.isEmpty())
		return;
	QVariant var = index.data(Qt::DisplayRole);
	QVariantMap info = var.value<QVariantMap>();
	karin::ut::Instance() -> removeOneRecord(tableName, RCD_VALUE, info[RCD_VALUE].toString());
	init();
}
