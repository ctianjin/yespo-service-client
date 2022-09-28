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
#include "contactdao_p.h"
#include "contact_p.h"
#include "user.h"
#include "database.h"
#include "queryresult.h"
#include "transaction.h"
#include "stringseparator.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

#ifndef NS_USER_CONTACT_COLUMNS
#  define NS_USER_CONTACT_COLUMNS "id, user_name, cid, add_book_id, contact_name, small_photo, large_photo, phone, voicera_id, sip_caller_id, source_language, target_language, online_status, system_user, photoFile, total_size, available_size"
#endif

#ifndef NS_USER_CONTACT_COLUMNS_PREFIXED
#  define NS_USER_CONTACT_COLUMNS_PREFIXED "contact.id, contact.user_name, contact.cid, contact.add_book_id, contact.contact_name, contact.small_photo, contact.large_photo, contact.phone, contact.voicera_id, contact.sip_caller_id, contact.source_language, contact.target_language, contact.online_status, contact.system_user, contact.photoFile, contact.total_size, contact.available_size"
#endif

namespace ns {
namespace User {

typedef QueryResult<Contact> ContactResult;
typedef QueryResult<Contact, QueryResultCreatorByRecord<Contact> > ContactResultByRecord;
typedef QueryResults<Contact> ContactResults;
typedef QueryResults<Contact, QueryResultCreatorByRecord<Contact> > ContactResultsByRecord;

namespace {

BUILDSYS_DECLARE_STRING_SEPARATOR(SqlComma, ", ")
BUILDSYS_DECLARE_STRING_SEPARATOR(SqlAnd, " AND ")

} // anonymous namespace

ContactDaoBase::ContactDaoBase(Database* db) :
    AbstractEntityDao(db, QLatin1String("contact"))
{
}

ContactDaoBase::~ContactDaoBase()
{
}

bool ContactDaoBase::createTable()
{
    static const char sql[] =
        "CREATE TABLE contact ( "
          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "user_name TEXT NOT NULL, "
          "cid TEXT, "
          "add_book_id TEXT, "
          "contact_name TEXT, "
          "small_photo TEXT, "
          "large_photo TEXT, "
          "phone TEXT, "
          "voicera_id TEXT, "
          "sip_caller_id TEXT, "
          "source_language TEXT, "
          "target_language TEXT, "
          "online_status TEXT, "
          "system_user TEXT, "
          "photoFile TEXT, "
          "total_size TEXT, "
          "available_size TEXT "
        ")"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

bool ContactDaoBase::dropTable()
{
    static const char sql[] =
        "DROP TABLE contact"
        ;

    Database* db = database();
    if (!db)
        return false;

    return db->exec(QLatin1String(sql));
}

QVariant ContactDaoBase::entities()
{
    return QVariant::fromValue(findAll());
}

int ContactDaoBase::count(qlonglong id)
{
    static const char sql[] =
        "SELECT COUNT(*) FROM contact "
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

int ContactDaoBase::countByExample(const Contact& entity)
{
    Database* db = database();
    if (!db)
        return -1;

    const ContactPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT COUNT(*) FROM contact WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_cid.isValid())
        (sqlw += a) += QLatin1String("cid=?");
    if (e->m_addBookId.isValid())
        (sqlw += a) += QLatin1String("add_book_id=?");
    if (e->m_contactName.isValid())
        (sqlw += a) += QLatin1String("contact_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");
    if (e->m_largePhoto.isValid())
        (sqlw += a) += QLatin1String("large_photo=?");
    if (e->m_phone.isValid())
        (sqlw += a) += QLatin1String("phone=?");
    if (e->m_voiceraId.isValid())
        (sqlw += a) += QLatin1String("voicera_id=?");
    if (e->m_sipCallerId.isValid())
        (sqlw += a) += QLatin1String("sip_caller_id=?");
    if (e->m_sourceLanguage.isValid())
        (sqlw += a) += QLatin1String("source_language=?");
    if (e->m_targetLanguage.isValid())
        (sqlw += a) += QLatin1String("target_language=?");
    if (e->m_onlineStatus.isValid())
        (sqlw += a) += QLatin1String("online_status=?");
    if (e->m_systemUser.isValid())
        (sqlw += a) += QLatin1String("system_user=?");
    if (e->m_photofile.isValid())
        (sqlw += a) += QLatin1String("photoFile=?");
    if (e->m_totalSize.isValid())
        (sqlw += a) += QLatin1String("total_size=?");
    if (e->m_availableSize.isValid())
        (sqlw += a) += QLatin1String("available_size=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_addBookId.isValid())
        query.addBindValue(e->m_addBookId);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_phone.isValid())
        query.addBindValue(e->m_phone);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_sipCallerId.isValid())
        query.addBindValue(e->m_sipCallerId);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_onlineStatus.isValid())
        query.addBindValue(e->m_onlineStatus);
    if (e->m_systemUser.isValid())
        query.addBindValue(e->m_systemUser);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);

