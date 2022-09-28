#ifndef ENTITYDATA_H
#define ENTITYDATA_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QVariant>
#include "dbglobal.h"

namespace ns {

class EntityDataPrivate;

class BUILDSYS_DB_DECL EntityData
{
public:
    EntityData();
    EntityData(const EntityData& rhs);
    ~EntityData();

    EntityData& operator=(const EntityData& rhs);

    bool isEmpty() const;
    void clear();

    QVariant value(int typeId) const;
    void setValue(int typeId, const QVariant& value);

    template <typename T>
    T value(int typeId) const;

    template <typename T>
    T value() const;

    template <typename T>
    void setValue(int typeId, const T& value);

    template <typename T>
    void setValue(const T& value);

private:
    QSharedDataPointer<EntityDataPrivate> d;
};

template <typename T>
inline T EntityData::value(int typeId) const
{
    QVariant v = value(typeId);
    return v.value<T>();
}

template <typename T>
inline T EntityData::value() const
{
    return value<T>(qMetaTypeId<T>());
}

template <typename T>
inline void EntityData::setValue(int typeId, const T& value)
{
    setValue(typeId, QVariant::fromValue(value));
}

template <typename T>
inline void EntityData::setValue(const T& value)
{
    setValue(qMetaTypeId<T>(), value);
}

} // namespace ns

Q_DECLARE_METATYPE(ns::EntityData)

#endif // ENTITYDATA_H
