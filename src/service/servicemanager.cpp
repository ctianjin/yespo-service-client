#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkCookieJar>
#include "servicemanager.h"
#include "servicemanager_p.h"
#include "service.h"
#include "userservice.h"
#include "sipservice.h"
#include "audioservice.h"
#include "commandservice.h"
#include "mediaservice.h"
#include "cameraservice.h"
#include "photoservice.h"
#include "databaseregistrar.h"
#include "user/user.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "ServiceManager:"
#endif // DEBUG_PREFIX

namespace ns {

const QString ServiceManagerOptions::user_ = QLatin1String("user");
const QString ServiceManagerOptions::sip_ = QLatin1String("sip");
const QString ServiceManagerOptions::audio_ = QLatin1String("audio");
const QString ServiceManagerOptions::command_ = QLatin1String("command");
const QString ServiceManagerOptions::media_ = QLatin1String("media");
const QString ServiceManagerOptions::camera_ = QLatin1String("camera");
const QString ServiceManagerOptions::photo_ = QLatin1String("photo");

ServiceManagerPrivate::ServiceManagerPrivate() :
    q_ptr(0),
    m_open(false),
    m_userServiceNum(1),
	m_sipServiceNum(1),
	m_audioServiceNum(1),
	m_commandServiceNum(1),
	m_mediaServiceNum(1),
	m_cameraServiceNum(1),
	m_photoServiceNum(1)
{
}

ServiceManagerPrivate::~ServiceManagerPrivate()
{
}

QString ServiceManagerPrivate::nextUserServiceName()
{
    typedef ServiceManagerOptions Opt;
    return Opt::user_ + QString::number(m_userServiceNum++);
}

QString ServiceManagerPrivate::nextSipServiceName()
{
	typedef ServiceManagerOptions Opt;
	return Opt::sip_ + QString::number(m_sipServiceNum++);
}

QString ServiceManagerPrivate::nextAudioServiceName()
{
	typedef ServiceManagerOptions Opt;
	return Opt::audio_ + QString::number(m_audioServiceNum++);
}

QString ServiceManagerPrivate::nextCommandServiceName()
{
	typedef ServiceManagerOptions Opt;
	return Opt::command_ + QString::number(m_commandServiceNum++);
}

QString ServiceManagerPrivate::nextMediaServiceName() 
{
	typedef ServiceManagerOptions Opt;
	return Opt::media_ + QString::number(m_mediaServiceNum++);
}

QString ServiceManagerPrivate::nextCameraServiceName()
{
	typedef ServiceManagerOptions Opt;
	return Opt::camera_ + QString::number(m_cameraServiceNum++);
}

QString ServiceManagerPrivate::nextPhotoServiceName()
{
	typedef ServiceManagerOptions Opt;
	return Opt::photo_ + QString::number(m_photoServiceNum++);
}

void ServiceManagerPrivate::initNetworkAccessManager() const
{
    if (!m_networkAccessManager) {
        if (!m_dedicatedNetworkAccessManager) {
            m_dedicatedNetworkAccessManager.reset(new QNetworkAccessManager);
        }
        m_networkAccessManager = m_dedicatedNetworkAccessManager.data();

		m_networkAccessManager->setCookieJar(new QNetworkCookieJar(0));
    }
}

ServiceManager::ServiceManager(QObject* parent) :
    QObject(parent),
    d_ptr(new ServiceManagerPrivate)
{
    d_ptr->q_ptr = this;

    qRegisterMetaType<User::LoginHistory>();
    qRegisterMetaType<Service::State>();
    qRegisterMetaType<Service::Error>();
}

ServiceManager::~ServiceManager()
{
    close();
    delete d_ptr;
}

void ServiceManager::applyPreference(const QSet<QString>& paths, bool force)
{
    Q_D(ServiceManager);

    typedef ServiceManagerOptions Opt;
    Opt& opt = d->m_opt;
    Preference* pref = preference();

    // Noop
    Q_UNUSED(opt);
    Q_UNUSED(pref);
}

QString ServiceManager::dataLocation() const
{
    Q_D(const ServiceManager);
    return d->m_dataLocation;
}

void ServiceManager::setDataLocation(const QString& dataLocation)
{
    Q_D(ServiceManager);
    d->m_dataLocation = dataLocation;
}

DatabaseRegistrar* ServiceManager::databaseRegistrar() const
{
    Q_D(const ServiceManager);
    return d->m_databaseRegistrar;
}

void ServiceManager::setDatabaseRegistrar(DatabaseRegistrar* databaseRegistrar)
{
    Q_D(ServiceManager);
    d->m_databaseRegistrar = databaseRegistrar;
}

QNetworkAccessManager* ServiceManager::networkAccessManager() const
{
    Q_D(const ServiceManager);

    d->initNetworkAccessManager();

    Q_ASSERT(d->m_networkAccessManager);
    return d->m_networkAccessManager;
}

void ServiceManager::setNetworkAccessManager(QNetworkAccessManager* networkAccessManager)
{
    Q_D(ServiceManager);
    d->m_networkAccessManager = networkAccessManager;
}

bool ServiceManager::isOpen() const
{
    Q_D(const ServiceManager);
    return d->m_open;
}

bool ServiceManager::open()
{
    Q_D(ServiceManager);

    d->m_open = true;

    qDebug() << DEBUG_PREFIX << "Opened successfully";
    return true;
}

void ServiceManager::close()
{
    Q_D(ServiceManager);

    d->m_open = false;

    qDebug() << DEBUG_PREFIX << "Closed successfully";
}

UserService* ServiceManager::createUserService()
{
    Q_D(ServiceManager);

    UserService* service = new UserService(this, d->nextUserServiceName(), this);

    typedef ServiceManagerOptions Opt;
    Preference* pref = preference();
    if (pref) {
        service->setPreference(new Preference(pref, Opt::user_));
    }

    return service;
}

SipService* ServiceManager::createSipService(const QString& userAgent, UserService* userService)
{
	Q_D(ServiceManager);

	SipService* service = new SipService(this, d->nextSipServiceName(), userAgent, this);
	service->setUserService(userService);

	typedef ServiceManagerOptions Opt;
	Preference* pref = preference();
	if (pref) {
		service->setPreference(new Preference(pref, Opt::sip_));
	}

	return service;
}

AudioService* ServiceManager::createAudioService(UserService* userService)
{
	Q_D(ServiceManager);

	AudioService* service = new AudioService(this, d->nextAudioServiceName(), this);
	service->setUserService(userService);

	typedef ServiceManagerOptions Opt;
	Preference* pref = preference();
	if (pref) {
		service->setPreference(new Preference(pref, Opt::audio_));
	}

	return service;
}

CommandService* ServiceManager::createCommandService(UserService* userService)
{
	Q_D(ServiceManager);

	CommandService* service = new CommandService(this, d->nextCommandServiceName(), this);
	service->setUserService(userService);

	typedef ServiceManagerOptions Opt;
	Preference* pref = preference();
	if (pref) {
		service->setPreference(new Preference(pref, Opt::command_));
	}

	return service;
}

MediaService* ServiceManager::createMediaService()
{
	Q_D(ServiceManager);

	MediaService* service = new MediaService(this, d->nextMediaServiceName(), this);

	typedef ServiceManagerOptions Opt;
	Preference* pref = preference();
	if (pref) {
		service->setPreference(new Preference(pref, Opt::media_));
	}

	return service;
}

CameraService* ServiceManager::createCameraService()
{
	Q_D(ServiceManager);

	CameraService* service = new CameraService(this, d->nextCameraServiceName(), this);

	typedef ServiceManagerOptions Opt;
	Preference* pref = preference();
	if (pref) {
		service->setPreference(new Preference(pref, Opt::camera_));
	}

	return service;
}

PhotoService* ServiceManager::createPhotoService()
{
	Q_D(ServiceManager);

	PhotoService* service = new PhotoService(this, d->nextPhotoServiceName(), this);

	typedef ServiceManagerOptions Opt;
	Preference* pref = preference();
	if (pref) {
		service->setPreference(new Preference(pref, Opt::photo_));
	}

	return service;
}

} // namespace ns
