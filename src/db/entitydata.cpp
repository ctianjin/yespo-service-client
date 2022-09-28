#include <QtCore/QHash>
#include <QtCore/QSharedData>
#include "entitydata.h"

namespace ns {

class EntityDataPrivate : public QSharedData
{
public:
    QHash<int, QVariant> m_data;

    EntityDataPrivate();
    EntityDataPrivate(const EntityDataPrivate& rhs);
    ~EntityDataPrivate();
};

EntityDataPrivate::EntityDataPrivate() :
    QSharedData()
{
}

EntityDataPrivate::EntityDataPrivate(const EntityDataPrivate& rhs) :
    QSharedData(rhs),
    m_data(rhs.m_data)
{
}

EntityDataPrivate::~EntityDataPrivate()
{
}

EntityData::EntityData() :
    d(new EntityDataPrivate)
{
}

EntityData::EntityData(const EntityData& rhs) :
    d(rhs.d)
{
}

EntityData::~EntityData()
{
}

EntityData& EntityData::operator=(const EntityData& rhs)
{
    d = rhs.d;
    return *this;
}

QVariant EntityData::value(int typeId) const
{
    return d->m_data.value(typeId);
}

void EntityData::setValue(int typeId, const QVariant& value)
{
    if (value.isValid()) {
        d->m_data.insert(typeId, value);
    } else {
        d->m_data.remove(typeId);
    }
}

void EntityData::clear()
{
    d->m_data.clear();
}

} // namespace ns
