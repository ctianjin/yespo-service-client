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
#include "screenshotdao_p.h"
#include "screenshot_p.h"
#include "uploadstatus.h"
#include "user.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_USER_SCREENSHOT_COLUMNS
#  define NS_USER_SCREENSHOT_COLUMNS "id, owner, status, meet_cdr_id, photo"
#endif

#ifndef NS_USER_SCREENSHOT_COLUMNS_PREFIXED
#  define NS_USER_SCREENSHOT_COLUMNS_PREFIXED "screenshot.id, screenshot.owner, screenshot.status, screenshot.meet_cdr_id, screenshot.photo"
#endif

namespace ns {
namespace User {

typedef QueryResult<Screenshot> ScreenshotResult;
typedef QueryResult<Screenshot, QueryResultCreatorByRecord<Screenshot> > ScreenshotResultByRecord;
typedef QueryResults<Screenshot> ScreenshotResults;
typedef QueryResults<Screenshot, QueryResultCreatorByRecord<Screenshot> > ScreenshotResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

ScreenshotDaoBase::ScreenshotDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("screenshot"))
{
}

ScreenshotDaoBase::~ScreenshotDaoBase()
{
}

bool ScreenshotDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE screenshot ( "
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "owner TEXT, "
          "status INTEGER, "
          "meet_cdr_id TEXT, "
          "photo TEXT "
        ")"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool ScreenshotDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE screenshot"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant ScreenshotDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int ScreenshotDaoBase::count(qlonglong id)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM screenshot "
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

int ScreenshotDaoBase::countByExample(const Screenshot& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const ScreenshotPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM screenshot WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_owner.isValid())
        (sqlw += a) += QLatin1String("owner=?");
    if (e->m_status.isValid())
        (sqlw += a) += QLatin1String("status=?");
    if (e->m_meetCdrId.isValid())
        (sqlw += a) += QLatin1String("meet_cdr_id=?");
    if (e->m_photo.isValid())
        (sqlw += a) += QLatin1String("photo=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_photo.isValid())
        query.addBindValue(e->m_photo);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int ScreenshotDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM screenshot"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

Screenshot ScreenshotDaoBase::find(qlonglong id)
{
    static const char sql[] =
        "SELECT "NS_USER_SCREENSHOT_COLUMNS" FROM screenshot "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return Screenshot();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(id);

    return ScreenshotResult(db)(query);
}

Screenshot ScreenshotDaoBase::findByExample(const Screenshot& entity)
{
    Database* db = database();
    if (!db)
        return Screenshot();

    const ScreenshotPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_SCREENSHOT_COLUMNS" FROM screenshot WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_owner.isValid())
        (sqlw += a) += QLatin1String("owner=?");
    if (e->m_status.isValid())
        (sqlw += a) += QLatin1String("status=?");
    if (e->m_meetCdrId.isValid())
        (sqlw += a) += QLatin1String("meet_cdr_id=?");
    if (e->m_photo.isValid())
        (sqlw += a) += QLatin1String("photo=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_photo.isValid())
        query.addBindValue(e->m_photo);

    return ScreenshotResult(db)(query);
}

ScreenshotList ScreenshotDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "NS_USER_SCREENSHOT_COLUMNS" FROM screenshot"
        ;

    Database* db = database();
    if (!db)
        return ScreenshotList();

    return ScreenshotResults(db)(sqlDecorate(sql));
}

ScreenshotList ScreenshotDaoBase::findAllByExample(const Screenshot& entity)
{
    Database* db = database();
    if (!db)
        return ScreenshotList();

    const ScreenshotPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_SCREENSHOT_COLUMNS" FROM screenshot WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_owner.isValid())
        (sqlw += a) += QLatin1String("owner=?");
    if (e->m_status.isValid())
        (sqlw += a) += QLatin1String("status=?");
    if (e->m_meetCdrId.isValid())
        (sqlw += a) += QLatin1String("meet_cdr_id=?");
    if (e->m_photo.isValid())
        (sqlw += a) += QLatin1String("photo=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_photo.isValid())
        query.addBindValue(e->m_photo);

    return ScreenshotResults(db)(query);
}

bool ScreenshotDaoBase::persist(Screenshot& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const ScreenshotPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO screenshot ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_owner.isValid()) {
        (sql += c) += QLatin1String("owner");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_status.isValid()) {
        (sql += c) += QLatin1String("status");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_meetCdrId.isValid()) {
        (sql += c) += QLatin1String("meet_cdr_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_photo.isValid()) {
        (sql += c) += QLatin1String("photo");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_photo.isValid())
        query.addBindValue(e->m_photo);

    if (db->exec(query)) {
        const QVariant id = query.lastInsertId();
        entity.d->m_id = id;
        return true;
    } else {
        return false;
    }
}

bool ScreenshotDaoBase::merge(const Screenshot& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const ScreenshotPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO screenshot ("));
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
    if (e->m_status.isValid()) {
        (sql += c) += QLatin1String("status");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_meetCdrId.isValid()) {
        (sql += c) += QLatin1String("meet_cdr_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_photo.isValid()) {
        (sql += c) += QLatin1String("photo");
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
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_photo.isValid())
        query.addBindValue(e->m_photo);

    return db->exec(query);
}

bool ScreenshotDaoBase::update(const Screenshot& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const ScreenshotPrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE screenshot SET "));
    QString sqlv;
    SqlComma c;
    if (e->m_owner.isValid())
        (sqlv += c) += QLatin1String("owner=?");
    if (e->m_status.isValid())
        (sqlv += c) += QLatin1String("status=?");
    if (e->m_meetCdrId.isValid())
        (sqlv += c) += QLatin1String("meet_cdr_id=?");
    if (e->m_photo.isValid())
        (sqlv += c) += QLatin1String("photo=?");

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QLatin1String(" WHERE id=?");

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_owner.isValid())
        query.addBindValue(e->m_owner);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_meetCdrId.isValid())
        query.addBindValue(e->m_meetCdrId);
    if (e->m_photo.isValid())
        query.addBindValue(e->m_photo);
    query.addBindValue(e->m_id);

    return db->exec(query);
}

bool ScreenshotDaoBase::remove(const Screenshot& entity)
{
    static const char sql[] =
        "DELETE FROM screenshot "
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

bool ScreenshotDaoBase::removeAll(bool resetIdentity)
{
    static const char sql[] =
        "DELETE FROM screenshot"
        ;
    static const char sqli[] =
        "DELETE FROM sqlite_sequence WHERE name='screenshot'"
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

bool ScreenshotDaoBase::refresh(Screenshot& entity)
{
    static const char sql[] =
        "SELECT "NS_USER_SCREENSHOT_COLUMNS" FROM screenshot "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const ScreenshotPrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_id);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = Screenshot(query);
    return true;
}

//$BEGIN-CUSTOM$
ScreenshotDao::ScreenshotDao(Database* db) :
    ScreenshotDaoBase(db)
{
}

ScreenshotDao::~ScreenshotDao()
{
}

bool ScreenshotDao::createTable()
{
    return ScreenshotDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_screenshot_xx ON screenshot ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!ScreenshotDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool ScreenshotDao::dropTable()
{
    return ScreenshotDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}

bool ScreenshotDao::updateMeetCDRIdByOwner(const QString& owner, const QString& Id)
{
	Database* db = database();
	if (!db)
		return false;

	QString sql(QLatin1String("UPDATE screenshot SET "));
	QString sqlv;
	SqlComma c;

	(sqlv += c) += QLatin1String("meet_cdr_id=?");

	if (sqlv.isEmpty())
		return true;
	sql += sqlv;
	sql += QLatin1String(" WHERE owner=? AND meet_cdr_id IS NULL");

	QSqlQuery query = db->query();
	query.prepare(sql);

	query.addBindValue(Id);

	query.addBindValue(owner);

	return db->exec(query);
}

ScreenshotList ScreenshotDao::findIdleByOwner(const QString& owner)
{
	Database* db = database();
	if (!db)
		return ScreenshotList();

	QString sql(QLatin1String("SELECT "NS_USER_SCREENSHOT_COLUMNS_PREFIXED" FROM screenshot WHERE "));
	QString sqlw;
	SqlAnd a;
	
	(sqlw += a) += QLatin1String("owner=?");
	(sqlw += a) += QLatin1String("status=?");
	(sqlw += a) += QLatin1String("meet_cdr_id NOT NULL");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorate(sql));

	query.addBindValue(owner);
	query.addBindValue(Idle);

	return ScreenshotResults(db)(query);
}

bool ScreenshotDao::updateUploadStatus(const QString& owner, const QString& Id, UploadStatus status)
{
	Database* db = database();
	if (!db)
		return false;

	QString sql(QLatin1String("UPDATE screenshot SET "));
	QString sqlv;
	SqlComma c;

	(sqlv += c) += QLatin1String("status=?");

	if (sqlv.isEmpty())
		return true;
	sql += sqlv;
	sql += QLatin1String(" WHERE owner=? AND meet_cdr_id=?");

	QSqlQuery query = db->query();
	query.prepare(sql);

	query.addBindValue(status);
	query.addBindValue(owner);
	query.addBindValue(Id);

	return db->exec(query);
}

bool ScreenshotDao::updateUploadStatusByPhoto(const QString& owner, const QString& Id, const QString& photo, UploadStatus status)
{
	Database* db = database();
	if (!db)
		return false;

	QString sql(QLatin1String("UPDATE screenshot SET "));
	QString sqlv;
	SqlComma c;

	(sqlv += c) += QLatin1String("status=?");

	if (sqlv.isEmpty())
		return true;
	sql += sqlv;
	sql += QLatin1String(" WHERE owner=? AND meet_cdr_id=? AND photo=?");

	QSqlQuery query = db->query();
	query.prepare(sql);

	query.addBindValue(status);
	query.addBindValue(owner);
	query.addBindValue(Id);
	query.addBindValue(photo);

	return db->exec(query);
}

//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
