#include <QtCore/QDebug>
#include <QDateTime>
#include <QtNetwork/QNetworkCookie>
#include <QDesktopServices>
#include <QDir>
#include <QtSystemInfo/QSystemDeviceInfo>
#include <QCryptographicHash>
#include "userservice.h"
#include "userservice_p.h"
#include "commandservice.h"
#include "servicemanager.h"
#include "user/userdatabase.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "UserService:"
#endif // DEBUG_PREFIX

namespace ns {

UserServiceOptions::UserServiceOptions()
{
}

UserServicePrivate::UserServicePrivate(ServiceManager* manager, const QString& name) :
    AbstractDataServicePrivate(manager, name),
	m_userServiceHost(),
	m_userServiceSipHost(),
	m_userServiceStatusHost(),
	m_userServiceImHost(),
	m_photoLocation(),
	m_userSessionId(),
	m_userName(),
	m_password(),
	m_systemId(),
	m_callerId(),
	m_loginId(),
	m_secret(),
	m_displayName(),
	m_skill(),
	m_local(),
	m_smallPhoto(),
	m_largePhoto(),
	m_userType(UserService::Translator)
{
    setDatabase(m_name.isEmpty() ? new User::UserDatabase : new User::UserDatabase(m_name));
    m_daos.setDatabase(m_db);
}

UserServicePrivate::~UserServicePrivate()
{
    cleanup();
}

void UserServicePrivate::init()
{
    Q_Q(UserService);
    Q_ASSERT(q);
    Q_ASSERT(m_manager);

     m_commandService = m_manager->createCommandService(q);
	 if (m_commandService) {
		 QObject::connect(m_commandService, SIGNAL(savescreenshotFinished(const Command::SaveScreenShotParam&, const Command::SaveScreenShotResult&)), 
			 q, SLOT(_q_screenshotFinished(const Command::SaveScreenShotParam&, const Command::SaveScreenShotResult&)));
	 }

	 {
		 QtMobility::QSystemDeviceInfo deviceInfo;
		 m_commandService->setCommonItem(QLatin1String("partner"), QLatin1String("1.0"), QLocale().name(), 
			 QString::fromLocal8Bit(deviceInfo.uniqueDeviceID().constData()), 
			 QLatin1String("Windows"));
	 }

	 m_photoLocation = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
	 QDir dir = m_photoLocation;
	 if (!dir.mkpath(m_photoLocation)) {
		 qWarning() << DEBUG_PREFIX << "Failed to create photo location" << m_photoLocation;
		 m_photoLocation = QString();
	 }
}

void UserServicePrivate::cleanup()
{
     if (!m_commandService->isStopped()) {
         m_commandService->stop();
     }

    m_userSessionId.clear();
   // m_userServiceHost.clear();

    AbstractDataServicePrivate::cleanup();
}

void UserServicePrivate::start()
{
	Q_Q(UserService);
	Q_ASSERT(q);

    if (!isStopped()) {
        qDebug() << DEBUG_PREFIX << "start rejected because not stopped";
        return;
    }

    setError(Service::NoError);
    setState(Service::InitState);

    Q_ASSERT(!isDatabaseOpen());
    if (!openDatabase()) {
        stopForError(Service::DataError);
        return;
    }

     m_commandService->start();
     if (m_commandService->error()) {
         stopForError(m_commandService);
         return;
	 }

    setState(Service::LoginState);

	if (m_userType == UserService::Translator) {
		Command::LoginParam param;
		param.setUserName(m_userName);
		param.setPassword(m_password);
		QObject::connect(m_commandService, SIGNAL(loginFinished(Command::LoginParam, Command::LoginResult)),
			q, SLOT(_q_loginFinished(Command::LoginParam, Command::LoginResult)));
		//m_commandService->login(param);
		m_commandService->login2(param);
	} else if(m_userType == UserService::Phone) {
		Command::PhoneLoginParam param;
		param.setVoiceraID(m_userName);
		param.setPassword(m_password);
		QObject::connect(m_commandService, SIGNAL(phoneloginFinished(Command::PhoneLoginParam, Command::PhoneLoginResult)),
			q, SLOT(_q_phoneloginFinished(Command::PhoneLoginParam, Command::PhoneLoginResult)), Qt::QueuedConnection);
		m_commandService->phonelogin(param);
	}
     
}

void UserServicePrivate::stop()
{
	Q_Q(UserService);
	Q_ASSERT(q);

	if (m_userType == UserService::Translator) {

		QObject::disconnect(m_commandService, SIGNAL(loginFinished(Command::LoginParam, Command::LoginResult)),
			q, SLOT(_q_loginFinished(Command::LoginParam, Command::LoginResult)));

	} else if(m_userType == UserService::Phone) {
		
		QObject::disconnect(m_commandService, SIGNAL(phoneloginFinished(Command::PhoneLoginParam, Command::PhoneLoginResult)),
			q, SLOT(_q_phoneloginFinished(Command::PhoneLoginParam, Command::PhoneLoginResult)));

	}

    if (isStopped()) {
        qDebug() << DEBUG_PREFIX << "stop rejected because already stopped";
        return;
    }

    if (!isActive()) {
        stopForNoError();
        return;
    }

    // TODO Send logout request, and wait for the response
    setState(Service::LogoutState);

    stopForNoError();
}

QList<QNetworkCookie> UserServicePrivate::cookies() const 
{
	return m_cookies;
}

void UserServicePrivate::setCookies(const QList<QNetworkCookie>& cookies)
{
	m_cookies = cookies;
}

QString UserServicePrivate::token() const 
{
	return m_token;
}

void UserServicePrivate::setToken(const QString& token)
{
	m_token = token;
}

void UserServicePrivate::_q_loginFinished(
    const Command::LoginParam& param, const Command::LoginResult& result)
{
	Q_Q(UserService);
	Q_ASSERT(q);

    if (m_state != Service::LoginState)
        return;

    if (m_userName!=param.userName() || m_password!=param.password()) {
        qDebug() << DEBUG_PREFIX << "Login param mismatch, ignored";
        return;
    }

	if (m_commandService->error()) {
		Q_EMIT q->loginFailed(QString(), m_commandService->errorText());
		stopForError(m_commandService);
		return;
	}

    if (!result.common().error().isEmpty()) {
		Q_EMIT q->loginFailed(result.common().errorMsg(), result.common().errorTips());
        stopForNoError();
        return;
    }

	m_systemId = result.systemId();
	m_callerId = result.callerId();
	m_loginId = result.loginId();
	m_secret = result.secret();
	m_displayName = result.displayName();
	m_skill = result.skill();
	m_local = result.local();
	m_sipServerList = result.sipServerList();
	m_userServiceStatusHost = result.statusServiceHost();
	m_userServiceSipHost = result.sipServiceHost();
	m_userServiceImHost = result.imServiceHost();
	m_smallPhoto = result.smallPhoto();
	m_largePhoto = result.largePhoto();
	m_token = result.token();

	if (m_commandService)
		m_commandService->setToken(m_token);
	qWarning() << DEBUG_PREFIX << "setstate active";
	setState(Service::ActiveState);

	Q_EMIT q->loginSucceed();
}

void UserServicePrivate::_q_phoneloginFinished(
	const Command::PhoneLoginParam& param, const Command::PhoneLoginResult& result)
{
	Q_Q(UserService);
	Q_ASSERT(q);

	if (m_state != Service::LoginState)
		return;

	if (m_userName!=param.voiceraID() || m_password!=param.password()) {
		qDebug() << DEBUG_PREFIX << "Login param mismatch, ignored";
		return;
	}

	if (m_commandService->error()) {
		Q_EMIT q->loginFailed(QString::number(m_commandService->error()), m_commandService->errorText());
		stopForError(m_commandService);
		return;
	}

	if (!result.errorMsg().isEmpty()) {
		Q_EMIT q->loginFailed(result.error(), result.errorMsg());
		stopForNoError();
		return;
	}

//	m_userSessionId = result.sessionId();
// 	m_sipAccount = result.SIPCallerID();
// 	m_userName = result.voiceraID();      //

	setState(Service::ActiveState);

	Q_EMIT q->loginSucceed();
}

void UserServicePrivate::_q_screenshotFinished(const Command::SaveScreenShotParam& param, const Command::SaveScreenShotResult& result)
{
	if (result.common().error().isEmpty()) {
		m_daos.screenshotDao.updateUploadStatusByPhoto(m_userName, param.meetCDRId(), param.photo(), User::Uploaded);
	} else {
		m_daos.screenshotDao.updateUploadStatusByPhoto(m_userName, param.meetCDRId(), param.photo(), User::Idle);
	}
}

void UserServicePrivate::addPhoneContact(const User::Contact& contact)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "addPhoneContact rejected because already stopped";
		return;
	}

	User::Contact tempContact = m_daos.contactDao.findByUserNameAndAddBookID(m_userName, contact.addBookId());
	if (!tempContact.isIdNull()) {
		tempContact.setCid(contact.cid());		
		tempContact.setAddBookId(contact.addBookId());
		tempContact.setContactName(contact.contactName());
		tempContact.setSmallPhoto(contact.smallPhoto());
		tempContact.setLargePhoto(contact.largePhoto());
		tempContact.setPhone(contact.phone());
		tempContact.setVoiceraId(contact.voiceraId());
		tempContact.setSipCallerId(contact.sipCallerId());
		tempContact.setSourceLanguage(contact.sourceLanguage());
		tempContact.setTargetLanguage(contact.targetLanguage());
		tempContact.setOnlineStatus(contact.onlineStatus());
		tempContact.setSystemUser(contact.systemUser());
		if (!m_daos.contactDao.update(tempContact)) {
			setError(Service::DataError);
		}
	} else {
		if (!m_daos.contactDao.merge(contact)) {
			setError(Service::DataError);
		}
	}
}

