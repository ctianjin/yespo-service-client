#ifndef RECORD_P_H
#define RECORD_P_H

#include <QtCore/QSharedData>
#include <QtCore/QVariantHash>
#include "dbglobal.h"

QT_BEGIN_NAMESPACE
class QSqlQuery;
class QSqlRecord;
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_DB_DECL RecordPrivate : public QSharedData
{
public:
    QVariantHash m_data;

    RecordPrivate();
    RecordPrivate(const RecordPrivate& rhs);
    explicit RecordPrivate(const QSqlQuery& query);
    explicit RecordPrivate(const QSqlRecord& record);
    ~RecordPrivate();

private:
    void init(const QSqlRecord& record);
};

} // namespace ns

#endif // RECORD_P_H
