// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include "blacklistdao_p.h"
#include "blacklist_p.h"
#include "user.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_USER_BLACKLIST_COLUMNS
#  define NS_USER_BLACKLIST_COLUMNS "id, owner, uniqueId, blacklist_id, partner_sys_id, ve_sys_id, display_name, small_photo"
#endif

#ifndef NS_USER_BLACKLIST_COLUMNS_PREFIXED
#  define NS_USER_BLACKLIST_COLUMNS_PREFIXED "blacklist.id, blacklist.owner, blacklist.uniqueId, blacklist.blacklist_id, blacklist.partner_sys_id, blacklist.ve_sys_id, blacklist.display_name, blacklist.small_photo"
#endif

namespace ns {
namespace User {

typedef QueryResult<Blacklist> BlacklistResult;
typedef QueryResult<Blacklist, QueryResultCreatorByRecord<Blacklist> > BlacklistResultByRecord;
typedef QueryResults<Blacklist> BlacklistResults;
typedef QueryResults<Blacklist, QueryResultCreatorByRecord<Blacklist> > BlacklistResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

BlacklistDaoBase::BlacklistDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("blacklist"))
{
}

BlacklistDaoBase::~BlacklistDaoBase()
{
}

bool BlacklistDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE blacklist ( "
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "owner TEXT, "
          "uniqueId TEXT, "
          "blacklist_id TEXT, "
          "partner_sys_id TEXT, "
          "ve_sys_id TEXT, "
          "display_name TEXT, "
          "small_photo TEXT "
        ")"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool BlacklistDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE blacklist"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant BlacklistDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int BlacklistDaoBase::count(qlonglong id)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM blacklist "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return -1;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(id);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int BlacklistDaoBase::countByExample(const Blacklist& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const BlacklistPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM blacklist WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_owner.isValid())
        (sqlw += a) += QLatin1String("owner=?");
    if (e->m_uniqueid.isValid())
        (sqlw += a) += QLatin1String("uniqueId=?");
    if (e->m_blacklistId.isValid())
        (sqlw += a) += QLatin1String("blacklist_id=?");
    if (e->m_partnerSysId.isValid())
        (sqlw += a) += QLatin1String("partner_sys_id=?");
    if (e->m_veSysId.isValid())
        (sqlw += a) += QLatin1String("ve_sys_id=?");
    if (e->m_displayName.isValid())
        (sqlw += a) += QLatin1String("display_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_uniqueid.isValid())
        query.addBindValue(e->m_uniqueid);
    if (e->m_blacklistId.isValid())
        query.addBindValue(e->m_blacklistId);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_veSysId.isValid())
        query.addBindValue(e->m_veSysId);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int BlacklistDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM blacklist"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

Blacklist BlacklistDaoBase::find(qlonglong id)
{
    static const char sql[] =
        "SELECT "NS_USER_BLACKLIST_COLUMNS" FROM blacklist "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return Blacklist();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(id);

    return BlacklistResult(db)(query);
}

Blacklist BlacklistDaoBase::findByExample(const Blacklist& entity)
{
    Database* db = database();
    if (!db)
        return Blacklist();

    const BlacklistPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_BLACKLIST_COLUMNS" FROM blacklist WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_owner.isValid())
        (sqlw += a) += QLatin1String("owner=?");
    if (e->m_uniqueid.isValid())
        (sqlw += a) += QLatin1String("uniqueId=?");
    if (e->m_blacklistId.isValid())
        (sqlw += a) += QLatin1String("blacklist_id=?");
    if (e->m_partnerSysId.isValid())
        (sqlw += a) += QLatin1String("partner_sys_id=?");
    if (e->m_veSysId.isValid())
        (sqlw += a) += QLatin1String("ve_sys_id=?");
    if (e->m_displayName.isValid())
        (sqlw += a) += QLatin1String("display_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_uniqueid.isValid())
        query.addBindValue(e->m_uniqueid);
    if (e->m_blacklistId.isValid())
        query.addBindValue(e->m_blacklistId);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_veSysId.isValid())
        query.addBindValue(e->m_veSysId);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);

    return BlacklistResult(db)(query);
}

BlacklistList BlacklistDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "NS_USER_BLACKLIST_COLUMNS" FROM blacklist"
        ;

