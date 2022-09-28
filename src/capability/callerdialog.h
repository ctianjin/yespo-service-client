#ifndef QPID_CALLERDIALOG_H
#define QPID_CALLERDIALOG_H

#include <QDialog>
#include <QList>
#include <QPointer>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace ns {

class Ui_CallerDialog;
class ConfigInfo;
class SipService;

class CallerDialog : public QDialog
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	
public:
    explicit CallerDialog(QWidget* parent = 0);
    virtual ~CallerDialog();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	void setConfig(ConfigInfo* cfg);
	
public Q_SLOTS:


protected:
	virtual void closeEvent(QCloseEvent * event);
	

private Q_SLOTS:
	void slotRegState(int accountid, int code, const QString& reason);
	void slotSipCallState(int callId, int state);

	void slotMakeCall();
	void slotHangupCall();

	void slotTimerout();

	void on_pushButtonHangupAll_clicked();
	void on_pushButtonReload_clicked();

private:
	void hangupAll();
	void unregisterAll();

    QScopedPointer<Ui_CallerDialog> m_ui;
	QPointer<SipService> m_sipService;
	ConfigInfo* m_config;
	QList<QWidget*> m_widgets;
	QTimer* m_timer;

    Q_DISABLE_COPY(CallerDialog)
};

} // namespace Qpid

#endif // QPID_CALLERDIALOG_H
