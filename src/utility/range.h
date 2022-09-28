#ifndef RANGE_H
#define RANGE_H

#include <QtCore/QtGlobal>
#include <QtCore/QDebug>

namespace ns {

template <typename T>
class Range
{
public:
    Range() :
        m_first(),
        m_last()
    {
    }

    Range(const Range& rhs) :
        m_first(rhs.m_first),
        m_last(rhs.m_last)
    {
    }

    Range(const T& t1, const T& t2)
    {
        setRange(t1, t2);
    }

    ~Range()
    {
    }

    Range& operator=(const Range& rhs)
    {
        m_first = rhs.m_first;
        m_last = rhs.m_last;
        return *this;
    }

    T first() const
    {
        return m_first;
    }

    T last() const
    {
        return m_last;
    }

    void setRange(const T& t1, const T& t2)
    {
        if (t1 <= t2) {
            m_first = t1;
            m_last = t2;
        } else {
            m_first = t2;
            m_last = t1;
        }
    }

    bool isEmpty() const
    {
        return m_first == m_last;
    }

    bool equals(const Range& rhs) const
    {
        if (isEmpty())
            return rhs.isEmpty();
        return m_first == rhs.m_first && m_last == rhs.m_last;
    }

    bool contains(const T& value) const
    {
        return m_first <= value && m_last > value;
    }

    bool contains(const Range& rhs) const
    {
        return m_first <= rhs.m_first && m_last >= rhs.m_last;
    }

    bool overlaps(const Range& rhs) const
    {
        return m_first < rhs.m_last && m_last > rhs.m_first;
    }

    Range intersect(const Range& rhs) const
    {
        if (m_first >= rhs.m_last || m_last <= rhs.m_first) {
            return Range<T>();
        }
        return Range<T>(qMax(m_first, rhs.m_first), qMin(m_last, rhs.m_last));
    }

    T interval() const
    {
        return m_last - m_first;
    }

protected:
    T m_first;
    T m_last;
};

template <typename T>
inline bool operator==(const Range<T>& lhs, const Range<T>& rhs)
{
    return lhs.equals(rhs);
}

template <typename T>
inline bool operator!=(const Range<T>& lhs, const Range<T>& rhs)
{
    return !lhs.equals(rhs);
}

template <typename T>
uint qHash(const Range<T>& key)
{
    uint h1 = qHash(key.first);
    uint h2 = qHash(key.last);
    return ((h1 << 16) | (h1 >> 16)) ^ h2;
}

template <typename T>
QDebug operator<<(QDebug dbg, const Range<T>& rhs)
{
    dbg.nospace() << '[' << rhs.first() << ',' << rhs.last() << ')';
    return dbg.space();
}

} // namespace ns

#endif // RANGE_H
