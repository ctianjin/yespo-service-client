#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "record.h"
#include "record_p.h"

namespace ns {

// FIXME Use upper case of all fields currently

RecordPrivate::RecordPrivate() :
    QSharedData()
{
}

RecordPrivate::RecordPrivate(const RecordPrivate& rhs) :
    QSharedData(rhs),
    m_data(rhs.m_data)
{
}

RecordPrivate::RecordPrivate(const QSqlQuery& query) :
    QSharedData()
{
    init(query.record());
}

RecordPrivate::RecordPrivate(const QSqlRecord& record) :
    QSharedData()
{
    init(record);
}

RecordPrivate::~RecordPrivate()
{
}

void RecordPrivate::init(const QSqlRecord& record)
{
    const int count = record.count();
    for (int i = 0; i < count; ++i) {
        const QSqlField field = record.field(i);
        m_data.insert(field.name().toUpper(), field.value());
    }
}

Record::Record() :
    d(new RecordPrivate)
{
}

Record::Record(const Record& rhs) :
    d(rhs.d)
{
}

Record::Record(const QSqlQuery& query) :
    d(new RecordPrivate(query))
{
}

Record::Record(const QSqlRecord& record) :
    d(new RecordPrivate(record))
{
}

Record::~Record()
{
}

Record& Record::operator=(const Record& rhs)
{
    d = rhs.d;
    return *this;
}

QStringList Record::fields() const
{
    return d->m_data.keys();
}

bool Record::isFieldNull(const QString& field) const
{
    QVariantHash::const_iterator iter = d->m_data.constFind(field.toUpper());
    if (iter == d->m_data.constEnd())
        return true;

    return iter.value().isNull();
}

void Record::setFieldNull(const QString& field)
{
    d->m_data.remove(field.toUpper());
}

QVariant Record::fieldValue(const QString& field) const
{
    return d->m_data.value(field.toUpper());
}

void Record::setFieldValue(const QString& field, const QVariant& value)
{
    d->m_data.insert(field.toUpper(), value);
}

} // namespace ns
