#ifndef QPID_PHONEVIEW_H
#define QPID_PHONEVIEW_H

#include <QScopedPointer>
#include "abstractphoneview.h"
#include "preferencesupport.h"
#include "user/contact.h"
#include "user/phonehistory.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class UserService;
class SipService;
class CommandService;
class MediaService;
class Ui_PhoneView;
struct PhoneViewOptions;
class Call;

class PhoneView : public AbstractPhoneView, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)
	Q_PROPERTY(MediaService* mediaService READ mediaService WRITE setMediaService)
//	Q_PROPERTY(Call* currentCall READ currentCall WRITE setCurrentCall)

public:
	static const QString m_selectedStyle;
	static const QString m_unSelectedStyle;

    explicit PhoneView(QWidget* parent = 0);
    virtual ~PhoneView();

	UserService* userService() const;
	void setUserService(UserService* userService);

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	MediaService* mediaService() const;
	void setMediaService(MediaService* mediaService);

	Call* currentCall() const;
	void setCurrentCall(Call* call);

	void signIn();
	void signOut();

	void showHomeWindow();
	void showNeedInterpreterWindow();
	void showCallingWindow();
	void closeCallingWindow();

	void showContactWindow(const User::Contact& contact, 
		const QString& nativeLanguage, 
		const QString& nickname, 
		const QString& nationality, 
		const QString& major);
	void showHistoryWindow(const User::Contact& contact, 
		const User::PhoneHistory& history);
	void updateContactWindow(const User::Contact& contact);

	void getContactDetail(User::Contact& contact);

	void callOut();
	void callIncomming();
	void callConnected(const QString& statusText);
	void callClosed();
	void callConfirmed();

	void setUserPhoto(const QString& photo);

	void setPSTNSupported(const QStringList& supported);

Q_SIGNALS:
	void callingMinimize(bool minimize);
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);

private Q_SLOTS:
	void slotHomeCallClicked();

	void on_toolButtonHome_clicked();
	void on_toolButtonCall_clicked();
	void on_toolButtonAddContact_clicked();
	void on_pushButtonNeedInterpreter_clicked();
	void on_stackedWidget_currentChanged (int index);

private:
	void retranslateTr();

    QScopedPointer<Ui_PhoneView> m_ui;
    QScopedPointer<PhoneViewOptions> m_opt;
	QPointer<UserService> m_userService;
	QPointer<SipService> m_sipService;
	QPointer<CommandService> m_commandService;
	QPointer<MediaService> m_mediaService;
//	QPointer<Call> m_currentCall;
	Call *m_currentCall;
	QWidget *m_widgetBeforeCall;

    Q_DISABLE_COPY(PhoneView)
};

} // namespace Qpid

#endif // QPID_PHONEVIEW_H
