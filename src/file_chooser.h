#ifndef _KARIN_FILECHOOSER_H
#define _KARIN_FILECHOOSER_H

#include <QAbstractListModel>
#include <MApplicationPage>
#include <QVector>
#include <MAbstractCellCreator>

class MContentItem;
class MList;

namespace karin
{
	class file_engine;
	class header_view;
	class button_with_label;

	class file_model : public QAbstractListModel
	{
		Q_OBJECT
		public:
			file_model(QObject *parent = 0);
			virtual ~file_model();
			void setPath(const QString &dir);
			virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
			virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

			public Q_SLOTS:
				void showHiddenFile(bool yes);
			void onlyShowChm(bool yes);

			private Q_SLOTS:
				void setBuddy();

Q_SIGNALS:
			void requestBuddy(const QString &path);

		private:
			file_engine *engine;

		private:
			file_model(const file_model &other);
			file_model & operator= (const file_model &other);
	};

	class file_delegate : public MAbstractCellCreator<MContentItem>
	{
		public:
			file_delegate();
			virtual ~file_delegate();
			virtual void updateCell(const QModelIndex &index, MWidget *cell) const;
	};

	class file_chooser : public MApplicationPage
	{
		Q_OBJECT
		public:
			file_chooser(QGraphicsItem *parent = 0);
			virtual ~file_chooser();

private Q_SLOTS:
			void open(const QModelIndex &index);

Q_SIGNALS:
			void openChm(const QString &path);

		private:
			MList *list;
			file_model *model;
			button_with_label *myShowHidden;
			button_with_label *myChmOnly;

		private:
			file_chooser(const file_chooser &other);
			file_chooser & operator= (const file_chooser &other);
	};
}


#endif
