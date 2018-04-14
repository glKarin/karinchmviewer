#ifndef _KARIN_SEARCHLIST_H
#define _KARIN_SEARCHLIST_H

#include <QAbstractListModel>
#include <MAbstractCellCreator>
#include <MContainer>
#include <QVector>

class MContentItem;
class MList;

namespace karin
{
	class chm_engine;
	class searchresult_item;
	class search_box;

	class search_model : public QAbstractListModel
	{
		Q_OBJECT

		public:
			search_model(QObject *parent = 0);
			virtual ~search_model();
			void setData(const QVector<searchresult_item> &vector);
			virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
			virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
			void clearList();

		private:
			QVector<searchresult_item> result;
	};

	class search_delegate : public MAbstractCellCreator<MContentItem>
	{
		public:
			search_delegate();
			virtual ~search_delegate();
			virtual void updateCell(const QModelIndex &index, MWidget *cell) const;
	};

	class search_listview : public MContainer
	{
		Q_OBJECT

		public:
			search_listview(QGraphicsItem *parent = 0);
			virtual ~search_listview();
			void setChmEngine(chm_engine *e);
			void init(bool b);

Q_SIGNALS:
			void openUrl(const QString &url);

			private Q_SLOTS:
				void open(const QModelIndex &index);
				void search();

		private:
			search_model *model;
			search_box *sb;
			MList *list;
			chm_engine *engine;

		private:
			search_listview(const search_listview &other);
			search_listview & operator= (const search_listview &other);
	};
}

#endif
