#include "karin_ut.h"
#include "database.h"

#include <QSettings>
#include <QDir>
#include <QDateTime>

#include <QDebug>

#define RECORD_NAME "karin.chm.rcd"
#define BOOKMARK_NAME "karin.chm.bmk"
#define USERNAME "karin"
#define PASSWORD "hello"

#define APP_CONFIG_VERSION "jinx"

karin::ut::ut()
	:settings(new QSettings),
	rcd_db(new karin::database(RECORD_NAME)),
	bmk_db(new karin::database(BOOKMARK_NAME))
{
	if(!rcd_db -> openDatabase())
	{
		qWarning()<<"Can not open SQL database";
	}
	else
	{
		QStringList list;
		list<<QString(RCD_VALUE) + " TEXT UNIQUE NOT NULL"
			<<QString(RCD_NAME) + " TEXT NOT NULL"
			<<QString(TIMESTAMP) + " INTERGER";
		rcd_db -> addTable(HISTORY_TABLE, list);
		QStringList list2;
		list2<<QString(RCD_VALUE) + " TEXT UNIQUE NOT NULL"
			<<QString(RCD_NAME) + " TEXT NOT NULL"
			<<QString(TIMESTAMP) + " INTERGER";
		rcd_db -> addTable(FAVORITE_TABLE, list2);
	}
	if(!bmk_db -> openDatabase())
	{
		qWarning()<<"Can not open SQL database";
	}
	else
	{
		QStringList list;
		list<<QString(BMK_INDEX_NAME) + " TEXT UNIQUE NOT NULL"
			<<QString(BMK_INDEX_VALUE) + " TEXT NOT NULL";
		bmk_db -> addTable(BOOKMARK_INDEX_TABLE, list);
	}
}

karin::ut::~ut()
{
	delete settings;
	rcd_db -> closeDatabase();
	bmk_db -> closeDatabase();
	delete rcd_db;
	delete bmk_db;
}

karin::ut * karin::ut::Instance()
{
	static karin::ut s_ut;
	return &s_ut;
}

void karin::ut::setSetting(const QString &key, const QVariant &value)
{
	if(!settings -> contains(key) || value.isNull())
	{
		QVariant defSets = karin::ut::defaultSettings(key);
		if(defSets.isNull())
			return;
		settings -> setValue(key, defSets);
#ifdef _SIMLATOR
		qDebug()<<"Setting key -> \""<<key<<"\" set to default -> "<<defSets;
#endif
	}
	else
		settings -> setValue(key, value);
}

QVariant karin::ut::getSetting(const QString &key)
{
	if(!settings -> contains(key))
		setSetting(key);
	return settings -> value(key);
}

QVariant karin::ut::defaultSettings(const QString &key)
{
	if(key == CONFIG_VERSION)
		return QVariant(APP_CONFIG_VERSION);
	else if(key == FILECHOOSER_LASTDIR)
		return QVariant(QDir::homePath());
	else if(key == FILECHOOSER_SHOWHIDDEN)
		return QVariant(false);
	else if(key == FILECHOOSER_CHMONLY)
		return QVariant(false);
	else if(key == ORIENTATION)
		return QVariant(0);
	else if(key == HANDLE_EXTERNAL_URL)
		return QVariant(true);
	else if(key == HANDLE_NEW_CHM)
		return QVariant(true);
	else if(key == SEARCH_RESULT_COUNT)
		return QVariant(100);
	else if(key == ZOOM_RANGE)
		return QVariant(2);
	else if(key == FULL_SCREEN)
		return QVariant(false);
	else if(key == SHOW_SPLASH)
		return QVariant(true);
	else if(key == SLIDE_IGNORE_ANGLE)
		return QVariant(20);
	else if(key == DOUBLE_CLICK_ZOOM)
		return QVariant(false);
	else if(key == DOUBLE_CLICK_ZOOM_RANGE)
		return QVariant(4);
	return QVariant();
}

QVariantList karin::ut::getRecordTableData(const QString &table) const
{
	QVariantList list = rcd_db -> getAllDataFromTable(table);
	QVariantList result;
	for(QVariantList::iterator itor = list.begin();
			itor != list.end();
			++itor)
		result.push_front(*itor);
	return result;
}

