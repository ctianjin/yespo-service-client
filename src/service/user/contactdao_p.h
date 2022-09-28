// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_CONTACTDAO_P_H
#define NS_USER_CONTACTDAO_P_H

#include "abstractentitydao.h"
#include "userfwd.h"
#include "contact.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

class ContactDaoBase : public AbstractEntityDao
{
public:
    typedef Contact Entity;
    typedef ContactList EntityList;

    explicit ContactDaoBase(Database* db = 0);
    virtual ~ContactDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(qlonglong id);
    virtual int countByExample(const Contact& entity);
    virtual int countAll();

    virtual Contact find(qlonglong id);
    virtual Contact findByExample(const Contact& entity);
    virtual ContactList findAll();
    virtual ContactList findAllByExample(const Contact& entity);

    virtual bool persist(Contact& entity);
    virtual bool merge(const Contact& entity);
    virtual bool update(const Contact& entity);
    virtual bool remove(const Contact& entity);
    virtual bool removeAll(bool resetIdentity = true);
    virtual bool refresh(Contact& entity);

private:
    Q_DISABLE_COPY(ContactDaoBase)
};

//$BEGIN-CUSTOM$
class ContactDao : public ContactDaoBase
{
public:
    explicit ContactDao(Database* db = 0);
    virtual ~ContactDao();

    virtual bool createTable();
    virtual bool dropTable();

	Contact findByUserNameAndAddBookID(const QString& userName, const QString& ID);
	Contact findByUserNameAndSmallPhoto(const QString& userName, const QString& link);
	Contact findByUserNameAndSIPCallerID(const QString& userName, const QString& number);
	ContactList findByUserName(const QString& userName);
	bool removeByUserNameAndAddBookID(const QString& userName, const QString& ID);
	Contact findByUserNameAndVoiceraID(const QString& userName, const QString& ID);
	bool removeByUserName(const QString& userName);
	bool updateByUserName(const QString& userName, const Contact& contact);

private:
    Q_DISABLE_COPY(ContactDao)
};
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns

#endif // NS_USER_CONTACTDAO_P_H
