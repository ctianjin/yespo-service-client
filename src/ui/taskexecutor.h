#ifndef TASKEXECUTOR_H
#define TASKEXECUTOR_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include "abstracttask.h"
#include "uiglobal.h"
#include "application.h"

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_UI_DECL TaskExecutor : public QObject
{
    Q_OBJECT

public:
    explicit TaskExecutor(QWidget* parent = 0);
    virtual ~TaskExecutor();

    AbstractTask* task() const;
    void setTask(AbstractTask* task);

public Q_SLOTS:
    ExecutionResult exec();

protected:
    virtual void reportSuccess();
    virtual void reportError();

    QWidget* parentWidget() const;

private:
    QPointer<AbstractTask> m_task;

    Q_DISABLE_COPY(TaskExecutor)
};

} // namespace ns

#endif // TASKEXECUTOR_H
