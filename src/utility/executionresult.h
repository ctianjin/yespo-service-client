#ifndef EXECUTIONRESULT_H
#define EXECUTIONRESULT_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QString>
#include "utilityglobal.h"

namespace ns {

class ExecutionResultPrivate;

class BUILDSYS_UTILITY_DECL ExecutionResult
{
public:
    enum Codes
    {
        Ok = 1,
        Canceled = 0,
        Error = -1
    };

    ExecutionResult();
    ExecutionResult(const ExecutionResult& rhs);
    ExecutionResult(int code, const QString& text);
    ~ExecutionResult();

    ExecutionResult& operator=(const ExecutionResult& rhs);

    int code() const;
    void setCode(int code);

    QString text() const;
    void setText(const QString& text);

    bool isOk() const;
    bool isCanceled() const;
    bool isSuccess() const;
    bool isError() const;

    void clear();

private:
    QSharedDataPointer<ExecutionResultPrivate> d;
};

class BUILDSYS_UTILITY_DECL ExecutionSuccess : public ExecutionResult
{
public:
    explicit ExecutionSuccess(const QString& text = QString());
    explicit ExecutionSuccess(int code, const QString& text = QString());
    ExecutionSuccess(const ExecutionSuccess& rhs);
    ~ExecutionSuccess();

    ExecutionSuccess& operator=(const ExecutionSuccess& rhs);
};

class BUILDSYS_UTILITY_DECL ExecutionError : public ExecutionResult
{
public:
    explicit ExecutionError(const QString& text = QString());
    explicit ExecutionError(int code, const QString& text = QString());
    ExecutionError(const ExecutionError& rhs);
    ~ExecutionError();

    ExecutionError& operator=(const ExecutionError& rhs);
};

class BUILDSYS_UTILITY_DECL ExecutionCanceled : public ExecutionResult
{
public:
    explicit ExecutionCanceled(const QString& text = QString());
    ExecutionCanceled(const ExecutionCanceled& rhs);
    ~ExecutionCanceled();

    ExecutionCanceled& operator=(const ExecutionCanceled& rhs);
};

BUILDSYS_UTILITY_DECL QDebug operator<<(QDebug, const ExecutionResult& rhs);

} // namespace ns

Q_DECLARE_METATYPE(ns::ExecutionResult)

#endif // EXECUTIONRESULT_H
