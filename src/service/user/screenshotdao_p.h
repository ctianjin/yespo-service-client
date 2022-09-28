// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "user.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_USER_SCREENSHOTDAO_P_H
#define NS_USER_SCREENSHOTDAO_P_H

#include "abstractentitydao.h"
#include "userfwd.h"
#include "screenshot.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace User {

class ScreenshotDaoBase : public AbstractEntityDao
{
public:
    typedef Screenshot Entity;
    typedef ScreenshotList EntityList;

    explicit ScreenshotDaoBase(Database* db = 0);
    virtual ~ScreenshotDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(qlonglong id);
    virtual int countByExample(const Screenshot& entity);
    virtual int countAll();

    virtual Screenshot find(qlonglong id);
    virtual Screenshot findByExample(const Screenshot& entity);
    virtual ScreenshotList findAll();
    virtual ScreenshotList findAllByExample(const Screenshot& entity);

    virtual bool persist(Screenshot& entity);
    virtual bool merge(const Screenshot& entity);
    virtual bool update(const Screenshot& entity);
    virtual bool remove(const Screenshot& entity);
    virtual bool removeAll(bool resetIdentity = true);
    virtual bool refresh(Screenshot& entity);

private:
    Q_DISABLE_COPY(ScreenshotDaoBase)
};

//$BEGIN-CUSTOM$
class ScreenshotDao : public ScreenshotDaoBase
{
public:
    explicit ScreenshotDao(Database* db = 0);
    virtual ~ScreenshotDao();

    virtual bool createTable();
    virtual bool dropTable();

	bool updateMeetCDRIdByOwner(const QString& owner, const QString& Id);
	ScreenshotList findIdleByOwner(const QString& owner);
	bool updateUploadStatus(const QString& owner, const QString& Id, UploadStatus status);
	bool updateUploadStatusByPhoto(const QString& owner, const QString& Id, const QString& photo, UploadStatus status);

private:
    Q_DISABLE_COPY(ScreenshotDao)
};
//$END-CUSTOM$

} // namespace ns::User
} // namespace ns

#endif // NS_USER_SCREENSHOTDAO_P_H
