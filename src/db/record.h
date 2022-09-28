#ifndef RECORD_H
#define RECORD_H

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

class Record;
class RecordPrivate;
typedef QList<Record> RecordList;

class BUILDSYS_DB_DECL Record
{
public:
    Record();
    Record(const Record& rhs);
    explicit Record(const QSqlQuery& query);
    explicit Record(const QSqlRecord& record);
    ~Record();

    Record& operator=(const Record& rhs);

    QStringList fields() const;

    bool isFieldNull(const QString& field) const;
    void setFieldNull(const QString& field);

    QVariant fieldValue(const QString& field) const;
    void setFieldValue(const QString& field, const QVariant& value);

private:
    friend class GenericDao;
    friend class GenericDaoBase;

    QSharedDataPointer<RecordPrivate> d;
};

} // namespace ns

Q_DECLARE_METATYPE(ns::Record)
Q_DECLARE_METATYPE(ns::RecordList)

#endif // RECORD_H
