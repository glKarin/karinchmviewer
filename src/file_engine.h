#ifndef _KARIN_FILEENGINE_H
#define _KARIN_FILEENGINE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QFile>

class QFileSystemWatcher;

namespace karin
{
	struct file_info
	{
		QString path;
		QString name;
		QString owner;
		QString group;
		QString permission;
		QString size;
		bool type;
	};

	class file_engine : public QObject
	{
		Q_OBJECT
		private:
			QString cpp_path;
			QVector<file_info> cpp_filelist;
			QStringList cpp_filters;
			bool cpp_showHidden;
			QFileSystemWatcher *watcher;

		private:
			void reset();
			void init();
			static QString permissionCode2String(QFile::Permissions per);
			static QString size2FormatString(qint64 size, bool isdir = false);

			private Q_SLOTS:
				void doDirectoryChanged(const QString &);

Q_SIGNALS:
			void filelistChanged();
			void pathChanged();

		public:
			file_engine(QObject *object = 0);
			virtual ~file_engine();
			inline QString path() const
			{
				return cpp_path;
			}
			void setPath(const QString &path);
			void setFilters(const QStringList &list);
			inline const QVector<file_info> * filelist() const
			{
				return &cpp_filelist;
			}
			inline QStringList filters() const
			{
				return cpp_filters;
			}
			void setShowHidden(bool b);
			inline bool showHidden() const
			{
				return cpp_showHidden;
			}

		private:
			file_engine(const file_engine &other);
			file_engine & operator= (const file_engine &other);

	};
}

#endif