User::Contact UserServicePrivate::findPhoneContactByNumber(const QString& number)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "findPhoneContactByNumber rejected because already stopped";
		return User::Contact();
	}

	User::Contact contact = m_daos.contactDao.findByUserNameAndSIPCallerID(m_userName, number);
	if (contact.isValid())
		return contact;
	else {
		User::ContactList contactList = m_daos.contactDao.findByUserName(m_userName);
		if (contactList.isEmpty())
			return User::Contact();

		foreach(User::Contact c, contactList) {
			QString phone = c.phone();
			QStringList phoneCountryList = phone.split(QLatin1Char(','));
			foreach(QString phoneCountry, phoneCountryList) {
				QStringList temp = phoneCountry.split(QLatin1Char('|'));
				if (temp.size()==2 && temp.at(1)==number)
					return c;
			}
		}
		return User::Contact();
	}
}

void UserServicePrivate::deletePhoneContact(const QString& addBookID)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "deletePhoneContact rejected because already stopped";
		return;
	}

	m_daos.contactDao.removeByUserNameAndAddBookID(m_userName, addBookID);
}

bool UserServicePrivate::phoneContactExist(const QString& voiceraID)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "phoneContactExist rejected because already stopped";
		return false;
	}

	User::Contact contact;
	contact = m_daos.contactDao.findByUserNameAndVoiceraID(m_userName, voiceraID);
	if (contact.isValid())
		return true;
	return false;
}

