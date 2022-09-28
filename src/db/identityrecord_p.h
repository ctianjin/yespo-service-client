#ifndef IDENTITYRECORD_P_H
#define IDENTITYRECORD_P_H

#include <QtCore/QSharedData>
#include <QtCore/QVariant>
#include <QtCore/QVariantHash>
#include "dbglobal.h"

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_DB_DECL IdentityRecordPrivate : public QSharedData
{
public:
    QVariant m_id;
    QVariantHash m_data;

    IdentityRecordPrivate();
    IdentityRecordPrivate(const IdentityRecordPrivate& rhs);
    explicit IdentityRecordPrivate(qlonglong id);
    explicit IdentityRecordPrivate(const QSqlQuery& query);
    explicit IdentityRecordPrivate(const QSqlRecord& record);
    ~IdentityRecordPrivate();

private:
    void init(const QSqlRecord& record);
};

} // namespace ns

#endif // IDENTITYRECORD_P_H