    const QVariant v = QueryValue<QVariant>(db)(query);
    return v.isValid() ? v.toInt() : -1;
}

int ContactDaoBase::countAll()
{
    static const char sql[] =
        "SELECT COUNT(*) FROM contact"
        ;

    Database* db = database();
    if (!db)
        return -1;

    const QVariant v = QueryValue<QVariant>(db)(QLatin1String(sql));
    return v.isValid() ? v.toInt() : -1;
}

Contact ContactDaoBase::find(qlonglong id)
{
    static const char sql[] =
        "SELECT "NS_USER_CONTACT_COLUMNS" FROM contact "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return Contact();

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(id);

    return ContactResult(db)(query);
}

Contact ContactDaoBase::findByExample(const Contact& entity)
{
    Database* db = database();
    if (!db)
        return Contact();

    const ContactPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_CONTACT_COLUMNS" FROM contact WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_cid.isValid())
        (sqlw += a) += QLatin1String("cid=?");
    if (e->m_addBookId.isValid())
        (sqlw += a) += QLatin1String("add_book_id=?");
    if (e->m_contactName.isValid())
        (sqlw += a) += QLatin1String("contact_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");
    if (e->m_largePhoto.isValid())
        (sqlw += a) += QLatin1String("large_photo=?");
    if (e->m_phone.isValid())
        (sqlw += a) += QLatin1String("phone=?");
    if (e->m_voiceraId.isValid())
        (sqlw += a) += QLatin1String("voicera_id=?");
    if (e->m_sipCallerId.isValid())
        (sqlw += a) += QLatin1String("sip_caller_id=?");
    if (e->m_sourceLanguage.isValid())
        (sqlw += a) += QLatin1String("source_language=?");
    if (e->m_targetLanguage.isValid())
        (sqlw += a) += QLatin1String("target_language=?");
    if (e->m_onlineStatus.isValid())
        (sqlw += a) += QLatin1String("online_status=?");
    if (e->m_systemUser.isValid())
        (sqlw += a) += QLatin1String("system_user=?");
    if (e->m_photofile.isValid())
        (sqlw += a) += QLatin1String("photoFile=?");
    if (e->m_totalSize.isValid())
        (sqlw += a) += QLatin1String("total_size=?");
    if (e->m_availableSize.isValid())
        (sqlw += a) += QLatin1String("available_size=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorateOne(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_addBookId.isValid())
        query.addBindValue(e->m_addBookId);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_phone.isValid())
        query.addBindValue(e->m_phone);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_sipCallerId.isValid())
        query.addBindValue(e->m_sipCallerId);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_onlineStatus.isValid())
        query.addBindValue(e->m_onlineStatus);
    if (e->m_systemUser.isValid())
        query.addBindValue(e->m_systemUser);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);

    return ContactResult(db)(query);
}

ContactList ContactDaoBase::findAll()
{
    static const char sql[] =
        "SELECT "NS_USER_CONTACT_COLUMNS" FROM contact"
        ;

    Database* db = database();
    if (!db)
        return ContactList();

    return ContactResults(db)(sqlDecorate(sql));
}

