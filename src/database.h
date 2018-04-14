#ifndef _KARIN_DATABASE_H
#define _KARIN_DATABASE_H

#include <QString>
#include <QVariant>
#include <QSqlDatabase>

namespace karin
{
	class database
	{
		public:
			database(const QString &name, const QString &user = QString(), const QString &password = QString());
			~database();
			bool openDatabase();
			void addTable(const QString &name, const QStringList &args, bool over = false);
			int queryTable(const QString &name, const QString &key, const QString &value) const;
			int queryTable(const QString &name, const QString &key = "*") const;
			QVariant queryTableValue(const QString &name, const QString &field, const QString &key, const QString &value) const;
			QVariantList getAllDataFromTable(const QString &name) const;
			void addElementToTable(const QString &name, const QVariantList &args);
			void removeElementFromTable(const QString &name, const QString &key, const QString &value);
			void clearTable(const QString &name);
			void closeDatabase();
			bool hasTable(const QString &name);
			void deleteTable(const QString &name);

		private:
			bool check() const;
			bool exec(QSqlQuery &query, const QString &cmd) const;
			bool exec(QSqlQuery &query) const;

		private:
			QSqlDatabase *db;

	};
}

#endif