void UserServicePrivate::clearPhoneContact()
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "clearPhoneContact rejected because already stopped";
		return;
	}

	m_daos.contactDao.removeByUserName(m_userName);
}

void UserServicePrivate::updatePhoneContact(const User::Contact& contact)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "clearPhoneContact rejected because already stopped";
		return;
	}

	m_daos.contactDao.updateByUserName(m_userName, contact);
}

void UserServicePrivate::addPhoneHistory(const User::PhoneHistory& history)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "addPhoneHistory rejected because already stopped";
		return;
	}

	if (!m_daos.phoneHistoryDao.merge(history)) {
		setError(Service::DataError);
	}
}

User::PhoneHistoryList UserServicePrivate::findPhoneHistory()
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "findPhoneHistory rejected because already stopped";
		return User::PhoneHistoryList();
	}

	return m_daos.phoneHistoryDao.findAllByUserName(m_userName);
}

void UserServicePrivate::deletePhoneHistory(const QString& meetCDRID)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "deletePhoneHistory rejected because already stopped";
		return;
	}

	m_daos.phoneHistoryDao.removeByUserNameAndMeetCDRID(m_userName, meetCDRID);
}

QString UserServicePrivate::recentPhoneHistoryID()
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "recentPhoneHistoryID rejected because already stopped";
		return QString();
	}

	QString cid;

	User::PhoneHistory phoneHistory = m_daos.phoneHistoryDao.findRecentByUserName(m_userName);
	if (phoneHistory.isValid())
		cid = phoneHistory.cid();
	
	return cid;
}