    Database* db = database();
    if (!db)
        return BlacklistList();

    return BlacklistResults(db)(sqlDecorate(sql));
}

BlacklistList BlacklistDaoBase::findAllByExample(const Blacklist& entity)
{
    Database* db = database();
    if (!db)
        return BlacklistList();

    const BlacklistPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_BLACKLIST_COLUMNS" FROM blacklist WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_owner.isValid())
        (sqlw += a) += QLatin1String("owner=?");
    if (e->m_uniqueid.isValid())
        (sqlw += a) += QLatin1String("uniqueId=?");
    if (e->m_blacklistId.isValid())
        (sqlw += a) += QLatin1String("blacklist_id=?");
    if (e->m_partnerSysId.isValid())
        (sqlw += a) += QLatin1String("partner_sys_id=?");
    if (e->m_veSysId.isValid())
        (sqlw += a) += QLatin1String("ve_sys_id=?");
    if (e->m_displayName.isValid())
        (sqlw += a) += QLatin1String("display_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_uniqueid.isValid())
        query.addBindValue(e->m_uniqueid);
    if (e->m_blacklistId.isValid())
        query.addBindValue(e->m_blacklistId);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_veSysId.isValid())
        query.addBindValue(e->m_veSysId);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);

    return BlacklistResults(db)(query);
}

