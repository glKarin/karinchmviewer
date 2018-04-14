#ifndef _KARIN_RECORDMANAGER_H
#define _KARIN_RECORDMANAGER_H

#include <QAbstractListModel>
#include <MApplicationPage>
#include <QVector>
#include <MAbstractCellCreator>

class MContainer;

namespace karin
{
	class editable_delegate;
	class header_view;

	class record_model : public QAbstractListModel
	{
		Q_OBJECT
		public:
			record_model(QObject *parent = 0);
			virtual ~record_model();
			void getData(const QString &table);
			virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
			virtual QVariant data(const QModelIndex &index, int role) const;

		private:
			QVariantList list;
	};

	class record_delegate : public MAbstractCellCreator<editable_delegate>
	{
		public:
			record_delegate();
			virtual ~record_delegate();
			virtual void updateCell(const QModelIndex &index, MWidget *cell) const;
	};

	class record_manager : public MApplicationPage
	{
		Q_OBJECT
		public:
			record_manager(const QString &table, QGraphicsItem *parent = 0);
			virtual ~record_manager();
			void init();
			void setTitle(const QString &t);

private Q_SLOTS:
			void open(const QModelIndex &index);
			void close(const QModelIndex &index);

Q_SIGNALS:
			void openChm(const QString &path);

		private:
			QString tableName;
			header_view *header;
			record_model *model;
			MContainer *container;

		private:
			record_manager(const record_manager &other);
			record_manager & operator= (const record_manager &other);
	};
}


#endif
