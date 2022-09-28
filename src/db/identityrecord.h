#ifndef IDENTITYRECORD_H
#define IDENTITYRECORD_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include "dbglobal.h"

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
class QVariant;
class QSqlQuery;
class QSqlRecord;
template <typename T> class QList;
QT_END_NAMESPACE

namespace ns {

class IdentityRecord;
class IdentityRecordPrivate;
typedef QList<IdentityRecord> IdentityRecordList;

class BUILDSYS_DB_DECL IdentityRecord
{
public:
    static const QString idField;

    IdentityRecord();
    IdentityRecord(const IdentityRecord& rhs);
    explicit IdentityRecord(qlonglong id);
    explicit IdentityRecord(const QSqlQuery& query);
    explicit IdentityRecord(const QSqlRecord& record);
    ~IdentityRecord();

    IdentityRecord& operator=(const IdentityRecord& rhs);

    bool isValid() const;
    IdentityRecord identity() const;
    bool identityEqual(const IdentityRecord& rhs) const;

    bool isIdNull() const;
    qlonglong id() const;
    void setIdNull();
    void setId(qlonglong id);

    QStringList fields() const;

    bool isFieldNull(const QString& field) const;
    void setFieldNull(const QString& field);

    QVariant fieldValue(const QString& field) const;
    void setFieldValue(const QString& field, const QVariant& value);

private:
    friend class GenericIdentityDao;
    friend class GenericIdentityDaoBase;

    QSharedDataPointer<IdentityRecordPrivate> d;
};

} // namespace ns

Q_DECLARE_METATYPE(ns::IdentityRecord)
Q_DECLARE_METATYPE(ns::IdentityRecordList)

#endif // IDENTITYRECORD_H
