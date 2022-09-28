#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtSql/QSqlField>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include "identityrecord.h"
#include "identityrecord_p.h"

namespace ns {

// FIXME Use upper case of all fields currently

const QString IdentityRecord::idField = QLatin1String("id");

IdentityRecordPrivate::IdentityRecordPrivate() :
    QSharedData()
{
}

IdentityRecordPrivate::IdentityRecordPrivate(const IdentityRecordPrivate& rhs) :
    QSharedData(rhs),
    m_id(rhs.m_id),
    m_data(rhs.m_data)
{
}

IdentityRecordPrivate::IdentityRecordPrivate(qlonglong id) :
    QSharedData(),
    m_id(id)
{
}

IdentityRecordPrivate::IdentityRecordPrivate(const QSqlQuery& query) :
    QSharedData()
{
    init(query.record());
}

IdentityRecordPrivate::IdentityRecordPrivate(const QSqlRecord& record) :
    QSharedData()
{
    init(record);
}

IdentityRecordPrivate::~IdentityRecordPrivate()
{
}

void IdentityRecordPrivate::init(const QSqlRecord& record)
{
    const int count = record.count();
    for (int i = 0; i < count; ++i) {
        const QSqlField field = record.field(i);
        m_data.insert(field.name().toUpper(), field.value());
    }

    QVariantHash::iterator iter = m_data.find(IdentityRecord::idField.toUpper());
    if (iter != m_data.end()) {
        m_id = iter.value();
        m_data.erase(iter);
    }
}

IdentityRecord::IdentityRecord() :
    d(new IdentityRecordPrivate)
{
}

IdentityRecord::IdentityRecord(const IdentityRecord& rhs) :
    d(rhs.d)
{
}

IdentityRecord::IdentityRecord(qlonglong id) :
    d(new IdentityRecordPrivate(id))
{
}

IdentityRecord::IdentityRecord(const QSqlQuery& query) :
    d(new IdentityRecordPrivate(query))
{
}

IdentityRecord::IdentityRecord(const QSqlRecord& record) :
    d(new IdentityRecordPrivate(record))
{
}

IdentityRecord::~IdentityRecord()
{
}

IdentityRecord& IdentityRecord::operator=(const IdentityRecord& rhs)
{
    d = rhs.d;
    return *this;
}

bool IdentityRecord::isValid() const
{
    return d->m_id.isValid();
}

IdentityRecord IdentityRecord::identity() const
{
    IdentityRecord record;
    record.d->m_id = d->m_id;
    return record;
}

bool IdentityRecord::identityEqual(const IdentityRecord& rhs) const
{
    return d->m_id == rhs.d->m_id;
}

bool IdentityRecord::isIdNull() const
{
    return d->m_id.isNull();
}

qlonglong IdentityRecord::id() const
{
    return d->m_id.toLongLong();
}

void IdentityRecord::setIdNull()
{
    d->m_id = QVariant(QVariant::LongLong);
}

void IdentityRecord::setId(qlonglong id)
{
    d->m_id = id;
}

QStringList IdentityRecord::fields() const
{
    return d->m_data.keys();
}

bool IdentityRecord::isFieldNull(const QString& field) const
{
    QVariantHash::const_iterator iter = d->m_data.constFind(field.toUpper());
    if (iter == d->m_data.constEnd())
        return true;

    return iter.value().isNull();
}

void IdentityRecord::setFieldNull(const QString& field)
{
    d->m_data.remove(field.toUpper());
}

QVariant IdentityRecord::fieldValue(const QString& field) const
{
    const QString upper = field.toUpper();
    if (upper == IdentityRecord::idField.toUpper())
        return d->m_id;
    return d->m_data.value(upper);
}

void IdentityRecord::setFieldValue(const QString& field, const QVariant& value)
{
    d->m_data.insert(field.toUpper(), value);
}

} // namespace ns
