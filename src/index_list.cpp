#include "index_list.h"
#include "chm_engine.h"
#include "karin_std.h"
#include "search_box.h"

#include <MList>
#include <MPopupList>
#include <QVector>
#include <MContentItem>
#include <MPannableViewport>
#include <MSeparator>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <QImage>
#include <QStringListModel>

#ifdef _SIMLATOR
#include <QDebug>
#endif

	karin::index_delegate::index_delegate()
:MAbstractCellCreator<MContentItem>()
{
}

karin::index_delegate::~index_delegate()
{
}

void karin::index_delegate::updateCell(const QModelIndex &index, MWidget *cell) const
{
	MContentItem *contentItem = dynamic_cast<MContentItem *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QVariantMap rowData = data.value<QVariantMap>();
	contentItem -> setTitle(rowData["name"].toString());
	contentItem -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-l-page.png"));
	contentItem -> setSubtitle(QObject::tr("Sub index") + ": " + QString::number(rowData["subname"].toInt()));
}

	karin::index_listview::index_listview(QGraphicsItem *parent)
:MContainer(parent),
	list(new MList),
	model(0),
	sb(new karin::search_box),
	current(0)
{
	MLayout *layout = new MLayout;
	layout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
	MPannableViewport *pan = new MPannableViewport;
	setHeaderVisible(false);
	sb -> setToolVisible(false);
	//list -> setSelectionMode(MList::SingleSelection);
	list -> setCellCreator(new karin::index_delegate);
	pan -> setWidget(list);
	policy -> addItem(sb);
	policy -> addItem(new MSeparator);
	policy -> addItem(pan);
	connect(list, SIGNAL(itemClicked (const QModelIndex &)), this, SLOT(tryOpen(const QModelIndex &)));
	sb -> setCheckable(false);
	connect(sb, SIGNAL(textChanged()), this, SLOT(filter()));
	centralWidget() -> setLayout(layout);
}

karin::index_listview::~index_listview()
{
	model = 0;
}

void karin::index_listview::tryOpen(const QModelIndex &index)
{
	if(!model)
		return;
	current = index.row();
	karin::index_item item = model ->table() -> at(current);
	if(item.subitem.size() > 1)
	{
		QStringList sl;
		for(QVector<karin::searchresult_item>::const_iterator itor = item.subitem.begin();
				itor != item.subitem.end();
				++itor)
			sl.push_back(itor -> name);
		MPopupList *popup = new MPopupList;
		QStringListModel *popupModel = new QStringListModel(popup);
		popupModel -> setStringList(sl);
		popup -> setItemModel(popupModel);
		popup -> setTitle(item.name);
		connect(popup, SIGNAL(clicked(const QModelIndex &)), this, SLOT(open(const QModelIndex &)));
		popup -> appear(MSceneWindow::DestroyWhenDone);
	}
	else
	{
		emit openUrl(item.subitem.at(0).url);
	}
}

void karin::index_listview::open(const QModelIndex &index)
{
	if(model)
		emit openUrl(model -> table() -> at(current).subitem.at(index.row()).url);
}

void karin::index_listview::filter()
{
	if(model)
	{
		if(sb -> text().isEmpty())
			list -> scrollTo(model -> index(0, 0), MList::PositionAtTopHint, MList::Animated);
		else
		{
			const QVector<karin::index_item> *vector = model -> table();
			for(int i = 0; i < vector -> size(); i++)
				if(vector -> at(i).name.startsWith(sb -> text(), Qt::CaseInsensitive))
				{
					list -> scrollTo(model -> index(i, 0), MList::PositionAtTopHint, MList::Animated);
					break;
				}
		}
	}
}

void karin::index_listview::init(bool b)
{
	sb -> setEnabled(b);
	current = 0;
	if(b)
	{
		sb -> setPrompt(tr("Input keyword to filter quickly"));
		if(model)
			list -> scrollTo(model -> index(0, 0), MList::PositionAtTopHint);
	}
	else
		sb -> setPrompt(tr("Not support to view index list"));
}

void karin::index_listview::setModel(karin::index_model *m)
{
	model = m;
	list -> setItemModel(model);
}
