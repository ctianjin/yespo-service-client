#include <QtCore/QDebug>
#include <QtCore/QString>
#include "executionresult.h"

namespace ns {

class ExecutionResultPrivate : public QSharedData
{
public:
    int m_code;
    QString m_text;

    ExecutionResultPrivate();
    ExecutionResultPrivate(const ExecutionResultPrivate& rhs);
    ExecutionResultPrivate(int code, const QString& text);
    ~ExecutionResultPrivate();
};

ExecutionResultPrivate::ExecutionResultPrivate() :
    QSharedData(),
    m_code(ExecutionResult::Ok),
    m_text()
{
}

ExecutionResultPrivate::ExecutionResultPrivate(const ExecutionResultPrivate& rhs) :
    QSharedData(rhs),
    m_code(rhs.m_code),
    m_text(rhs.m_text)
{
}

ExecutionResultPrivate::ExecutionResultPrivate(int code, const QString& text) :
    QSharedData(),
    m_code(code),
    m_text(text)
{
}

ExecutionResultPrivate::~ExecutionResultPrivate()
{
}

ExecutionResult::ExecutionResult() :
    d(new ExecutionResultPrivate)
{
}

ExecutionResult::ExecutionResult(const ExecutionResult& rhs) :
    d(rhs.d)
{
}

ExecutionResult::ExecutionResult(int code, const QString& text) :
    d(new ExecutionResultPrivate(code, text))
{
}

ExecutionResult::~ExecutionResult()
{
}

ExecutionResult& ExecutionResult::operator=(const ExecutionResult& rhs)
{
    d = rhs.d;
    return *this;
}

int ExecutionResult::code() const
{
    return d->m_code;
}

void ExecutionResult::setCode(int code)
{
    d->m_code = code;
}

QString ExecutionResult::text() const
{
    return d->m_text;
}

void ExecutionResult::setText(const QString& text)
{
    d->m_text = text;
}

bool ExecutionResult::isOk() const
{
    return d->m_code == Ok;
}

bool ExecutionResult::isCanceled() const
{
    return d->m_code == Canceled;
}

bool ExecutionResult::isSuccess() const
{
    return d->m_code >= Ok;
}

bool ExecutionResult::isError() const
{
    return d->m_code <= Error;
}

void ExecutionResult::clear()
{
    *this = ExecutionResult();
}

ExecutionSuccess::ExecutionSuccess(const QString& text) :
    ExecutionResult(Ok, text)
{
}

ExecutionSuccess::ExecutionSuccess(int code, const QString& text) :
    ExecutionResult(code, text)
{
    Q_ASSERT(isSuccess());
}

ExecutionSuccess::ExecutionSuccess(const ExecutionSuccess& rhs) :
    ExecutionResult(rhs)
{
}

ExecutionSuccess::~ExecutionSuccess()
{
}

ExecutionSuccess& ExecutionSuccess::operator=(const ExecutionSuccess& rhs)
{
    ExecutionResult::operator=(rhs);
    return *this;
}

ExecutionError::ExecutionError(const QString& text) :
    ExecutionResult(Error, text)
{
}

ExecutionError::ExecutionError(int code, const QString& text) :
    ExecutionResult(code, text)
{
    Q_ASSERT(isError());
}

ExecutionError::ExecutionError(const ExecutionError& rhs) :
    ExecutionResult(rhs)
{
}

ExecutionError::~ExecutionError()
{
}

ExecutionError& ExecutionError::operator=(const ExecutionError& rhs)
{
    ExecutionResult::operator=(rhs);
    return *this;
}

ExecutionCanceled::ExecutionCanceled(const QString& text) :
    ExecutionResult(Canceled, text)
{
}

ExecutionCanceled::ExecutionCanceled(const ExecutionCanceled& rhs) :
    ExecutionResult(rhs)
{
}

ExecutionCanceled::~ExecutionCanceled()
{
}

ExecutionCanceled& ExecutionCanceled::operator=(const ExecutionCanceled& rhs)
{
    ExecutionResult::operator=(rhs);
    return *this;
}

QDebug operator<<(QDebug dbg, const ExecutionResult& rhs)
{
    dbg.nospace() << '[' << rhs.code() << ',' << rhs.text() << ']';
    return dbg.space();
}

} // namespace ns