bool BlacklistDaoBase::persist(Blacklist& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const BlacklistPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO blacklist ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_owner.isValid()) {
        (sql += c) += QLatin1String("owner");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_uniqueid.isValid()) {
        (sql += c) += QLatin1String("uniqueId");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_blacklistId.isValid()) {
        (sql += c) += QLatin1String("blacklist_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerSysId.isValid()) {
        (sql += c) += QLatin1String("partner_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_veSysId.isValid()) {
        (sql += c) += QLatin1String("ve_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_displayName.isValid()) {
        (sql += c) += QLatin1String("display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_smallPhoto.isValid()) {
        (sql += c) += QLatin1String("small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_uniqueid.isValid())
        query.addBindValue(e->m_uniqueid);
    if (e->m_blacklistId.isValid())
        query.addBindValue(e->m_blacklistId);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_veSysId.isValid())
        query.addBindValue(e->m_veSysId);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);

    if (db->exec(query)) {
        const QVariant id = query.lastInsertId();
        entity.d->m_id = id;
        return true;
    } else {
        return false;
    }
}

bool BlacklistDaoBase::merge(const Blacklist& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const BlacklistPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO blacklist ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_id.isValid()) {
        (sql += c) += QLatin1String("id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_owner.isValid()) {
        (sql += c) += QLatin1String("owner");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_uniqueid.isValid()) {
        (sql += c) += QLatin1String("uniqueId");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_blacklistId.isValid()) {
        (sql += c) += QLatin1String("blacklist_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_partnerSysId.isValid()) {
        (sql += c) += QLatin1String("partner_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_veSysId.isValid()) {
        (sql += c) += QLatin1String("ve_sys_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_displayName.isValid()) {
        (sql += c) += QLatin1String("display_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_smallPhoto.isValid()) {
        (sql += c) += QLatin1String("small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_uniqueid.isValid())
        query.addBindValue(e->m_uniqueid);
    if (e->m_blacklistId.isValid())
        query.addBindValue(e->m_blacklistId);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_veSysId.isValid())
        query.addBindValue(e->m_veSysId);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);

    return db->exec(query);
}

bool BlacklistDaoBase::update(const Blacklist& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const BlacklistPrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE blacklist SET "));
    QString sqlv;
    SqlComma c;
    if (e->m_owner.isValid())
        (sqlv += c) += QLatin1String("owner=?");
    if (e->m_uniqueid.isValid())
        (sqlv += c) += QLatin1String("uniqueId=?");
    if (e->m_blacklistId.isValid())
        (sqlv += c) += QLatin1String("blacklist_id=?");
    if (e->m_partnerSysId.isValid())
        (sqlv += c) += QLatin1String("partner_sys_id=?");
    if (e->m_veSysId.isValid())
        (sqlv += c) += QLatin1String("ve_sys_id=?");
    if (e->m_displayName.isValid())
        (sqlv += c) += QLatin1String("display_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlv += c) += QLatin1String("small_photo=?");

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QLatin1String(" WHERE id=?");

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_uniqueid.isValid())
        query.addBindValue(e->m_uniqueid);
    if (e->m_blacklistId.isValid())
        query.addBindValue(e->m_blacklistId);
    if (e->m_partnerSysId.isValid())
        query.addBindValue(e->m_partnerSysId);
    if (e->m_veSysId.isValid())
        query.addBindValue(e->m_veSysId);
    if (e->m_displayName.isValid())
        query.addBindValue(e->m_displayName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    query.addBindValue(e->m_id);

    return db->exec(query);
}

bool BlacklistDaoBase::remove(const Blacklist& entity)
{
    static const char sql[] =
        "DELETE FROM blacklist "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    QSqlQuery query = db->query();
    query.prepare(QLatin1String(sql));
    query.addBindValue(entity.d->m_id);

    return db->exec(query);
}

bool BlacklistDaoBase::removeAll(bool resetIdentity)
{
    static const char sql[] =
        "DELETE FROM blacklist"
        ;
    static const char sqli[] =
        "DELETE FROM sqlite_sequence WHERE name='blacklist'"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (resetIdentity) {
        Transaction trans(db);
        if (!db->exec(QLatin1String(sql)))
            return false;
        if (!db->exec(QLatin1String(sqli)))
            return false;
        return trans.commit();
    } else {
        return db->exec(QLatin1String(sql));
    }
}

bool BlacklistDaoBase::refresh(Blacklist& entity)
{
    static const char sql[] =
        "SELECT "NS_USER_BLACKLIST_COLUMNS" FROM blacklist "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const BlacklistPrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_id);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = Blacklist(query);
    return true;
}

//$BEGIN-CUSTOM$
BlacklistDao::BlacklistDao(Database* db) :
    BlacklistDaoBase(db)
{
}

BlacklistDao::~BlacklistDao()
{
}

bool BlacklistDao::createTable()
{
    return BlacklistDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_blacklist_xx ON blacklist ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!BlacklistDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool BlacklistDao::dropTable()
{
    return BlacklistDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}

Blacklist BlacklistDao::findRecentByOwner(const QString& owner)
{
	Database* db = database();
	if (!db)
		return Blacklist();

	QString sql(QLatin1String("SELECT "NS_USER_BLACKLIST_COLUMNS" FROM blacklist WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("owner=?");

	sql += sqlw;
	sql += QLatin1String(" ORDER BY blacklist_id DESC LIMIT 0,1");

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sql);

	query.addBindValue(owner);

	return BlacklistResult(db)(query);
}

BlacklistList BlacklistDao::findAllByOwner(const QString& owner)
{
	Database* db = database();
	if (!db)
		return BlacklistList();

	QString sql(QLatin1String("SELECT "NS_USER_BLACKLIST_COLUMNS" FROM blacklist WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("owner=?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorate(sql));

	query.addBindValue(owner);

	return BlacklistResults(db)(query);
}

Blacklist BlacklistDao::findBlacklistIdByName(const QString& name)
{
	Database* db = database();
	if (!db)
		return Blacklist();

	QString sql(QLatin1String("SELECT id FROM blacklist WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("display_name=?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorate(sql));

	query.addBindValue(name);

	return BlacklistResult(db)(query);
}

void BlacklistDao::removeRecentById(const QString& id)
{
	Database* db = database();
	if (!db)
		return ;

	QString sql(QLatin1String("DELETE FROM blacklist WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("ve_sys_id=?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorate(sql));

	query.addBindValue(id);

	db->exec(query);
}

//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
