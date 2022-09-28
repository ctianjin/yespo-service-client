#ifndef QPID_CALLSPLASH_H
#define QPID_CALLSPLASH_H

#include <QScopedPointer>
#include <QPointer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QTimer;
class QRect;
QT_END_NAMESPACE

namespace Qpid {

class SipService;
class Ui_CallSplash;

class CallSplash : public QWidget
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)

public:
	static const int stepUnit;

    explicit CallSplash(QWidget* parent = 0);
    virtual ~CallSplash();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	void setCallDetails(int callID, 
		const QString& callerName, 
		const QString& srcLang, 
		const QString& dstLang, 
		const QString& serviceMode, 
		const QString& photo);

	void slowShow();
	void slowHide();

public Q_SLOTS:

Q_SIGNALS:
	void decline();
	void answer();

protected:
	virtual void changeEvent(QEvent* event);
	virtual void resizeEvent(QResizeEvent* event);
	virtual void hideEvent(QHideEvent * event);

private Q_SLOTS:
	void slotSlowShow();

	void on_pushButtonReject_clicked();
	void on_pushButtonAnswer_clicked();

private:
	QTimer* m_slowTimer;
	QRect* m_initRect;
	int m_showHeight;

	int m_callID;

    QScopedPointer<Ui_CallSplash> m_ui;
	QPointer<SipService> m_sipService;

    Q_DISABLE_COPY(CallSplash)
};

} // namespace Qpid

#endif // QPID_CALLSPLASH_H
