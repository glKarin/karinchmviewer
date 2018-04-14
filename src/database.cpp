#include "database.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QSqlTableModel>
#ifdef _HARMATTAN
#include <QDir>
#include <QFileInfo>
#include <QFile>
#endif
#include <QStringList>
#include <QDebug>

#define HARMATTAN_SQL "QSQLITE"

karin::database::database(const QString &name, const QString &user, const QString &password)
{
	db = new QSqlDatabase(QSqlDatabase::addDatabase(HARMATTAN_SQL, name));
#ifdef _HARMATTAN
	QString path = QDir::homePath() + "/." + M_APPLICATION_NAME;
	QFileInfo info(path);
	if(!info.exists())
		QDir(path).mkpath(path);
	if(!info.isDir())
	{
		QFile::remove(path);
		QDir(path).mkpath(path);
	}
	db -> setDatabaseName(path + "/" + name + ".db");
#else
	db -> setDatabaseName(name + ".db");
#endif
	if(!user.isEmpty() && !password.isEmpty())
	{
		db -> setUserName(user);
		db -> setPassword(password);
	}
	db -> setHostName(DEVELOPER);
}

karin::database::~database()
{
	closeDatabase();
	delete db;
}

bool karin::database::openDatabase()
{
	if(db -> isValid())
	{
		return db -> open(); //???
	}
	return false;
}

bool karin::database::check() const
{
	return db -> isValid() && db -> isOpen();
}

QVariantList karin::database::getAllDataFromTable(const QString &name) const
{
	QVariantList list;
	if(check())
	{
		if(!db -> tables().contains(name))
			return list;
		QSqlTableModel model(0, *db);
		model.setTable(name);
		model.select();
#ifdef _SIMLATOR
		qDebug()<<model.rowCount();
#endif
		for (int i = 0; i < model.rowCount(); i++)
		{
			QSqlRecord record = model.record(i);
			QMap<QString, QVariant> map;
			for(int j = 0; j < record.count(); j++)
			{
				map.insert(record.field(j).name(), record.value(j));
			}
			list.push_back(QVariant(map));
		}
	}
	return list;
}

void karin::database::addTable(const QString &name, const QStringList &args, bool over)
{
	if(!check())
		return;
	QSqlQuery query(*db);
	if(db -> tables().contains(name))
	{
		if(over)
		{
			if(db -> transaction())
			{
				query.exec(QString("DROP TABLE IF EXISTS %1").arg(name));
				qDebug()<<QString("DROP TABLE IF EXISTS %1").arg(name);
				query.exec(QString("CREATE TABLE IF NOT EXISTS %1 (%2)").arg(name).arg(args.join(",")));
				qDebug()<<QString("CREATE TABLE IF NOT EXISTS %1 (%2)").arg(name).arg(args.join(","));
				if(!db -> commit())
				{
					qDebug()<<"SQL ERR -> "<<query.lastError().text();
					if(!db -> rollback())
					{
						qDebug()<<"SQL ERR -> "<<query.lastError().text();
					}
				}
			}
			else
			{
				if(exec(query, QString("DROP TABLE IF EXISTS %1").arg(name)))
					exec(query, QString("CREATE TABLE IF NOT EXISTS %1 (%2)").arg(name).arg(args.join(",")));
			}
		}
		else
		{
			return;
		}
	}
	else
	{
		exec(query, QString("CREATE TABLE IF NOT EXISTS %1 (%2)").arg(name).arg(args.join(",")));
	}
}

// select count(key) form table where ? = ?...
int karin::database::queryTable(const QString &name, const QString &key) const
{
	if(check())
	{
		QSqlQuery query(*db);
		if(!exec(query, QString("SELECT %1 FROM %2").arg(key).arg(name)))
			return -1;
		int i = 0;
		while(query.next())
			i++;
		return i;
	}
	return -1;
}

int karin::database::queryTable(const QString &name, const QString &key, const QString &value) const
{
	if(check())
	{
		QSqlQuery query(*db);
		if(!exec(query, QString("SELECT * FROM %1 WHERE %2 = '%3'").arg(name).arg(key).arg(value)))
			return -1;
		int i = 0;
		while(query.next())
			i++;
		return i;
	}
	return -1;
}

QVariant karin::database::queryTableValue(const QString &name, const QString &field, const QString &key, const QString &value) const
{
	if(check())
	{
		QSqlQuery query(*db);
		if(!exec(query, QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg(field).arg(name).arg(key).arg(value)))
			return QVariant();
		if(query.next())
			return query.value(0);
	}
	return QVariant();
}

void karin::database::addElementToTable(const QString &name, const QVariantList &args)
{
	if(!check())
		return;
	QStringList sl;
	for(int i = 0; i < args.length(); i++)
	{
		sl<<"?";
	}
	QSqlQuery query(*db);
	query.prepare(QString("INSERT OR REPLACE INTO %1 VALUES (%2)").arg(name).arg(sl.join(",")));
	qDebug()<<QString("INSERT OR REPLACE INTO %1 VALUES (%2)").arg(name).arg(sl.join(","));
	for(QVariantList::const_iterator itor = args.constBegin();
			itor != args.constEnd();
			++itor)
	{
		query.addBindValue(*itor);
	}
	exec(query);
}

void karin::database::removeElementFromTable(const QString &name, const QString &key, const QString &value)
{
	if(!check())
		return;
	QSqlQuery query(*db);
	exec(query, QString("DELETE FROM %1 WHERE %2 = '%3'").arg(name).arg(key).arg(value));
}

void karin::database::clearTable(const QString &name)
{
	if(!check())
		return;
	QSqlQuery query(*db);
	exec(query, QString("DELETE FROM %1").arg(name));
}

void karin::database::closeDatabase()
{
	if(check())
		db -> close();
}

bool karin::database::hasTable(const QString &name)
{
	if(check())
		return db -> tables().contains(name);
	return false;
}

void karin::database::deleteTable(const QString &name)
{
	if(!check())
		return;
	QSqlQuery query(*db);
	exec(query, QString("DROP TABLE IF EXISTS %1").arg(name));
}

bool karin::database::exec(QSqlQuery &query, const QString &cmd) const
{
	qDebug()<<cmd;
	bool b = query.exec(cmd);
	if(!b)
		qDebug()<<"SQL ERR -> "<<query.lastError().text();
	return b;
}
bool karin::database::exec(QSqlQuery &query) const
{
	bool b = query.exec();
	if(!b)
		qDebug()<<"SQL ERR -> "<<query.lastError().text();
	return b;
}