void karin::ut::doRecord(const QString &table, const QString &name, const QString &path)
{
	QVariantList list;
	list<<QVariant(path)
		<<QVariant(name)
		<<QDateTime::currentMSecsSinceEpoch();
	rcd_db -> addElementToTable(table, list);
}

void karin::ut::removeOneRecord(const QString &table, const QString &key, const QString &value)
{
	rcd_db -> removeElementFromTable(table, key, value);
}

void karin::ut::clearRecord(const QString &table)
{
	rcd_db -> clearTable(table);
}

bool karin::ut::hasBookmarkTable(const QString &name) const
{
	return bmk_db -> queryTable(BOOKMARK_INDEX_TABLE, BMK_INDEX_NAME, name) > 0;
}

void karin::ut::newBookmarkTable(const QString &name)
{
	QVariantList list;
	QString id = QString("karin_") + QString::number(QDateTime::currentMSecsSinceEpoch());
	list<<QVariant(name)
		<<QVariant(id);
	bmk_db -> addElementToTable(BOOKMARK_INDEX_TABLE, list);
	QStringList list2;
	list2<<QString(BMK_VALUE) + " TEXT UNIQUE NOT NULL"
		<<QString(BMK_NAME) + " TEXT NOT NULL"
		<<QString(TIMESTAMP) + " INTERGER";
	bmk_db -> addTable(id, list2);
#ifdef _SIMLATOR
	qDebug()<<id<<"__"<<bmk_db -> hasTable(id);
#endif
}

void karin::ut::deleteBookmarkTable(const QString &name)
{
	QString id = bmk_db -> queryTableValue(BOOKMARK_INDEX_TABLE, BMK_INDEX_VALUE, BMK_INDEX_NAME, name).toString();
	bmk_db -> deleteTable(id);
	bmk_db -> removeElementFromTable(BOOKMARK_INDEX_TABLE, BMK_INDEX_NAME, name);
}

void karin::ut::addBookmarkToTable(const QString &name, const QString &topic, const QString &url)
{
#ifdef _SIMLATOR
	qDebug()<<bmk_db -> queryTable(BOOKMARK_INDEX_TABLE, BMK_INDEX_NAME, name);
	qDebug()<<bmk_db -> hasTable(BOOKMARK_INDEX_TABLE);
#endif
	if(bmk_db -> queryTable(BOOKMARK_INDEX_TABLE, BMK_INDEX_NAME, name) == 0)
		newBookmarkTable(name);
	QVariantList list;
	list<<QVariant(url)
		<<QVariant(topic)
		<<QDateTime::currentMSecsSinceEpoch();
	QString id = bmk_db -> queryTableValue(BOOKMARK_INDEX_TABLE, BMK_INDEX_VALUE, BMK_INDEX_NAME, name).toString();
#ifdef _SIMLATOR
	qDebug()<<id<<"__"<<bmk_db -> hasTable(id);
#endif
	bmk_db -> addElementToTable(id, list);
}

void karin::ut::removeBookmarkFormTable(const QString &name, const QString &key, const QString &value)
{
	QString id = bmk_db -> queryTableValue(BOOKMARK_INDEX_TABLE, BMK_INDEX_VALUE, BMK_INDEX_NAME, name).toString();
	bmk_db -> removeElementFromTable(id, key, value);
	if(bmk_db -> queryTable(id) == 0)
		deleteBookmarkTable(name);
}

QVariantList karin::ut::getBookmarkTableData(const QString &name) const
{
	QString id = bmk_db -> queryTableValue(BOOKMARK_INDEX_TABLE, BMK_INDEX_VALUE, BMK_INDEX_NAME, name).toString();
	QVariantList list = bmk_db -> getAllDataFromTable(id);
	QVariantList result;
	for(QVariantList::iterator itor = list.begin();
			itor != list.end();
			++itor)
		result.push_front(*itor);
	return result;
}

bool karin::ut::isFavoriteChm(const QString &name) const
{
	return rcd_db -> queryTable(FAVORITE_TABLE, RCD_VALUE, name) == 1;
}
