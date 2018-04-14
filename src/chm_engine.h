#ifndef _KARIN_CHMENGINE_H
#define _KARIN_CHMENGINE_H

#include <QString>
#include <QVector>
#include <QAbstractListModel>

#include <libchmfile.h>
#include <QObject>
#include "karin_std.h"

namespace karin
{
	class chm_engine;

	struct directory_item
	{
		QString name;
		QString url;
		int imageid;
		int indent;
		QString parent;
		bool isdir;
		//mutable bool opened;
		int index;
	};

	struct searchresult_item
	{
		QString name;
		QString url;
	};

	struct index_item
	{
		QString name;
		QVector<searchresult_item> subitem;
		int imageid;
	};

	struct encoding_item
	{
		QString family;
		QString codec;
	};

	struct directory_node
	{
		const directory_item * node;
		bool opened;
	};

	class directory_model : public QAbstractListModel
	{
		Q_OBJECT

		public:
			directory_model(QObject *parent = 0);
			virtual ~directory_model();
			virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
			virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

			public Q_SLOTS:
				void open(const QModelIndex &index);
			void init();
			void all();

		private:
			QList<directory_node> list;
			const QVector<directory_item> *m_table;

			friend class chm_engine;
	};


	class index_model : public QAbstractListModel
	{
		Q_OBJECT

		public:
			index_model(QObject *parent = 0);
			virtual ~index_model();
			virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
			virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
			inline const QVector<index_item> * table() const
			{
				return m_table;
			}

		private:
			const QVector<index_item> *m_table;

			friend class chm_engine;
	};

	class chm_engine : public QObject, private LCHMFile
	{
		Q_OBJECT

		public:
			enum URLFormat
			{
				InternalURL_or_OthersURL = 0,
				RemoteURL,
				JavaScriptURL,
				NewChmURL
			};

		private:
			index_model *cpp_indexModel;
			directory_model *cpp_directoryModel;
			QString cpp_chmfile;
			QString cpp_codec;
			QVector<directory_item> cpp_dirlist;
			QVector<index_item> cpp_indexlist;
			bool hasTOC;
			bool hasTOI;

		private:
			void parseTOC_vector2list(const QVector<LCHMParsedEntry> &v);
			bool parseTOI_vector2list(const QVector<LCHMParsedEntry> &v);
			void init();
			void doDebug(OutputType type, const QString &info);
			//static bool setCharset(QString *s);


		public:
			chm_engine(QObject *object = 0);
			virtual ~chm_engine();
			inline karin::index_model * indexModel()
			{
				return cpp_indexModel;
			}
			inline karin::directory_model * directoryModel()
			{
				return cpp_directoryModel;
			}
			OperationResult loadFile(const QString &chm);
			void closeAll();
			inline QString title() const
			{
				return LCHMFile::title();
			}
			inline QString homeUrl() const
			{
				return LCHMFile::homeUrl();
			}
			inline bool hasTableOfContents() const
			{
				return hasTOC;
			}
			inline bool hasIndexTable() const
			{
				return hasTOI;
			}
			inline bool hasSearchTable() const
			{
				return LCHMFile::hasSearchTable();
			}
			inline QString chmfile() const
			{
				return cpp_chmfile;
			}
			inline bool getFileContentAsString(QString * str, const QString& url)
			{
				return LCHMFile::getFileContentAsString(str, url);
			}
			inline bool getFileContentAsBinary(QByteArray * data, const QString& url)
			{
				return LCHMFile::getFileContentAsBinary(data, url);
			}
			inline bool getFileSize(unsigned int * size, const QString& url)
			{
				return LCHMFile::getFileSize(size, url);
			}
			inline bool enumerateFiles(QStringList * files)
			{
				return LCHMFile::enumerateFiles(files);
			}
			inline QString	getTopicByUrl(const QString& url)
			{
				return LCHMFile::getTopicByUrl(url);
			}
			//const QPixmap * getBookIconPixmap( unsigned int imagenum );
			inline QString normalizeUrl(const QString& url) const
			{
				return LCHMFile::normalizeUrl(url);
			}
			bool	searchQuery (QVector<searchresult_item> *vector, const QString& query, unsigned int limit = 100 );
			inline LCHMFileImpl * impl()
			{
				return LCHMFile::impl();
			}
			inline const QVector<index_item> * indexlist() const
			{
				return &cpp_indexlist;
			}
			inline const QVector<directory_item> * dirlist() const
			{
				return &cpp_dirlist;
			}
			bool hasFile(const QString &name);

			inline QString codec() const
			{
				return cpp_codec;
			}
			bool setTextCodec(const QString &s);
			static QVector<encoding_item> getEncodingList();
			static QString absoluteUrl(const QString &baseurl, const QString &url);
			URLFormat getURLFormat(const QString &name, QVariantMap *uu, bool dbg = false);

Q_SIGNALS:
			void dirlistChanged();
			void indexlistChanged();
			void output(int t, const QString &err);

		private:
			chm_engine(const chm_engine &other);
			chm_engine & operator= (const chm_engine &other);

	};
}
#endif
