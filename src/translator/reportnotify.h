#ifndef REPORTNOTIFY_H
#define REPORTNOTIFY_H

#include <QtCore/QScopedPointer>
#include <QtCore/QPointer>
#include <QWidget>
#include "command.h"

QT_BEGIN_NAMESPACE
class QMovie;
QT_END_NAMESPACE

namespace ns {

class Ui_ReportNotify;

class ReportNotify : public QWidget
{
    Q_OBJECT

public:
    ReportNotify::ReportNotify(QWidget* parent = 0);
    ~ReportNotify();

	void setNoReportNum(const QString& num);

Q_SIGNALS:
	void requestReportList();
protected:
	virtual void changeEvent(QEvent* event);

private	Q_SLOTS:
	void on_pushButtonCheck_clicked();
private:
	void retranslateTr();
	QString m_num;

	QScopedPointer<Ui_ReportNotify> m_ui;
};

} // namespace ns

#endif // REPORTNOTIFY_H
