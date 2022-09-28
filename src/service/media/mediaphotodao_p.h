// $Id$
/*
 * Copyright (C) 2015 RAR Limited
 * All rights reserved.
 *
 * Contact: Development Department <addison@yespo.com>
 */

// *** Generated from "media.schema" ***
// *** Do NOT edit (unless in CUSTOM areas)! ***

#ifndef NS_MEDIA_MEDIAPHOTODAO_P_H
#define NS_MEDIA_MEDIAPHOTODAO_P_H

#include "abstractentitydao.h"
#include "mediafwd.h"
#include "mediaphoto.h"
//$BEGIN-CUSTOM-INCLUDE$
//$END-CUSTOM-INCLUDE$

namespace ns {
namespace Media {

class MediaPhotoDaoBase : public AbstractEntityDao
{
public:
    typedef MediaPhoto Entity;
    typedef MediaPhotoList EntityList;

    explicit MediaPhotoDaoBase(Database* db = 0);
    virtual ~MediaPhotoDaoBase();

    virtual bool createTable();
    virtual bool dropTable();

    virtual QVariant entities();

    virtual int count(qlonglong id);
    virtual int countByExample(const MediaPhoto& entity);
    virtual int countAll();

    virtual MediaPhoto find(qlonglong id);
    virtual MediaPhoto findByExample(const MediaPhoto& entity);
    virtual MediaPhotoList findAll();
    virtual MediaPhotoList findAllByExample(const MediaPhoto& entity);

    virtual bool persist(MediaPhoto& entity);
    virtual bool merge(const MediaPhoto& entity);
    virtual bool update(const MediaPhoto& entity);
    virtual bool remove(const MediaPhoto& entity);
    virtual bool removeAll(bool resetIdentity = true);
    virtual bool refresh(MediaPhoto& entity);

private:
    Q_DISABLE_COPY(MediaPhotoDaoBase)
};

//$BEGIN-CUSTOM$
class MediaPhotoDao : public MediaPhotoDaoBase
{
public:
    explicit MediaPhotoDao(Database* db = 0);
    virtual ~MediaPhotoDao();

    virtual bool createTable();
    virtual bool dropTable();

	MediaPhoto findByDownloadUrl(const QString& url);
	bool deleteByDownloadUrl(const QString& url);
	bool updateByDownloadUrl(const QString& url, const MediaPhoto& newData);

private:
    Q_DISABLE_COPY(MediaPhotoDao)
};
//$END-CUSTOM$

} // namespace ns::Media
} // namespace ns

#endif // NS_MEDIA_MEDIAPHOTODAO_P_H
