#ifndef QPID_USERSERVICE_P_H
#define QPID_USERSERVICE_P_H

#include "userservice.h"
#include "abstractdataservice_p.h"
#include "command.h"
#include "user/userdaos_p.h"

namespace ns {

class CommandService;

struct UserServiceOptions
{
    UserServiceOptions();
};

class UserServicePrivate : public AbstractDataServicePrivate
{
    Q_DECLARE_PUBLIC(UserService)

public:
    UserServiceOptions m_opt;
    QPointer<CommandService> m_commandService;
    User::UserDaos m_daos;
	QString m_userServiceHost;
	QString m_userServiceSipHost;
	QString m_userServiceStatusHost;
	QString m_userServiceImHost;
	QString m_photoLocation;
	QString m_userSessionId;
    QString m_userName;
    QString m_password;
    QString m_systemId;
    QString m_callerId;
	QString m_loginId;
	QString m_secret;
	QString m_displayName;
	QStringList m_skill;
	QStringList m_local;
	QStringList m_sipServerList;
	QString m_smallPhoto;
	QString m_largePhoto;
	UserService::UserType m_userType;

	QList<QNetworkCookie> m_cookies;
	QString m_token;

    UserServicePrivate(ServiceManager* manager, const QString& name);
    virtual ~UserServicePrivate();

    void start();
    void stop();

    void _q_loginFinished(const Command::LoginParam& param, const Command::LoginResult& result);
	void _q_phoneloginFinished(const Command::PhoneLoginParam& param, const Command::PhoneLoginResult& result);
	void _q_screenshotFinished(const Command::SaveScreenShotParam& param, const Command::SaveScreenShotResult& result);

	void addPhoneContact(const User::Contact& contact);
	User::Contact findPhoneContactByNumber(const QString& number);
	void deletePhoneContact(const QString& addBookID);
	bool phoneContactExist(const QString& voiceraID);
	void clearPhoneContact();
	void updatePhoneContact(const User::Contact& contact);

	void addPhoneHistory(const User::PhoneHistory& history);
	User::PhoneHistoryList findPhoneHistory();
	void deletePhoneHistory(const QString& meetCDRID);
	QString recentPhoneHistoryID();

	QList<QNetworkCookie> cookies() const;
	void setCookies(const QList<QNetworkCookie>& cookies);

	QString token() const;
	void setToken(const QString& token);

	/* Partner's CDR Interface */
	void addCallRecord(User::CallRecord& cdr);
	User::CallRecordList findCallRecord();
	QString recentCallRecordID();
	User::CallRecord findCallRecord(const QString& meetCDRId);
	void updateCallRecord(const User::CallRecord& cdr);
	void todayCallRecord(int& answered, int& success, int& missed);

	/* Screenshot Interface */
	void addScreenshot(User::Screenshot& screenshot);
	void updateScreenshotMeetCDRId(const QString& Id);
	void uploadScreenshot();

	/*Blacklist*/
	void addBlacklistRecord(User::Blacklist& blist);
	QString recentBlacklistID();
	User::BlacklistList findBlacklistRecord();
	User::Blacklist findBlacklistIdByName(const QString& name);
	void removeRecentById(const QString& id);
// 	User::BlacklistList findBlackListRecord();
// 	User::Blacklist findBlackListRecord(const QString& key);
// 	void updateBlacklistRecord(User::Blacklist& blist);



protected:
    virtual void cleanup();

private:
    void init();
};

} // namespace ns

#endif // QPID_USERSERVICE_P_H
