#ifndef QPID_RECORDACCESSOR_H
#define QPID_RECORDACCESSOR_H

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace ns {

template <typename T>
class RecordAccessor
{
public:
    RecordAccessor();
    explicit RecordAccessor(const QString& fieldPrefix);
    RecordAccessor(const RecordAccessor& rhs);
    ~RecordAccessor();

    RecordAccessor& operator=(const RecordAccessor& rhs);

    QString fieldPrefix() const;
    void setFieldPrefix(const QString& fieldPrefix);

    bool isFieldNull(const T& entity, const QString& field) const;
    void setFieldNull(T& entity, const QString& field) const;

    QVariant fieldValue(const T& entity, const QString& field) const;
    void setFieldValue(T& entity, const QString& field, const QVariant& value) const;

private:
    QString m_fieldPrefix;
};

template <typename T>
inline RecordAccessor<T>::RecordAccessor()
{
}

template <typename T>
inline RecordAccessor<T>::RecordAccessor(const QString& fieldPrefix) :
    m_fieldPrefix(fieldPrefix)
{
}

template <typename T>
inline RecordAccessor<T>::RecordAccessor(const RecordAccessor& rhs) :
    m_fieldPrefix(rhs.m_fieldPrefix)
{
}

template <typename T>
inline RecordAccessor<T>::~RecordAccessor()
{
}

template <typename T>
inline RecordAccessor<T>& RecordAccessor<T>::operator=(const RecordAccessor& rhs)
{
    m_fieldPrefix = rhs.m_fieldPrefix;
    return *this;
}

template <typename T>
inline QString RecordAccessor<T>::fieldPrefix() const
{
    return m_fieldPrefix;
}

template <typename T>
inline void RecordAccessor<T>::setFieldPrefix(const QString& fieldPrefix)
{
    m_fieldPrefix = fieldPrefix;
}

template <typename T>
bool RecordAccessor<T>::isFieldNull(const T& entity, const QString& field) const
{
    return entity.isFieldNull(m_fieldPrefix + field);
}

template <typename T>
void RecordAccessor<T>::setFieldNull(T& entity, const QString& field) const
{
    entity.setFieldNull(m_fieldPrefix + field);
}

template <typename T>
QVariant RecordAccessor<T>::fieldValue(const T& entity, const QString& field) const
{
    return entity.fieldValue(m_fieldPrefix + field);
}

template <typename T>
void RecordAccessor<T>::setFieldValue(T& entity, const QString& field, const QVariant& value) const
{
    entity.setFieldValue(m_fieldPrefix + field, value);
}

} // namespace ns

#endif // QPID_RECORDACCESSOR_H
