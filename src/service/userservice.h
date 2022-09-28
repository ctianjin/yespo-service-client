#ifndef USERSERVICE_H
#define USERSERVICE_H

#include "abstractdataservice.h"
#include "serviceglobal.h"
#include "user/callrecord.h"
#include "user/calltype.h"
#include "user/screenshot.h"
#include "user/phonehistory.h"
#include "user/contact.h"
#include "user/blacklist.h"

QT_BEGIN_NAMESPACE
class QNetworkCookie;
QT_END_NAMESPACE

namespace ns {

class CommandService;
class UserServicePrivate;

class BUILDSYS_SERVICE_DECL UserService : public AbstractDataService
{
    Q_OBJECT

public:
    explicit UserService(ServiceManager* manager, QObject* parent = 0);
    UserService(ServiceManager* manager, const QString& name, QObject* parent = 0);
    virtual ~UserService();

	enum UserType {
		Phone = 0,
		Translator = 1
	};

    QString userName() const;
    void setUserName(const QString& userName);

    QString password() const;
    void setPassword(const QString& password);

    QString userSessionId() const;

    QString userServiceHost() const;
	QString userServiceHostNoPort() const;
	void setServiceHost(const QString& serviceHost);

	QString sipServiceHost() const;
	void setServiceSipHost(const QString& sipServiceHost);

	QString imServiceHost() const;
	QString imServicePort() const;
	QString imServiceHostNoPort() const;
	void setImServiceHost(const QString& serviceHost);

	QString statusServiceHost() const;
	QString statusServicePort() const;
	QString statusServiceHostNoPort() const;
	void setStatusServiceHost(const QString& serviceHost);

	QList<QNetworkCookie> cookies() const;
	void setCookies(const QList<QNetworkCookie>& cookies);

	QString token() const;
	void setToken(const QString& token);

	QString systemId() const;
	QString loginId() const;
	QString callerId() const;
//	QString loginId() const;
	QString secret() const;
	QString displayName() const;
	QStringList skill() const;
	QStringList local() const;
	QStringList sipServerList() const;
	QString photo() const;

	CommandService* commandService() const;

	UserType userType() const;
	void setUserType(UserType type);

	void addPhoneContact(const User::Contact& contact);
	User::Contact findPhoneContactByNumber(const QString& number);
	void deletePhoneContact(const QString& addBookID);
	bool phoneContactExist(const QString& voiceraID);
	void clearPhoneContact();
	void updatePhoneContact(const User::Contact& contact);

	/* Phone history */
	void addPhoneHistory(const User::PhoneHistory& history);
	User::PhoneHistoryList findPhoneHistory();
	void deletePhoneHistory(const QString& meetCDRID);
	QString recentPhoneHistoryID();

	/* Partner's CDR Interface */
	void addCallRecord(User::CallRecord& cdr);
	User::CallRecordList findCallRecord();
	QString recentCallRecordID();
	User::CallRecord findCallRecord(const QString& meetCDRId);
	void updateCallRecord(const User::CallRecord& cdr);
	void todayCallRecord(int& answered, int& success, int& missed);

	/* Screenshot Interface */
	QString createScreenshotName() const;
	void addScreenshot(User::Screenshot& screenshot);
	void updateScreenshotMeetCDRId(const QString& Id);
	void uploadScreenshot();

	/*Blacklist*/
	void addBlacklistRecord(User::Blacklist& blist);
	QString recentBlacklistID();
	User::BlacklistList findBlacklistRecord();
	User::Blacklist findBlacklistIdByName(const QString& name);
	void removeRecentById(const QString& id);
// 	User::Blacklist findBlackListRecord(const QString& key);
// 	void updateBlacklistRecord(User::Blacklist& blist);

    virtual void start();
    virtual void stop();

Q_SIGNALS:
	void loginSucceed();
	void loginFailed(const QString& error, const QString& errorMsg);

protected:
    UserService(UserServicePrivate& dd, QObject* parent = 0);

    virtual void applyPreference(const QSet<QString>& paths, bool force);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_loginFinished(const Command::LoginParam&, const Command::LoginResult&))
	Q_PRIVATE_SLOT(d_func(), void _q_phoneloginFinished(const Command::PhoneLoginParam&, const Command::PhoneLoginResult&))
	Q_PRIVATE_SLOT(d_func(), void _q_screenshotFinished(const Command::SaveScreenShotParam&, const Command::SaveScreenShotResult&))

    Q_DECLARE_PRIVATE(UserService)
    Q_DISABLE_COPY(UserService)
};

} // namespace ns

#endif // USERSERVICE_H
