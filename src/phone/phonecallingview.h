#ifndef QPID_PHONECALLINGVIEW_H
#define QPID_PHONECALLINGVIEW_H

#include <QScopedPointer>
#include "abstractphoneview.h"
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
class QTimer;
class QMovie;
QT_END_NAMESPACE

namespace Qpid {

class UserService;
class SipService;
class Ui_PhoneCallingView;
struct PhoneCallingViewOptions;
class Call;

class PhoneCallingView : public AbstractPhoneView, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)

public:
    explicit PhoneCallingView(QWidget* parent = 0);
    virtual ~PhoneCallingView();

	UserService* userService() const;
	void setUserService(UserService* userService);

	SipService* sipService() const;
	void setSipService(SipService* sipService);

// 	Call* currentCall() const;
// 	void setCurrentCall(Call* call);

	void signIn();
	void signOut();

	void callOut();
	void callIn();
	void callConnected(const QString& stausText);
	void callClosed();
	void callConfirmed();

	void showMinimized(bool customerized);

	void setUserPhoto(const QString& photo);

Q_SIGNALS:
	void maximize();
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);

private Q_SLOTS:
	void slotMute(bool mute);
	void slotSpeaker(bool open);
	void slotHangup();
	void slotDTMF(const QChar& c);
	void slotAnswer();
	void slotReject();
	void slotVolumeChanged(int value);
	void slotDuration();

private:
	void retranslateTr();

    QScopedPointer<Ui_PhoneCallingView> m_ui;
    QScopedPointer<PhoneCallingViewOptions> m_opt;
	QPointer<UserService> m_userService;
	QPointer<SipService> m_sipService;
//	QPointer<Call> m_currentCall;
	QTimer* m_durationTimer;
	int m_duration;
	QMovie *m_callConnecting;

    Q_DISABLE_COPY(PhoneCallingView)
};

} // namespace Qpid

#endif // QPID_PHONECALLINGVIEW_H