/* Implementations' of  Partner's CDR Interface */

void UserServicePrivate::addCallRecord(User::CallRecord& cdr)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "addCallRecord rejected because already stopped";
		return;
	}

	cdr.setOwner(m_userName);

	if (!m_daos.callRecordDao.merge(cdr)) {
		setError(Service::DataError);
	}
}

User::CallRecordList UserServicePrivate::findCallRecord()
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "findCallRecord rejected because already stopped";
		return User::CallRecordList();
	}

	return m_daos.callRecordDao.findAllByOwner(m_userName);
}

void UserServicePrivate::updateCallRecord(const User::CallRecord& cdr)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "updateCallRecord rejected because already stopped";
		return;
	}

	m_daos.callRecordDao.update(cdr);
}

void UserServicePrivate::todayCallRecord(int& answered, int& success, int& missed)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "todayCallRecord rejected because already stopped";
		return;
	}

	QDateTime today = QDateTime::currentDateTime();
	today.setTime(QTime(0,0,0));
	QDateTime todayBegin = today;
	today.setTime(QTime(23,59,59));
	QDateTime todayEnd = today;


	User::CallRecordList cdrAnswered = m_daos.callRecordDao.callRecordAnswered(m_userName, QString::number(todayBegin.toTime_t()), QString::number(todayEnd.toTime_t()));
	answered = cdrAnswered.size();

	User::CallRecordList cdrMissed = m_daos.callRecordDao.callRecordMissed(m_userName, QString::number(todayBegin.toTime_t()), QString::number(todayEnd.toTime_t()));
	missed = cdrMissed.size();

	User::CallRecordList cdrSuccess = m_daos.callRecordDao.callRecordSuccess(m_userName, QString::number(todayBegin.toTime_t()), QString::number(todayEnd.toTime_t()));
	success = cdrSuccess.size();
}

QString UserServicePrivate::recentCallRecordID()
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "recentCallRecordID rejected because already stopped";
		return QLatin1String("0");
	}

	User::CallRecord cdr = m_daos.callRecordDao.findRecentByOwner(m_userName);
	if (cdr.isValid())
		return cdr.cdrId();
	return QLatin1String("0");
}

User::CallRecord UserServicePrivate::findCallRecord(const QString& meetCDRId)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "recentCallRecordID rejected because already stopped";
		return User::CallRecord();
	}

	User::CallRecord cdr = m_daos.callRecordDao.findByOwnerAndMeetCDRId(m_userName, meetCDRId);
	return cdr;
}

void UserServicePrivate::addScreenshot(User::Screenshot& screenshot)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "addScreenshot rejected because already stopped";
		return;
	}

	screenshot.setOwner(m_userName);
	screenshot.setStatus(User::Idle);

	if (!m_daos.screenshotDao.merge(screenshot)) {
		setError(Service::DataError);
	}
}

void UserServicePrivate::updateScreenshotMeetCDRId(const QString& Id)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "updateScreenshotMeetCDRId rejected because already stopped";
		return;
	}

	if (!m_daos.screenshotDao.updateMeetCDRIdByOwner(m_userName, Id)) {
		setError(Service::DataError);
	}
}