ContactList ContactDaoBase::findAllByExample(const Contact& entity)
{
    Database* db = database();
    if (!db)
        return ContactList();

    const ContactPrivate* e = entity.d;

    QString sql(QLatin1String("SELECT "NS_USER_CONTACT_COLUMNS" FROM contact WHERE "));
    QString sqlw;
    SqlAnd a;
    if (e->m_id.isValid())
        (sqlw += a) += QLatin1String("id=?");
    if (e->m_userName.isValid())
        (sqlw += a) += QLatin1String("user_name=?");
    if (e->m_cid.isValid())
        (sqlw += a) += QLatin1String("cid=?");
    if (e->m_addBookId.isValid())
        (sqlw += a) += QLatin1String("add_book_id=?");
    if (e->m_contactName.isValid())
        (sqlw += a) += QLatin1String("contact_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlw += a) += QLatin1String("small_photo=?");
    if (e->m_largePhoto.isValid())
        (sqlw += a) += QLatin1String("large_photo=?");
    if (e->m_phone.isValid())
        (sqlw += a) += QLatin1String("phone=?");
    if (e->m_voiceraId.isValid())
        (sqlw += a) += QLatin1String("voicera_id=?");
    if (e->m_sipCallerId.isValid())
        (sqlw += a) += QLatin1String("sip_caller_id=?");
    if (e->m_sourceLanguage.isValid())
        (sqlw += a) += QLatin1String("source_language=?");
    if (e->m_targetLanguage.isValid())
        (sqlw += a) += QLatin1String("target_language=?");
    if (e->m_onlineStatus.isValid())
        (sqlw += a) += QLatin1String("online_status=?");
    if (e->m_systemUser.isValid())
        (sqlw += a) += QLatin1String("system_user=?");
    if (e->m_photofile.isValid())
        (sqlw += a) += QLatin1String("photoFile=?");
    if (e->m_totalSize.isValid())
        (sqlw += a) += QLatin1String("total_size=?");
    if (e->m_availableSize.isValid())
        (sqlw += a) += QLatin1String("available_size=?");

    sql += sqlw;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(sqlDecorate(sql));
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_addBookId.isValid())
        query.addBindValue(e->m_addBookId);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_phone.isValid())
        query.addBindValue(e->m_phone);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_sipCallerId.isValid())
        query.addBindValue(e->m_sipCallerId);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_onlineStatus.isValid())
        query.addBindValue(e->m_onlineStatus);
    if (e->m_systemUser.isValid())
        query.addBindValue(e->m_systemUser);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);

    return ContactResults(db)(query);
}

