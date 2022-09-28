// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include "mediaphotodao_p.h"
#include "mediaphoto_p.h"
#include "mediastatus.h"
#include "media.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_MEDIA_MEDIAPHOTO_COLUMNS
#  define NS_MEDIA_MEDIAPHOTO_COLUMNS "id, download_url, status, total_size, available_size, fileName"
#endif

#ifndef NS_MEDIA_MEDIAPHOTO_COLUMNS_PREFIXED
#  define NS_MEDIA_MEDIAPHOTO_COLUMNS_PREFIXED "media_photo.id, media_photo.download_url, media_photo.status, media_photo.total_size, media_photo.available_size, media_photo.fileName"
#endif

namespace ns {
namespace Media {

typedef QueryResult<MediaPhoto> MediaPhotoResult;
typedef QueryResult<MediaPhoto, QueryResultCreatorByRecord<MediaPhoto> > MediaPhotoResultByRecord;
typedef QueryResults<MediaPhoto> MediaPhotoResults;
typedef QueryResults<MediaPhoto, QueryResultCreatorByRecord<MediaPhoto> > MediaPhotoResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

MediaPhotoDaoBase::MediaPhotoDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("media_photo"))
{
}

MediaPhotoDaoBase::~MediaPhotoDaoBase()
{
}

bool MediaPhotoDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE media_photo ( "
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "download_url TEXT, "
          "status INTEGER, "
          "total_size TEXT, "
          "available_size TEXT, "
          "fileName TEXT "
        ")"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool MediaPhotoDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE media_photo"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant MediaPhotoDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int MediaPhotoDaoBase::count(qlonglong id)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM media_photo "
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

int MediaPhotoDaoBase::countByExample(const MediaPhoto& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const MediaPhotoPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM media_photo WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_downloadUrl.isValid())
        (sqlw += a) += QLatin1String("download_url=?");
    if (e->m_status.isValid())
        (sqlw += a) += QLatin1String("status=?");
    if (e->m_totalSize.isValid())
        (sqlw += a) += QLatin1String("total_size=?");
    if (e->m_availableSize.isValid())
        (sqlw += a) += QLatin1String("available_size=?");
    if (e->m_filename.isValid())
        (sqlw += a) += QLatin1String("fileName=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_downloadUrl.isValid())
        query.addBindValue(e->m_downloadUrl);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);
    if (e->m_filename.isValid())
        query.addBindValue(e->m_filename);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int MediaPhotoDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM media_photo"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

MediaPhoto MediaPhotoDaoBase::find(qlonglong id)
{
    static const char sql[] =
        "SELECT "NS_MEDIA_MEDIAPHOTO_COLUMNS" FROM media_photo "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return MediaPhoto();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(id);

    return MediaPhotoResult(db)(query);
}

MediaPhoto MediaPhotoDaoBase::findByExample(const MediaPhoto& entity)
{
    Database* db = database();
    if (!db)
        return MediaPhoto();

    const MediaPhotoPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_MEDIA_MEDIAPHOTO_COLUMNS" FROM media_photo WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_downloadUrl.isValid())
        (sqlw += a) += QLatin1String("download_url=?");
    if (e->m_status.isValid())
        (sqlw += a) += QLatin1String("status=?");
    if (e->m_totalSize.isValid())
        (sqlw += a) += QLatin1String("total_size=?");
    if (e->m_availableSize.isValid())
        (sqlw += a) += QLatin1String("available_size=?");
    if (e->m_filename.isValid())
        (sqlw += a) += QLatin1String("fileName=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_downloadUrl.isValid())
        query.addBindValue(e->m_downloadUrl);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);
    if (e->m_filename.isValid())
        query.addBindValue(e->m_filename);

    return MediaPhotoResult(db)(query);
}

MediaPhotoList MediaPhotoDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "NS_MEDIA_MEDIAPHOTO_COLUMNS" FROM media_photo"
        ;

    Database* db = database();
    if (!db)
        return MediaPhotoList();

    return MediaPhotoResults(db)(sqlDecorate(sql));
}

MediaPhotoList MediaPhotoDaoBase::findAllByExample(const MediaPhoto& entity)
{
    Database* db = database();
    if (!db)
        return MediaPhotoList();

    const MediaPhotoPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_MEDIA_MEDIAPHOTO_COLUMNS" FROM media_photo WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_downloadUrl.isValid())
        (sqlw += a) += QLatin1String("download_url=?");
    if (e->m_status.isValid())
        (sqlw += a) += QLatin1String("status=?");
    if (e->m_totalSize.isValid())
        (sqlw += a) += QLatin1String("total_size=?");
    if (e->m_availableSize.isValid())
        (sqlw += a) += QLatin1String("available_size=?");
    if (e->m_filename.isValid())
        (sqlw += a) += QLatin1String("fileName=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_downloadUrl.isValid())
        query.addBindValue(e->m_downloadUrl);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);
    if (e->m_filename.isValid())
        query.addBindValue(e->m_filename);

    return MediaPhotoResults(db)(query);
}

bool MediaPhotoDaoBase::persist(MediaPhoto& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const MediaPhotoPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO media_photo ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_downloadUrl.isValid()) {
        (sql += c) += QLatin1String("download_url");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_status.isValid()) {
        (sql += c) += QLatin1String("status");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_totalSize.isValid()) {
        (sql += c) += QLatin1String("total_size");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_availableSize.isValid()) {
        (sql += c) += QLatin1String("available_size");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_filename.isValid()) {
        (sql += c) += QLatin1String("fileName");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_downloadUrl.isValid())
        query.addBindValue(e->m_downloadUrl);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);
    if (e->m_filename.isValid())
        query.addBindValue(e->m_filename);

    if (db->exec(query)) {
        const QVariant id = query.lastInsertId();
        entity.d->m_id = id;
        return true;
    } else {
        return false;
    }
}