void UserServicePrivate::uploadScreenshot()
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "uploadScreenshot rejected because already stopped";
		return;
	}

	if (m_commandService) {
		User::ScreenshotList screenshotList = m_daos.screenshotDao.findIdleByOwner(m_userName);
		foreach(const User::Screenshot& screenshot, screenshotList) {
			Command::SaveScreenShotParam param;
			param.setMeetCDRId(screenshot.meetCdrId());
			param.setPhoto(screenshot.photo());
			m_commandService->savescreenshot(param);
//			m_commandService->setToken(m_token);
			m_daos.screenshotDao.updateUploadStatus(m_userName, screenshot.meetCdrId(), User::Uploading);
		}
	}
	
}

void UserServicePrivate::addBlacklistRecord(User::Blacklist& blist)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "merge BlacklistRecord rejected because already stopped";
		return;
	}
	
	blist.setOwner(m_userName);

	if(!m_daos.blacklistDao.merge(blist))
		setError(Service::DataError);
}

QString UserServicePrivate::recentBlacklistID()
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "recentBlacklistID rejected because already stopped";
		return QLatin1String("0");
	}

	User::Blacklist blist = m_daos.blacklistDao.findRecentByOwner(m_userName);
	if (blist.isValid())
		return blist.blacklistId();
	return QLatin1String("0");
}

 User::BlacklistList UserServicePrivate::findBlacklistRecord()
 {
	 if (isStopped()) {
		 qDebug() << DEBUG_PREFIX << "findBlacklistRecord rejected because already stopped";
		 return User::BlacklistList();
	 }

	 return m_daos.blacklistDao.findAllByOwner(m_userName);
 }
 
 User::Blacklist UserServicePrivate::findBlacklistIdByName(const QString& name)
 {
	 if (isStopped()) {
		 qDebug() << DEBUG_PREFIX << "findBlacklistRecord rejected because already stopped";
		 return User::Blacklist();
	 }

	 return m_daos.blacklistDao.findBlacklistIdByName(name);
 }

 void UserServicePrivate::removeRecentById(const QString& id)
 {
	 if (isStopped()) {
		 qDebug() << DEBUG_PREFIX << "findBlacklistRecord rejected because already stopped";
		 return ;
	 }

	m_daos.blacklistDao.removeRecentById(id);
 }
// 
// void UserServicePrivate::updateBlacklistRecord(User::Blacklist& blist)
// {
// 
// }

UserService::UserService(ServiceManager* manager, QObject* parent) :
    AbstractDataService(*new UserServicePrivate(manager, QString()), parent)
{
    Q_D(UserService);
    d->init();
}

UserService::UserService(ServiceManager* manager, const QString& name, QObject* parent) :
    AbstractDataService(*new UserServicePrivate(manager, name), parent)
{
    Q_D(UserService);
    d->init();
}

UserService::UserService(UserServicePrivate& dd, QObject* parent) :
    AbstractDataService(dd, parent)
{
    Q_D(UserService);
    d->init();
}

UserService::~UserService()
{
}

void UserService::applyPreference(const QSet<QString>& paths, bool force)
{
    // TODO
}

QString UserService::userName() const
{
    Q_D(const UserService);
    return d->m_userName;
}

void UserService::setUserName(const QString& userName)
{
    Q_D(UserService);
    d->m_userName = userName;
}

QString UserService::password() const
{
    Q_D(const UserService);
    return d->m_password;
}

void UserService::setPassword(const QString& password)
{
    Q_D(UserService);
    d->m_password = password;
}

QString UserService::userSessionId() const
{
    Q_D(const UserService);
    return d->m_userSessionId;
}

QString UserService::userServiceHost() const
{
    Q_D(const UserService);
    return d->m_userServiceHost;
}

QString UserService::userServiceHostNoPort() const
{
	Q_D(const UserService);
	QString userServiceHost = d->m_userServiceHost;
	int splitter = userServiceHost.indexOf(QLatin1Char(':'));
	if (splitter != -1) 
		userServiceHost = userServiceHost.left(splitter);
	return userServiceHost;
}

void UserService::setServiceHost(const QString& serviceHost)
{
	Q_D(UserService);
	d->m_userServiceHost = serviceHost;
}