bool ContactDaoBase::persist(Contact& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const ContactPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT INTO contact ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_userName.isValid()) {
        (sql += c) += QLatin1String("user_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_cid.isValid()) {
        (sql += c) += QLatin1String("cid");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_addBookId.isValid()) {
        (sql += c) += QLatin1String("add_book_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_contactName.isValid()) {
        (sql += c) += QLatin1String("contact_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_smallPhoto.isValid()) {
        (sql += c) += QLatin1String("small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_largePhoto.isValid()) {
        (sql += c) += QLatin1String("large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_phone.isValid()) {
        (sql += c) += QLatin1String("phone");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_voiceraId.isValid()) {
        (sql += c) += QLatin1String("voicera_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_sipCallerId.isValid()) {
        (sql += c) += QLatin1String("sip_caller_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_sourceLanguage.isValid()) {
        (sql += c) += QLatin1String("source_language");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_targetLanguage.isValid()) {
        (sql += c) += QLatin1String("target_language");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_onlineStatus.isValid()) {
        (sql += c) += QLatin1String("online_status");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_systemUser.isValid()) {
        (sql += c) += QLatin1String("system_user");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_photofile.isValid()) {
        (sql += c) += QLatin1String("photoFile");
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
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_addBookId.isValid())
        query.addBindValue(e->m_addBookId);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_phone.isValid())
        query.addBindValue(e->m_phone);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_sipCallerId.isValid())
        query.addBindValue(e->m_sipCallerId);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_onlineStatus.isValid())
        query.addBindValue(e->m_onlineStatus);
    if (e->m_systemUser.isValid())
        query.addBindValue(e->m_systemUser);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);

    if (db->exec(query)) {
        const QVariant id = query.lastInsertId();
        entity.d->m_id = id;
        return true;
    } else {
        return false;
    }
}

bool ContactDaoBase::merge(const Contact& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const ContactPrivate* e = entity.d;

    QString sql(QLatin1String("INSERT OR REPLACE INTO contact ("));
    QString sqlv(QLatin1String("VALUES ("));
    SqlComma c, cv;
    if (e->m_id.isValid()) {
        (sql += c) += QLatin1String("id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_userName.isValid()) {
        (sql += c) += QLatin1String("user_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_cid.isValid()) {
        (sql += c) += QLatin1String("cid");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_addBookId.isValid()) {
        (sql += c) += QLatin1String("add_book_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_contactName.isValid()) {
        (sql += c) += QLatin1String("contact_name");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_smallPhoto.isValid()) {
        (sql += c) += QLatin1String("small_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_largePhoto.isValid()) {
        (sql += c) += QLatin1String("large_photo");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_phone.isValid()) {
        (sql += c) += QLatin1String("phone");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_voiceraId.isValid()) {
        (sql += c) += QLatin1String("voicera_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_sipCallerId.isValid()) {
        (sql += c) += QLatin1String("sip_caller_id");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_sourceLanguage.isValid()) {
        (sql += c) += QLatin1String("source_language");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_targetLanguage.isValid()) {
        (sql += c) += QLatin1String("target_language");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_onlineStatus.isValid()) {
        (sql += c) += QLatin1String("online_status");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_systemUser.isValid()) {
        (sql += c) += QLatin1String("system_user");
        (sqlv += cv) += QLatin1String("?");
    }
    if (e->m_photofile.isValid()) {
        (sql += c) += QLatin1String("photoFile");
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
    sql += QLatin1String(") ");
    sqlv += QLatin1String(")");
    sql += sqlv;

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_id.isValid())
        query.addBindValue(e->m_id);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_addBookId.isValid())
        query.addBindValue(e->m_addBookId);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_phone.isValid())
        query.addBindValue(e->m_phone);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_sipCallerId.isValid())
        query.addBindValue(e->m_sipCallerId);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_onlineStatus.isValid())
        query.addBindValue(e->m_onlineStatus);
    if (e->m_systemUser.isValid())
        query.addBindValue(e->m_systemUser);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);

    return db->exec(query);
}

bool ContactDaoBase::update(const Contact& entity)
{
    Database* db = database();
    if (!db)
        return false;

    const ContactPrivate* e = entity.d;

    QString sql(QLatin1String("UPDATE contact SET "));
    QString sqlv;
    SqlComma c;
    if (e->m_userName.isValid())
        (sqlv += c) += QLatin1String("user_name=?");
    if (e->m_cid.isValid())
        (sqlv += c) += QLatin1String("cid=?");
    if (e->m_addBookId.isValid())
        (sqlv += c) += QLatin1String("add_book_id=?");
    if (e->m_contactName.isValid())
        (sqlv += c) += QLatin1String("contact_name=?");
    if (e->m_smallPhoto.isValid())
        (sqlv += c) += QLatin1String("small_photo=?");
    if (e->m_largePhoto.isValid())
        (sqlv += c) += QLatin1String("large_photo=?");
    if (e->m_phone.isValid())
        (sqlv += c) += QLatin1String("phone=?");
    if (e->m_voiceraId.isValid())
        (sqlv += c) += QLatin1String("voicera_id=?");
    if (e->m_sipCallerId.isValid())
        (sqlv += c) += QLatin1String("sip_caller_id=?");
    if (e->m_sourceLanguage.isValid())
        (sqlv += c) += QLatin1String("source_language=?");
    if (e->m_targetLanguage.isValid())
        (sqlv += c) += QLatin1String("target_language=?");
    if (e->m_onlineStatus.isValid())
        (sqlv += c) += QLatin1String("online_status=?");
    if (e->m_systemUser.isValid())
        (sqlv += c) += QLatin1String("system_user=?");
    if (e->m_photofile.isValid())
        (sqlv += c) += QLatin1String("photoFile=?");
    if (e->m_totalSize.isValid())
        (sqlv += c) += QLatin1String("total_size=?");
    if (e->m_availableSize.isValid())
        (sqlv += c) += QLatin1String("available_size=?");

    if (sqlv.isEmpty())
        return true;
    sql += sqlv;
    sql += QLatin1String(" WHERE id=?");

    QSqlQuery query = db->query();
    query.prepare(sql);
    if (e->m_userName.isValid())
        query.addBindValue(e->m_userName);
    if (e->m_cid.isValid())
        query.addBindValue(e->m_cid);
    if (e->m_addBookId.isValid())
        query.addBindValue(e->m_addBookId);
    if (e->m_contactName.isValid())
        query.addBindValue(e->m_contactName);
    if (e->m_smallPhoto.isValid())
        query.addBindValue(e->m_smallPhoto);
    if (e->m_largePhoto.isValid())
        query.addBindValue(e->m_largePhoto);
    if (e->m_phone.isValid())
        query.addBindValue(e->m_phone);
    if (e->m_voiceraId.isValid())
        query.addBindValue(e->m_voiceraId);
    if (e->m_sipCallerId.isValid())
        query.addBindValue(e->m_sipCallerId);
    if (e->m_sourceLanguage.isValid())
        query.addBindValue(e->m_sourceLanguage);
    if (e->m_targetLanguage.isValid())
        query.addBindValue(e->m_targetLanguage);
    if (e->m_onlineStatus.isValid())
        query.addBindValue(e->m_onlineStatus);
    if (e->m_systemUser.isValid())
        query.addBindValue(e->m_systemUser);
    if (e->m_photofile.isValid())
        query.addBindValue(e->m_photofile);
    if (e->m_totalSize.isValid())
        query.addBindValue(e->m_totalSize);
    if (e->m_availableSize.isValid())
        query.addBindValue(e->m_availableSize);
    query.addBindValue(e->m_id);

    return db->exec(query);
}

bool ContactDaoBase::remove(const Contact& entity)
{
    static const char sql[] =
        "DELETE FROM contact "
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

bool ContactDaoBase::removeAll(bool resetIdentity)
{
    static const char sql[] =
        "DELETE FROM contact"
        ;
    static const char sqli[] =
        "DELETE FROM sqlite_sequence WHERE name='contact'"
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

bool ContactDaoBase::refresh(Contact& entity)
{
    static const char sql[] =
        "SELECT "NS_USER_CONTACT_COLUMNS" FROM contact "
         "WHERE id=?"
        ;

    Database* db = database();
    if (!db)
        return false;

    if (!entity.isValid())
        return false;

    const ContactPrivate* e = entity.d;

    QSqlQuery query = db->query();
    query.setForwardOnly(true);
    query.prepare(QLatin1String(sql));
    query.addBindValue(e->m_id);

    if (!db->exec(query))
        return false;
    if (!query.next())
        return false;

    entity = Contact(query);
    return true;
}

//$BEGIN-CUSTOM$
ContactDao::ContactDao(Database* db) :
    ContactDaoBase(db)
{
}

ContactDao::~ContactDao()
{
}

bool ContactDao::createTable()
{
    return ContactDaoBase::createTable();

    // Customize createTable here
    /*
    static const char sql[] =
        "CREATE INDEX idx_contact_xx ON contact ( xx )"
        ;

    Database* db = database();
    if (!db)
        return false;

    Transaction trans(db);

    if (!ContactDaoBase::createTable())
        return false;
    if (!db->exec(QLatin1String(sql)))
        return false;

    return trans.commit();
    */
}

bool ContactDao::dropTable()
{
    return ContactDaoBase::dropTable();

    // Customize dropTable here
    /*
    Database* db = database();
    if (!db)
        return false;
    */
}

Contact ContactDao::findByUserNameAndAddBookID(const QString& userName, const QString& ID)
{
	Database* db = database();
	if (!db || userName.isEmpty() || ID.isEmpty())
		return Contact();

	QString sql(QLatin1String("SELECT "NS_USER_CONTACT_COLUMNS" FROM contact WHERE "));
	QString sqlw;
	SqlAnd a;
	
	(sqlw += a) += QLatin1String("user_name=?");
	(sqlw += a) += QLatin1String("add_book_id=?");
	
	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorateOne(sql));
	
	query.addBindValue(userName);
	query.addBindValue(ID);
	
	return ContactResult(db)(query);
}

Contact ContactDao::findByUserNameAndSmallPhoto(const QString& userName, const QString& link)
{
	Database* db = database();
	if (!db || userName.isEmpty() || link.isEmpty())
		return Contact();

	QString sql(QLatin1String("SELECT "NS_USER_CONTACT_COLUMNS" FROM contact WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("user_name=?");
	(sqlw += a) += QLatin1String("small_photo=?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorateOne(sql));

	query.addBindValue(userName);
	query.addBindValue(link);

	return ContactResult(db)(query);
}

Contact ContactDao::findByUserNameAndSIPCallerID(const QString& userName, const QString& number)
{
	Database* db = database();
	if (!db || userName.isEmpty() || number.isEmpty())
		return Contact();

	QString sql(QLatin1String("SELECT "NS_USER_CONTACT_COLUMNS" FROM contact WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("user_name=?");
	(sqlw += a) += QLatin1String("sip_caller_id=?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorateOne(sql));

	query.addBindValue(userName);
	query.addBindValue(number);

	return ContactResult(db)(query);
}

ContactList ContactDao::findByUserName(const QString& userName)
{
	Database* db = database();
	if (!db)
		return ContactList();


	QString sql(QLatin1String("SELECT "NS_USER_CONTACT_COLUMNS" FROM contact WHERE "));
	QString sqlw;
	SqlAnd a;
	
	(sqlw += a) += QLatin1String("user_name=?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorate(sql));
	
	query.addBindValue(userName);
	
	return ContactResults(db)(query);
}

bool ContactDao::removeByUserNameAndAddBookID(const QString& userName, const QString& ID)
{
	static const char sql[] =
		"DELETE FROM contact "
		"WHERE user_name=? AND add_book_id=?"
		;

	Database* db = database();
	if (!db || userName.isEmpty() || ID.isEmpty())
		return false;

	QSqlQuery query = db->query();
	query.prepare(QLatin1String(sql));
	query.addBindValue(userName);
	query.addBindValue(ID);

	return db->exec(query);
}

Contact ContactDao::findByUserNameAndVoiceraID(const QString& userName, const QString& ID)
{
	Database* db = database();
	if (!db || userName.isEmpty() || ID.isEmpty())
		return Contact();

	QString sql(QLatin1String("SELECT "NS_USER_CONTACT_COLUMNS" FROM contact WHERE "));
	QString sqlw;
	SqlAnd a;

	(sqlw += a) += QLatin1String("user_name=?");
	(sqlw += a) += QLatin1String("voicera_id=?");

	sql += sqlw;

	QSqlQuery query = db->query();
	query.setForwardOnly(true);
	query.prepare(sqlDecorateOne(sql));

	query.addBindValue(userName);
	query.addBindValue(ID);

	return ContactResult(db)(query);
}

bool ContactDao::removeByUserName(const QString& userName)
{
	static const char sql[] =
		"DELETE FROM contact "
		"WHERE user_name=?"
		;

	Database* db = database();
	if (!db || userName.isEmpty())
		return false;

	QSqlQuery query = db->query();
	query.prepare(QLatin1String(sql));
	query.addBindValue(userName);

	return db->exec(query);
}

bool ContactDao::updateByUserName(const QString& userName, const Contact& contact)
{
	Database* db = database();
	if (!db)
		return false;

	QString sql(QLatin1String("UPDATE contact SET "));
	QString sqlv;
	SqlComma c;
	(sqlv += c) += QLatin1String("phone=?");
	(sqlv += c) += QLatin1String("source_language=?");
	(sqlv += c) += QLatin1String("target_language=?");

	if (sqlv.isEmpty())
		return true;
	sql += sqlv;
	sql += QLatin1String(" WHERE user_name=? AND add_book_id=?");

	QSqlQuery query = db->query();
	query.prepare(sql);

	query.addBindValue(contact.phone());
	query.addBindValue(contact.sourceLanguage());
	query.addBindValue(contact.targetLanguage());
	query.addBindValue(userName);
	query.addBindValue(contact.addBookId());

	return db->exec(query);
}

//$END-CUSTOM$

} // namespace ns::User
} // namespace ns
