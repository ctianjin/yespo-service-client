#ifndef DATETIMERANGE_H
#define DATETIMERANGE_H

#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QTime>
#include "range.h"

namespace ns {

class DateRange : public Range<QDate>
{
public:
    DateRange() :
        Range<QDate>()
    {
    }

    DateRange(const DateRange& rhs) :
        Range<QDate>(rhs)
    {
    }

    DateRange(const Range<QDate>& rhs) :
        Range<QDate>(rhs)
    {
    }

    DateRange(const QDate& t1, const QDate& t2) :
        Range<QDate>(t1, t2)
    {
    }

    ~DateRange()
    {
    }

    DateRange& operator=(const DateRange& rhs)
    {
        Range<QDate>::operator=(rhs);
        return *this;
    }

    DateRange& operator=(const Range<QDate>& rhs)
    {
        Range<QDate>::operator=(rhs);
        return *this;
    }

    int days() const
    {
        return m_first.daysTo(m_last);
    }

    void setDaysRange(const QDate& first, int days)
    {
        setRange(first, first.addDays(days));
    }

    void setMonthsRange(const QDate& first, int months)
    {
        setRange(first, first.addMonths(months));
    }

    void setYearsRange(const QDate& first, int years)
    {
        setRange(first, first.addYears(years));
    }

private:
    using Range<QDate>::interval;
};

class TimeRange : public Range<QTime>
{
public:
    TimeRange() :
        Range<QTime>()
    {
    }

    TimeRange(const TimeRange& rhs) :
        Range<QTime>(rhs)
    {
    }

    TimeRange(const Range<QTime>& rhs) :
        Range<QTime>(rhs)
    {
    }

    TimeRange(const QTime& t1, const QTime& t2) :
        Range<QTime>(t1, t2)
    {
    }

    ~TimeRange()
    {
    }

    TimeRange& operator=(const TimeRange& rhs)
    {
        Range<QTime>::operator=(rhs);
        return *this;
    }

    TimeRange& operator=(const Range<QTime>& rhs)
    {
        Range<QTime>::operator=(rhs);
        return *this;
    }

    int secs() const
    {
        return m_first.secsTo(m_last);
    }

    int msecs() const
    {
        return m_first.msecsTo(m_last);
    }

    void setSecsRange(const QTime& first, int secs)
    {
        setRange(first, first.addSecs(secs));
    }

    void setMSecsRange(const QTime& first, int msecs)
    {
        setRange(first, first.addMSecs(msecs));
    }

private:
    using Range<QTime>::interval;
};

class DateTimeRange : public Range<QDateTime>
{
public:
    DateTimeRange() :
        Range<QDateTime>()
    {
    }

    DateTimeRange(const DateTimeRange& rhs) :
        Range<QDateTime>(rhs)
    {
    }

    DateTimeRange(const Range<QDateTime>& rhs) :
        Range<QDateTime>(rhs)
    {
    }

    DateTimeRange(const QDateTime& t1, const QDateTime& t2) :
        Range<QDateTime>(t1, t2)
    {
    }

    ~DateTimeRange()
    {
    }

    DateTimeRange& operator=(const DateTimeRange& rhs)
    {
        Range<QDateTime>::operator=(rhs);
        return *this;
    }

    DateTimeRange& operator=(const Range<QDateTime>& rhs)
    {
        Range<QDateTime>::operator=(rhs);
        return *this;
    }

    int days() const
    {
        return m_first.daysTo(m_last);
    }

    int secs() const
    {
        return m_first.secsTo(m_last);
    }

    qint64 msecs() const
    {
        return m_first.msecsTo(m_last);
    }

    void setDaysRange(const QDateTime& first, int days)
    {
        setRange(first, first.addDays(days));
    }

    void setMonthsRange(const QDateTime& first, int months)
    {
        setRange(first, first.addMonths(months));
    }

    void setYearsRange(const QDateTime& first, int years)
    {
        setRange(first, first.addYears(years));
    }

    void setSecsRange(const QDateTime& first, int secs)
    {
        setRange(first, first.addSecs(secs));
    }

    void setMSecsRange(const QDateTime& first, qint64 msecs)
    {
        setRange(first, first.addMSecs(msecs));
    }

private:
    using Range<QDateTime>::interval;
};

} // namespace ns

#endif // DATETIMERANGE_H
