#include "search_list.h"
#include "chm_engine.h"
#include "karin_std.h"
#include "karin_ut.h"
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

	karin::search_model::search_model(QObject *parent)
:QAbstractListModel(parent)
{
}

karin::search_model::~search_model()
{
}

void karin::search_model::setData(const QVector<karin::searchresult_item> &vector)
{
	result = vector;
	emit layoutChanged();
}

void karin::search_model::clearList()
{
	result.clear();
	emit layoutChanged();
}

int karin::search_model::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return result.size();
}

QVariant karin::search_model::data(const QModelIndex &index, int role) const
{
	QVariantMap data;
	if(role == Qt::DisplayRole)
	{
		int i = index.row();
		if(i >= result.size())
			return QVariant(data);
		data.insert("name", QVariant(result.at(i).name));
		data.insert("url", QVariant(result.at(i).url));
	}
	return QVariant(data);
}

	karin::search_delegate::search_delegate()
:MAbstractCellCreator<MContentItem>()
{
}

karin::search_delegate::~search_delegate()
{
}

void karin::search_delegate::updateCell(const QModelIndex &index, MWidget *cell) const
{
	MContentItem *contentItem = dynamic_cast<MContentItem *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QVariantMap rowData = data.value<QVariantMap>();
	contentItem -> setTitle(rowData["name"].toString());
	contentItem -> setImage(QImage(QString(KARIN_PREFIX) + "/resources/karin-l-page.png"));
	contentItem -> setSubtitle(rowData["url"].toString());
}

	karin::search_listview::search_listview(QGraphicsItem *parent)
:MContainer(parent),
	model(new karin::search_model(this)),
	sb(new karin::search_box),
	list(new MList),
	engine(0)
{
	MLayout *layout = new MLayout;
	layout -> setContentsMargins(0.0, 0.0, 0.0, 0.0);
	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
	MPannableViewport *pan = new MPannableViewport;
	setHeaderVisible(false);
	//list -> setSelectionMode(MList::SingleSelection);
	list -> setCellCreator(new karin::search_delegate);
	list -> setItemModel(model);
	pan -> setWidget(list);
	policy -> addItem(sb);
	policy -> addItem(new MSeparator);
	policy -> addItem(pan);
	connect(list, SIGNAL(itemClicked (const QModelIndex &)), this, SLOT(open(const QModelIndex &)));
	connect(sb, SIGNAL(returnPressed()), this, SLOT(search()));
	connect(sb, SIGNAL(triggered(bool)), this, SLOT(search()));
	centralWidget() -> setLayout(layout);
}

karin::search_listview::~search_listview()
{
	engine = 0;
}

void karin::search_listview::open(const QModelIndex &index)
{
	emit openUrl(index.data(Qt::DisplayRole).value<QVariantMap>()["url"].toString());
}

void karin::search_listview::setChmEngine(karin::chm_engine *e)
{
	engine = e;
}

void karin::search_listview::search()
{
	if(engine && !engine -> chmfile().isEmpty() && engine -> hasSearchTable())
	{
		QVector<searchresult_item> vector;
		model -> setData(vector);
		if(sb -> text().isEmpty())
			return;
		if(!engine -> searchQuery(&vector, sb -> text(), karin::ut::Instance() -> getSetting<unsigned int>(SEARCH_RESULT_COUNT)))
			vector.clear();
		model -> setData(vector);
		list -> scrollTo(model -> index(0, 0), MList::PositionAtTopHint);
	}
}

void karin::search_listview::init(bool b)
{
	sb -> setEnabled(b);
	if(b)
	{
		sb -> setPrompt(tr("Input keyword to search"));
		if(model)
			list -> scrollTo(model -> index(0, 0), MList::PositionAtTopHint);
	}
	else
		sb -> setPrompt(tr("Not support to search"));
	model -> clearList();
}