bool MediaPhotoDaoBase::merge(const MediaPhoto& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const MediaPhotoPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO media_photo ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_id.isValid()) {
        (sql += c) += QLatin1String("id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_downloadUrl.isValid()) {
        (sql += c) += QLatin1String("download_url");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_status.isValid()) {
        (sql += c) += QLatin1String("status");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_totalSize.isValid()) {
        (sql += c) += QLatin1String("total_size");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_availableSize.isValid()) {
        (sql += c) += QLatin1String("available_size");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_filename.isValid()) {
        (sql += c) += QLatin1String("fileName");
        (sqlv += cv) += QLatin1String("?");
    }
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_downloadUrl.isValid())
        query.addBindValue(e->m_downloadUrl);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);
    if (e->m_filename.isValid())
        query.addBindValue(e->m_filename);

    return db->exec(query);
}

bool MediaPhotoDaoBase::update(const MediaPhoto& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const MediaPhotoPrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE media_photo SET "));
    QString sqlv;
    SqlComma c;
    if (e->m_downloadUrl.isValid())
        (sqlv += c) += QLatin1String("download_url=?");
    if (e->m_status.isValid())
        (sqlv += c) += QLatin1String("status=?");
    if (e->m_totalSize.isValid())
        (sqlv += c) += QLatin1String("total_size=?");
    if (e->m_availableSize.isValid())
        (sqlv += c) += QLatin1String("available_size=?");
    if (e->m_filename.isValid())
        (sqlv += c) += QLatin1String("fileName=?");

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QLatin1String(" WHERE id=?");

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_downloadUrl.isValid())
        query.addBindValue(e->m_downloadUrl);
    if (e->m_status.isValid())
        query.addBindValue(e->m_status);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);
    if (e->m_filename.isValid())
        query.addBindValue(e->m_filename);
    query.addBindValue(e->m_id);

    return db->exec(query);
}

bool MediaPhotoDaoBase::remove(const MediaPhoto& entity)
{
    static const char sql[] =
        "DELETE FROM media_photo "
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

bool MediaPhotoDaoBase::removeAll(bool resetIdentity)
{
    static const char sql[] =
        "DELETE FROM media_photo"
        ;
    static const char sqli[] =
        "DELETE FROM sqlite_sequence WHERE name='media_photo'"
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

bool MediaPhotoDaoBase::refresh(MediaPhoto& entity)
{
    static const char sql[] =
        "SELECT "NS_MEDIA_MEDIAPHOTO_COLUMNS" FROM media_photo "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const MediaPhotoPrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_id);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = MediaPhoto(query);
    return true;
}

//$BEGIN-CUSTOM$
MediaPhotoDao::MediaPhotoDao(Database* db) :
    MediaPhotoDaoBase(db)
{
}

MediaPhotoDao::~MediaPhotoDao()
{
}

bool MediaPhotoDao::createTable()
{
    return MediaPhotoDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_media_photo_xx ON media_photo ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!MediaPhotoDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool MediaPhotoDao::dropTable()
{
    return MediaPhotoDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}

MediaPhoto MediaPhotoDao::findByDownloadUrl(const QString& url)
{
	Database* db = database();
	if (!db)
		return MediaPhoto();

	QString sql(QLatin1String("SELECT "NS_MEDIA_MEDIAPHOTO_COLUMNS" FROM media_photo WHERE "));
	QString sqlw;
	SqlAnd a;
	
	(sqlw += a) += QLatin1String("download_url=?");
	
	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorateOne(sql));
	
	query.addBindValue(url);

	return MediaPhotoResult(db)(query);
}

bool MediaPhotoDao::deleteByDownloadUrl(const QString& url)
{
	static const char sql[] =
		"DELETE FROM media_photo "
		"WHERE download_url=?"
		;

	Database* db = database();
	if (!db)
		return false;

	QSqlQuery query = db->query();
	query.prepare(QLatin1String(sql));
	query.addBindValue(url);

	return db->exec(query);
}

bool MediaPhotoDao::updateByDownloadUrl(const QString& url, const MediaPhoto& newData)
{
	Database* db = database();
	if (!db)
		return false;

	QString sql(QLatin1String("UPDATE media_photo SET "));
	QString sqlv;
	SqlComma c;
	
	(sqlv += c) += QLatin1String("status=?");

	if (!newData.totalSize().isEmpty())
		(sqlv += c) += QLatin1String("total_size=?");
	if (!newData.availableSize().isEmpty())
		(sqlv += c) += QLatin1String("available_size=?");
	if (!newData.filename().isEmpty())
		(sqlv += c) += QLatin1String("fileName=?");

	if (sqlv.isEmpty())
		return true;
	sql += sqlv;
	sql += QLatin1String(" WHERE download_url=?");

	QSqlQuery query = db->query();
	query.prepare(sql);

	query.addBindValue(newData.status());

	if (!newData.totalSize().isEmpty())
		query.addBindValue(newData.totalSize());
	if (!newData.availableSize().isEmpty())
		query.addBindValue(newData.availableSize());
	if (!newData.filename().isEmpty())
		query.addBindValue(newData.filename());
	query.addBindValue(url);

	return db->exec(query);
}

//$END-CUSTOM$

} // namespace ns::Media
} // namespace ns