QString UserService::sipServiceHost() const
{
	Q_D(const UserService);
	return d->m_userServiceSipHost;
}

void UserService::setServiceSipHost(const QString& sipServiceHost)
{
	Q_D(UserService);

	d->m_userServiceSipHost = sipServiceHost;
}

QString UserService::imServiceHost() const
{
	Q_D(const UserService);

	return d->m_userServiceImHost;
}

QString UserService::imServicePort() const 
{
	Q_D(const UserService);

	QString imServiceHostPort = d->m_userServiceImHost;
	int splitter = imServiceHostPort.lastIndexOf(QLatin1Char(':'));
	if (splitter != -1) 
		imServiceHostPort = imServiceHostPort.right(imServiceHostPort.length() - splitter - 1);
	return imServiceHostPort;
}

QString UserService::imServiceHostNoPort() const
{
	Q_D(const UserService);
	QString imServiceHost = d->m_userServiceImHost;
	int splitter = imServiceHost.indexOf(QLatin1Char(':'));
	if (splitter != -1) 
		imServiceHost = imServiceHost.left(splitter);
	return imServiceHost;
}

void UserService::setImServiceHost(const QString& serviceHost)
{
	Q_D(UserService);

	d->m_userServiceImHost = serviceHost;
}

QString UserService::statusServiceHost() const 
{
	Q_D(const UserService);

	return d->m_userServiceStatusHost;
}

QString UserService::statusServicePort() const 
{
	Q_D(const UserService);

	QString statusServicePort = d->m_userServiceStatusHost;
	int splitter = statusServicePort.lastIndexOf(QLatin1Char(':'));
	if (splitter != -1) 
		statusServicePort = statusServicePort.right(statusServicePort.length() - splitter - 1);
	return statusServicePort;
}

QString UserService::statusServiceHostNoPort() const
{
	Q_D(const UserService);

	QString statusServiceHost = d->m_userServiceStatusHost;
	int splitter = statusServiceHost.indexOf(QLatin1Char(':'));
	if (splitter != -1) 
		statusServiceHost = statusServiceHost.left(splitter);
	return statusServiceHost;
}

void UserService::setStatusServiceHost(const QString& serviceHost)
{
	Q_D(UserService);

	d->m_userServiceStatusHost = serviceHost;
}

QList<QNetworkCookie> UserService::cookies() const 
{
	Q_D(const UserService);

	return d->m_cookies;
}

void UserService::setCookies(const QList<QNetworkCookie>& cookies)
{
	Q_D(UserService);

	d->setCookies(cookies);
}

QString UserService::token() const 
{
	Q_D(const UserService);

	return d->m_token;
}

void UserService::setToken(const QString& token)
{
	Q_D(UserService);

	d->setToken(token);
}

QString UserService::systemId() const
{
	Q_D(const UserService);
	return d->m_systemId;
}

QString UserService::loginId() const
{
	Q_D(const UserService);
	return d->m_loginId;
}

QString UserService::callerId() const
{
	Q_D(const UserService);
	return d->m_callerId;
}

//	QString UserService::loginId() const;

QString UserService::secret() const
{
	Q_D(const UserService);
	return d->m_secret;
}

QString UserService::displayName() const
{
	Q_D(const UserService);
	return d->m_displayName;
}

QStringList UserService::skill() const
{
	Q_D(const UserService);
	return d->m_skill;
}

QStringList UserService::local() const
{
	Q_D(const UserService);
	return d->m_local;
}

QStringList UserService::sipServerList() const 
{
	Q_D(const UserService);
	return d->m_sipServerList;
}

QString UserService::photo() const
{
	Q_D(const UserService);
	return d->m_smallPhoto;
}

CommandService* UserService::commandService() const 
{
	Q_D(const UserService);

	return d->m_commandService;
}

UserService::UserType UserService::userType() const
{
	Q_D(const UserService);
	return d->m_userType;
}

void UserService::setUserType(UserType type)
{
	Q_D(UserService);
	d->m_userType = type;
}

