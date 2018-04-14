#ifndef _KARIN_BOOKMARKDIALOG_H
#define _KARIN_BOOKMARKDIALOG_H

#include <MDialog>
#include <QAbstractListModel>
#include <MAbstractCellCreator>

namespace karin
{

	class editable_delegate;

	class bookmark_model : public QAbstractListModel
	{
		Q_OBJECT
		public:
			bookmark_model(QObject *parent = 0);
			virtual ~bookmark_model();
			void init(const QString &file);
			virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
			virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

		private:
			QVariantList list;
	};

	class bookmark_delegate : public MAbstractCellCreator<editable_delegate>
	{
		public:
			bookmark_delegate();
			virtual ~bookmark_delegate();
			virtual void updateCell(const QModelIndex &index, MWidget *cell) const;
	};

	class bookmark_dialog : public MDialog
	{
		Q_OBJECT

		public:
			bookmark_dialog();
			virtual ~bookmark_dialog();
			void init(const QString &file, const QString &topic, const QString &url);

Q_SIGNALS:
			void openUrl(const QString &url);

			private Q_SLOTS:
			void addBookmark();
			void open(const QModelIndex &index);
			void removeBookmark(const QModelIndex &index);

		private:
			QString m_file;
			QString m_topic;
			QString m_url;
			bookmark_model *model;

		private:
			bookmark_dialog(const bookmark_dialog &other);
			bookmark_dialog & operator= (const bookmark_dialog &other);

	};
}

#endif
