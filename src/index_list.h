#ifndef _KARIN_INDEXLIST_H
#define _KARIN_INDEXLIST_H

#include <MAbstractCellCreator>
#include <MContainer>

class MContentItem;
class MList;

namespace karin
{
	class chm_engine;
	class index_item;
	class index_model;
	class search_box;

	class index_delegate : public MAbstractCellCreator<MContentItem>
	{
		public:
			index_delegate();
			virtual ~index_delegate();
			virtual void updateCell(const QModelIndex &index, MWidget *cell) const;
	};

	class index_listview : public MContainer
	{
		Q_OBJECT

		public:
			index_listview(QGraphicsItem *parent = 0);
			virtual ~index_listview();
			void init(bool b);
			void setModel(index_model *model);

Q_SIGNALS:
			void openUrl(const QString &url);

			private Q_SLOTS:
				void open(const QModelIndex &index);
				void tryOpen(const QModelIndex &index);
				void filter();

		private:
			MList *list;
			index_model *model;
			search_box *sb;
			int current;

		private:
			index_listview(const index_listview &other);
			index_listview & operator= (const index_listview &other);
	};
}

#endif
