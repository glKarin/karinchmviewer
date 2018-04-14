#ifndef _KARIN_DIRECTORYTREE_H
#define _KARIN_DIRECTORYTREE_H

#include <MAbstractCellCreator>
#include <QList>
#include <MListItem>
#include <MContainer>

class MList;
class MLabel;
class MSeparator;
class MButton;
class QGraphicsAnchorLayout;

namespace karin
{
	class directory_model;

	class tree_node : public MListItem
	{
		Q_OBJECT

		public:
			tree_node(QGraphicsItem *parent = 0);
			virtual ~tree_node();
			void setTitle(const QString &t);
			void setCheckable(bool b);
			void setChecked(bool b);
			void setIndent(int len);

			private Q_SLOTS:
				void thisLongTap(bool b);

		private:
			QGraphicsAnchorLayout *layout;
			int indent;
			MLabel *title;
			MButton *checkbox;
			MSeparator *separator;
			MSeparator *separator2;
			MSeparator *separator3;

		private:
			tree_node(const tree_node &other);
			tree_node & operator= (const tree_node &other);
	};

	class directory_delegate : public MAbstractCellCreator<tree_node>
	{
		public:
			directory_delegate();
			virtual ~directory_delegate();
			virtual void updateCell(const QModelIndex &index, MWidget *cell) const;
	};

	class directory_treeview : public MContainer
	{
		Q_OBJECT

		public:
			directory_treeview(QGraphicsItem *parent = 0);
			virtual ~directory_treeview();
			void setModel(directory_model *m);
			void init(bool b);

Q_SIGNALS:
			void openUrl(const QString &url);

			private Q_SLOTS:
				void open(const QModelIndex &index);
				void openDir(const QModelIndex &index);

		private:
			MList *list;
			MButton *doIn;
			MButton *doOut;
			directory_model *model;

		private:
			directory_treeview(const directory_treeview &other);
			directory_treeview & operator= (const directory_treeview &other);
	};
}

#endif
