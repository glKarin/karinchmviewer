#include "bookmark_dialog.h"
#include "karin_ut.h"
#include "editable_delegate.h"

#include <MButton>
#include <MLinearLayoutPolicy>
#include <MSeparator>
#include <MLayout>
#include <MPannableViewport>
#include <MList>
#include <QDateTime>
#ifdef _SIMLATOR
#include <QDebug>
#endif

karin::bookmark_model::bookmark_model(QObject *parent)
	:QAbstractListModel(parent)
{
}

karin::bookmark_model::~bookmark_model()
{
}

QVariant karin::bookmark_model::data(const QModelIndex &index, int role) const
{
	QVariantMap data;
	if(role == Qt::DisplayRole)
	{
		int i = index.row();
		if(i >= list.size())
			return QVariant(data);
		return list.at(i);
	}
	return QVariant(data);
}

void karin::bookmark_model::init(const QString &file)
{
	list = karin::ut::Instance() -> getBookmarkTableData(file);
	emit layoutChanged();
}

int karin::bookmark_model::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return list.size();
}

karin::bookmark_delegate::bookmark_delegate()
	:MAbstractCellCreator<karin::editable_delegate>()
{
}

karin::bookmark_delegate::~bookmark_delegate()
{
}

void karin::bookmark_delegate::updateCell (const QModelIndex &index, MWidget *cell) const
{
	karin::editable_delegate * contentItem = dynamic_cast<karin::editable_delegate *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QVariantMap rowData = data.value<QVariantMap>();
	contentItem -> setTitle(rowData[BMK_NAME].toString() + " - " + QDateTime::fromMSecsSinceEpoch(rowData[TIMESTAMP].value<qint64>()).toString());
	contentItem -> setSubtitle(rowData[BMK_VALUE].toString());
}

karin::bookmark_dialog::bookmark_dialog()
	:MDialog(),
	model(new karin::bookmark_model(this))
{
	MLayout *layout = new MLayout;
	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
	MButton *add = new MButton;
	MPannableViewport *pan = new MPannableViewport;
	MList *list = new MList;
	add -> setText(tr("Add current page to bookmarks"));
	add -> setPreferredWidth(centralWidget() -> preferredWidth());
	add -> setMinimumWidth(centralWidget() -> minimumWidth());
	add -> setMaximumWidth(centralWidget() -> maximumWidth());
	pan -> setPreferredSize(centralWidget() -> preferredWidth(), 300);
	list -> setItemModel(model);
	list -> setCellCreator(new karin::bookmark_delegate);
	setTitle(tr("Bookmark List"));
	setCloseButtonVisible(true);
	setTitleBarIconId("icon-l-notes-main-view");
	//addButton(M::OkButton);
	pan -> setWidget(list);
	policy -> addItem(add);
	policy -> addItem(new MSeparator);
	policy -> addItem(pan);
	connect(add, SIGNAL(clicked(bool)), this, SLOT(addBookmark()));
	connect(list, SIGNAL(itemClicked(const QModelIndex &)), this, SLOT(open(const QModelIndex &)));
	connect(list, SIGNAL(itemLongTapped(const QModelIndex &)), this, SLOT(removeBookmark(const QModelIndex &)));
	centralWidget() -> setLayout(layout);
}

karin::bookmark_dialog::~bookmark_dialog()
{
	setCentralWidget(0);
}

void karin::bookmark_dialog::init(const QString &file, const QString &topic, const QString &url)
{
	m_file = file;
	m_topic = topic;
	m_url = url;
	model -> init(m_file);
}

void karin::bookmark_dialog::addBookmark()
{
	if(!m_url.isEmpty())
	{
		karin::ut::Instance() -> addBookmarkToTable(m_file, m_topic.isEmpty() ? tr("Bookmark") : m_topic, m_url);
		model -> init(m_file);
	}
}

void karin::bookmark_dialog::open(const QModelIndex &index)
{
	disappear();
	emit openUrl(index.data().value<QVariantMap>()[BMK_VALUE].toString());
}

void karin::bookmark_dialog::removeBookmark(const QModelIndex &index)
{
	QVariantMap data = index.data().value<QVariantMap>();
	karin::ut::Instance() -> removeBookmarkFormTable(m_file, BMK_VALUE, data[BMK_VALUE].toString());
	model -> init(m_file);
}
