#ifndef _KARIN_UT_H
#define _KARIN_UT_H

#include <QVariant>

#define HISTORY_TABLE "history_table"
#define FAVORITE_TABLE "favorite_table"

#define RCD_NAME "Name"
#define RCD_VALUE "Path"
#define TIMESTAMP "Timestamp"
#define BMK_NAME "Topic"
#define BMK_VALUE "Url"

#define BOOKMARK_INDEX_TABLE "bookmark_index_table"
#define BMK_INDEX_NAME "Path"
#define BMK_INDEX_VALUE "ChmID"

#define FILECHOOSER_LASTDIR "filechooser_lastdir"
#define FILECHOOSER_SHOWHIDDEN "filechooser_showhidden"
#define FILECHOOSER_CHMONLY "filechooser_chmonly"
#define ORIENTATION "orientation"
#define HANDLE_EXTERNAL_URL "handle_external_url"
#define HANDLE_NEW_CHM "handle_new_chm"
#define SEARCH_RESULT_COUNT "search_result_count"
#define ZOOM_RANGE "zoom_range"
#define FULL_SCREEN "fullscreen"
#define SHOW_SPLASH "show_splash"
#define SLIDE_IGNORE_ANGLE  "slide_ignore_angle"
#define DOUBLE_CLICK_ZOOM  "double_click_zoom"
#define CONFIG_VERSION "config_version"
#define DOUBLE_CLICK_ZOOM_RANGE "double_click_zoom_range"

class QSettings;
class QString;

namespace karin
{
	class database;

	class ut
	{
		public:
			~ut();
			static ut * Instance();
			void setSetting(const QString &key, const QVariant &value = QVariant());
			QVariant getSetting(const QString &key);
			template <class T> T getSetting(const QString &key);
			template <class T> void setSetting(const QString &key, const T &t);
			QVariantList getRecordTableData(const QString &table) const;
			void doRecord(const QString &table, const QString &name, const QString &path);
			void removeOneRecord(const QString &table, const QString &key, const QString &value);
			void clearRecord(const QString &table);

			bool hasBookmarkTable(const QString &name) const;
			void newBookmarkTable(const QString &name);
			void deleteBookmarkTable(const QString &name);
			void addBookmarkToTable(const QString &table, const QString &topic, const QString &url);
			void removeBookmarkFormTable(const QString &table, const QString &key, const QString &value);
			QVariantList getBookmarkTableData(const QString &table) const;

			bool isFavoriteChm(const QString &name) const;

		private:
			ut();
			QSettings *settings;
			database *rcd_db;
			database *bmk_db;
			static QVariant defaultSettings(const QString &key);

		private:
			ut(const ut &other);
			ut & operator= (const ut &other);
	};
}

template <class T>
T karin::ut::getSetting(const QString &key)
{
	QVariant var = getSetting(key);
	if(var.isNull() || !var.canConvert<T>())
		return T();
	else
		return var.value<T>();
}

template <class T>
void karin::ut::setSetting(const QString &key, const T &t)
{
	QVariant var(t);
	if(!var.isValid())
		return;
	setSetting(key, var);
}

#endif
