#include <QDebug>
#include <QDir>
#ifdef Q_OS_WIN
#include <QProcess>
#endif // Q_OS_WIN
#include <QTimer>
#include <QIcon>
#include <QRegExp>
#include "capabilityapplication.h"
#include "preference.h"
#include "defaultpreference.h"
#include "preferencedialog.h"
#include "servicemanager.h"
#include "userservice.h"
#include "sipservice.h"
#include "audioservice.h"
#include "commandservice.h"
#include "mediaservice.h"
#include "product.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "CapabilityApplication:"
#endif // QPID_DEBUG_PREFIX

namespace ns {

struct PlayerLocationOption
{
	static const QString location_;
};

const QString PlayerLocationOption::location_ = QLatin1String("ui.player.location");

CapabilityApplication::CapabilityApplication(int& argc, char** argv) :
    Application(argc, argv)
{
    setProductApplication(QLatin1String(QPID_APPLICATION_CAPABILITY));
    setStyleSheetPath(QLatin1String(":/style/phone.qss"));
}

CapabilityApplication::~CapabilityApplication()
{

}

ServiceManager* CapabilityApplication::serviceManager() const
{
    Q_ASSERT(m_serviceManager && "Call CapabilityApplication::init() first!");
    return m_serviceManager.data();
}

UserService* CapabilityApplication::userService() const
{
    Q_ASSERT(m_userService && "Call CapabilityApplication::init() first!");
    return m_userService.data();
}

SipService* CapabilityApplication::sipService() const
{
	Q_ASSERT(m_sipService && "Call CapabilityApplication::init() first!");
	return m_sipService.data();
}

AudioService* CapabilityApplication::audioService() const
{
	Q_ASSERT(m_audioService && "Call CapabilityApplication::init() first!");
	return m_audioService.data();
}

CommandService* CapabilityApplication::commandService() const
{
	Q_ASSERT(m_commandService && "Call CapabilityApplication::init() first!");
	return m_commandService.data();
}

MediaService* CapabilityApplication::mediaService() const
{
	Q_ASSERT(m_mediaService && "Call CapabilityApplication::init() first!");
	return m_mediaService.data();
}

UserService* CapabilityApplication::createUserService() const
{
    return serviceManager()->createUserService();
}

SipService* CapabilityApplication::createSipService(UserService* userService) const
{
	QString userAgent = tr("%1 %2").arg(applicationName(), applicationVersion());
	return serviceManager()->createSipService(userAgent, userService);
}

AudioService* CapabilityApplication::createAudioService(UserService* userService) const
{
	return serviceManager()->createAudioService(userService);
}

CommandService* CapabilityApplication::createCommandService(UserService* userService) const
{
	return serviceManager()->createCommandService(userService);
}

MediaService* CapabilityApplication::createMediaService() const
{
	return serviceManager()->createMediaService();
}

QStringList CapabilityApplication::links() const
{
    return m_links;
}

QStringList CapabilityApplication::takeLinks()
{
    QStringList ret = m_links;
    m_links.clear();
    return ret;
}

bool CapabilityApplication::init(QSplashScreen* splash)
{
    if (!Application::init(splash)) {
        return false;
    }

	if (!initService()) {
		qCritical() << QPID_DEBUG_PREFIX << "Failed to init service manager";
		// Not acceptable
		return false;
	}

    if (!autoCheckUpdates()) {
        qWarning() << QPID_DEBUG_PREFIX << "Failed to check updates automatically";
        // Acceptable
    }

    // Enable DNS prefetch
//    QWebSettings* settings = QWebSettings::globalSettings();
//    settings->setAttribute(QWebSettings::DnsPrefetchEnabled, true);

    // Initialize links from program arguments
    m_links.clear();

    const QString url = arguments().value(1);
    if (!url.isEmpty()) {
        m_links << url;
    }

    if (!m_links.isEmpty()) {
        Q_EMIT linksAvailable();
    }

    return true;
}

bool CapabilityApplication::confirm(Preference* pref, const QString& text, QWidget* parent)
{
	Q_ASSERT(pref);

 	return false;
}

bool CapabilityApplication::confirm(const QString& prefPath, const QString& text, QWidget* parent)
{
	Preference pref(preferenceManager(), prefPath);
	return confirm(&pref, text, parent);
}

void CapabilityApplication::modifyLocation()
{
//	Q_ASSERT(m_mediaService);

	typedef PlayerLocationOption Opt;

	const QDir defDir(dataLocation());
	const QString defCacheLocation = defDir.absoluteFilePath(QLatin1String("cache"));

	Preference pref(preferenceManager());
	QString location = pref.getString(Opt::location_);
	const QDir dir(location);
	QString cacheLocation = dir.absoluteFilePath(QLatin1String("cache"));
	if (!location.isEmpty()) {
		m_serviceManager->setDataLocation(location);
	} else {
		m_serviceManager->setDataLocation(dataLocation());
		pref.putString(Opt::location_, dataLocation());
	}
}

void CapabilityApplication::escapeFileName(QString& fileName)
{
	QRegExp exp(QLatin1String("[\\/:*?\"<>|]"));
	fileName.replace(exp, QLatin1String("_"));
}

QIODevice* CapabilityApplication::tone(const QString& tone)
{
	return m_tones.value(tone, NULL);
}

bool CapabilityApplication::initDefaultPreference()
{
    if (!Application::initDefaultPreference()) {
        return false;
    }

    DefaultPreference defaultRoot(preferenceManager());
    return defaultRoot.import(QLatin1String(":/data/default.xml"));
}

bool CapabilityApplication::initService()
{
    Q_ASSERT(!m_serviceManager);

    m_serviceManager.reset(new ServiceManager(this));

    Q_CHECK_PTR(m_serviceManager);
	modifyLocation();
    m_serviceManager->setPreference(newPreference(QLatin1String("service")));

    m_serviceManager->setDatabaseRegistrar(databaseRegistrar());
    if (!m_serviceManager->open()) {
        return false;
    }

    m_userService.reset(m_serviceManager->createUserService());
    if (!m_userService) {
        return false;
    }

	QString userAgent = tr("%1 %2").arg(applicationName(), applicationVersion());
    m_sipService.reset(m_serviceManager->createSipService(userAgent, m_userService.data()));
    if (!m_sipService) {
        return false;
    }

	m_audioService.reset(m_serviceManager->createAudioService(m_userService.data()));
	if (!m_audioService) {
		return false;
	}

	m_commandService.reset(m_serviceManager->createCommandService(m_userService.data()));
	if (!m_commandService) {
		return false;
	}

	m_mediaService.reset(m_serviceManager->createMediaService());
	if (!m_mediaService) {
		return false;
	} else {
		m_mediaService->start();
	}

    // TODO Subject to change
	modifyLocation();

    connect(m_userService.data(), SIGNAL(error(Qpid::Service::Error)),
        this, SLOT(slotServiceError()), Qt::QueuedConnection);
	connect(m_sipService.data(), SIGNAL(error(Qpid::Service::Error)),
		this, SLOT(slotServiceError()), Qt::QueuedConnection);
	connect(m_audioService.data(), SIGNAL(error(Qpid::Service::Error)),
		this, SLOT(slotServiceError()), Qt::QueuedConnection);
	connect(m_commandService.data(), SIGNAL(error(Qpid::Service::Error)),
		this, SLOT(slotServiceError()), Qt::QueuedConnection);
	connect(m_mediaService.data(), SIGNAL(error(Qpid::Service::Error)),
		this, SLOT(slotServiceError()), Qt::QueuedConnection);

    return true;
}

void CapabilityApplication::about()
{
//     AboutDialog dlg(activeWindow());
//     dlg.adjustSize();
//     dlg.exec();
}

void CapabilityApplication::editPreferences()
{
    Preference root(preferenceManager());

    PreferenceDialog dlg(&root, activeWindow());
    dlg.adjustSize();
    dlg.exec();
}

void CapabilityApplication::checkUpdates(bool force)
{
#ifdef Q_OS_WIN
    const QDir updaterDir(applicationDirPath());
    QString updaterPath = updaterDir.absoluteFilePath(QLatin1String("Up2date.exe"));
    updaterPath.prepend(QLatin1Char('\"'));
    updaterPath.append(QLatin1Char('\"'));

    QStringList args;
    if (!force) {
        args << QLatin1String("SILENCE");
    }

    qDebug() << QPID_DEBUG_PREFIX << "Check updates using" << updaterPath;
    QProcess::startDetached(updaterPath, args, updaterDir.absolutePath());
#endif // Q_OS_WIN
}

bool CapabilityApplication::autoCheckUpdates()
{
    QTimer::singleShot(5000, this, SLOT(checkUpdates()));
    return true;
}

void CapabilityApplication::slotServiceError()
{
    AbstractService* service = qobject_cast<AbstractService*>(sender());
    if (service) {
        reportError(service->errorText());
 		if (service->error() == Service::SessionInvalid) {
 			Q_EMIT sessionTimeOut();
 		}
    }
}

} // namespace Qpid