void UserService::addPhoneContact(const User::Contact& contact)
{
	Q_D(UserService);
	d->addPhoneContact(contact);
}

User::Contact UserService::findPhoneContactByNumber(const QString& number)
{
	Q_D(UserService);
	return d->findPhoneContactByNumber(number);
}

void UserService::deletePhoneContact(const QString& addBookID)
{
	Q_D(UserService);
	d->deletePhoneContact(addBookID);
}

bool UserService::phoneContactExist(const QString& voiceraID)
{
	Q_D(UserService);
	return d->phoneContactExist(voiceraID);
}

void UserService::clearPhoneContact()
{
	Q_D(UserService);
	d->clearPhoneContact();
}

void UserService::updatePhoneContact(const User::Contact& contact)
{
	Q_D(UserService);
	d->updatePhoneContact(contact);
}

void UserService::addPhoneHistory(const User::PhoneHistory& history)
{
	Q_D(UserService);
	d->addPhoneHistory(history);
}

User::PhoneHistoryList UserService::findPhoneHistory()
{
	Q_D(UserService);
	return d->findPhoneHistory();
}

void UserService::deletePhoneHistory(const QString& meetCDRID)
{
	Q_D(UserService);
	d->deletePhoneHistory(meetCDRID);
}

QString UserService::recentPhoneHistoryID()
{
	Q_D(UserService);

	return d->recentPhoneHistoryID();
}

void UserService::addCallRecord(User::CallRecord& cdr)
{
	Q_D(UserService);

	d->addCallRecord(cdr);
}

User::CallRecordList UserService::findCallRecord()
{
	Q_D(UserService);

	return d->findCallRecord();
}

void UserService::updateCallRecord(const User::CallRecord& cdr)
{
	Q_D(UserService);

	return d->updateCallRecord(cdr);
}

void UserService::todayCallRecord(int& answered, int& success, int& missed)
{
	Q_D(UserService);

	d->todayCallRecord(answered, success, missed);
}

QString UserService::recentCallRecordID()
{
	Q_D(UserService);

	return d->recentCallRecordID();
}

User::CallRecord UserService::findCallRecord(const QString& meetCDRId)
{
	Q_D(UserService);

	return d->findCallRecord(meetCDRId);
}

QString UserService::createScreenshotName() const
{
	Q_D(const UserService);

	QString fileName;
	int timestamp = QDateTime::currentDateTime().toTime_t();
	QString uniqueId = QString::number(timestamp) + d->m_userName;
	fileName = QString::fromLocal8Bit(QCryptographicHash::hash(uniqueId.toLocal8Bit(), QCryptographicHash::Md5).toHex().constData());

	return (d->m_photoLocation+QLatin1Char('/')+fileName+QLatin1String(".jpg"));
}

void UserService::addScreenshot(User::Screenshot& screenshot)
{
	Q_D(UserService);

	d->addScreenshot(screenshot);
}

void UserService::updateScreenshotMeetCDRId(const QString& Id)
{
	Q_D(UserService);

	d->updateScreenshotMeetCDRId(Id);
}

void UserService::uploadScreenshot()
{
	Q_D(UserService);

	d->uploadScreenshot();
}

void UserService::addBlacklistRecord(User::Blacklist& blist)
{
	Q_D(UserService);

	d->addBlacklistRecord(blist);
}

QString UserService::recentBlacklistID()
{
	Q_D(UserService);

	return d->recentBlacklistID();
}

User::BlacklistList UserService::findBlacklistRecord()
{
	Q_D(UserService);

	return d->findBlacklistRecord();
}
 
User::Blacklist UserService::findBlacklistIdByName(const QString& name)
{
	Q_D(UserService);
 
	return d->findBlacklistIdByName(name);
}

void UserService::removeRecentById(const QString& id)
{
	Q_D(UserService);

	d->removeRecentById(id);
}

void UserService::start()
{
    Q_D(UserService);
    d->start();
}

void UserService::stop()
{
    Q_D(UserService);
    d->stop();
}

} // namespace ns

#include "moc_userservice.cpp"
