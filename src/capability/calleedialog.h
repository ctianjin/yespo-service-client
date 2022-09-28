#ifndef QPID_CALLEEDIALOG_H
#define QPID_CALLEEDIALOG_H

#include <QDialog>
#include <QList>
#include <QPointer>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace ns {

class Ui_CalleeDialog;
class ConfigInfo;
class SipService;

class CalleeDialog : public QDialog
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	
public:
    explicit CalleeDialog(QWidget* parent = 0);
    virtual ~CalleeDialog();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	void setConfig(ConfigInfo* cfg);
	
public Q_SLOTS:


protected:
	virtual void closeEvent(QCloseEvent * event);
	

private Q_SLOTS:
	void slotRegState(int accountid, int code, const QString& reason);
	void slotSipIncommingCall(int accountId, int callId, const QString& srcLang, const QString& dstLang, const QString& mode, const QString& serviceMode, const QString& remote);
	void slotSipCallState(int callId, int state);

	void slotSIPIncomingCallEx(const QStringList& callInContentList);

	void slotAnswerCall();
	void slotHangupCall();

	void slotTimerout();

	void on_pushButtonHangupAll_clicked();
	void on_pushButtonReload_clicked();

private:
	void hangupAll();
	void unregisterAll();

    QScopedPointer<Ui_CalleeDialog> m_ui;
	QPointer<SipService> m_sipService;
	ConfigInfo* m_config;
	QList<QWidget*> m_widgets;
	QTimer* m_timer;

    Q_DISABLE_COPY(CalleeDialog)
};

} // namespace Qpid

#endif // QPID_CALLEEDIALOG_H
