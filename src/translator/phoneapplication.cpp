#include <QDebug>
#include <QDir>
#ifdef Q_OS_WIN
#include <QProcess>
#endif // Q_OS_WIN
#include <QTimer>
#include <QIcon>
#include <QRegExp>
#include <QtXml/QDomDocument>
#include "phoneapplication.h"
#include "preference.h"
#include "defaultpreference.h"
#include "preferencedialog.h"
#include "servicemanager.h"
#include "userservice.h" 
#include "sipservice.h" 
#include "audioservice.h" 
#include "commandservice.h"
#include "mediaservice.h"
#include "cameraservice.h" 
#include "photoservice.h"
#include "product.h" 
#include "im.h"
//#include "quitdialog.h" 

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "PhoneApplication:"
#endif // DEBUG_PREFIX

namespace ns {

struct QuitDialogOptions
{
	static const QString remember_exit_;
	static const QString window_hide_;
};

const QString QuitDialogOptions::remember_exit_ = QLatin1String("remember_exit");
const QString QuitDialogOptions::window_hide_ = QLatin1String("window_hide");

struct PlayerLocationOption
{
	static const QString location_;
};

const QString PlayerLocationOption::location_ = QLatin1String("ui.player.location");

PhoneApplication::PhoneApplication(int& argc, char** argv) :
    Application(argc, argv)
{
    setProductApplication(QLatin1String(BUILDSYS_APPLICATION_TRANSLATOR));

#ifdef BUILDSYS_DEMO
	setProductApplication(QLatin1String(BUILDSYS_APPLICATION_TRANSLATOR"-DEMO"));
#endif
 
#if defined(BUILDSYS_LOCAL) && !defined(BUILDSYS_DEMO)
	setProductApplication(QLatin1String(BUILDSYS_APPLICATION_TRANSLATOR"-LOCAL")); 
#endif

    setStyleSheetPath(QLatin1String(":/style/phone.qss"));
 
    QIcon icon;  
    icon.addFile(QLatin1String(":/images/phone_16.png"), QSize(16, 16));
    icon.addFile(QLatin1String(":/images/phone_32.png"), QSize(32, 32));
    icon.addFile(QLatin1String(":/images/phone_48.png"), QSize(48, 48));
    icon.addFile(QLatin1String(":/images/phone_64.png"), QSize(64, 64));
    icon.addFile(QLatin1String(":/images/phone_128.png"), QSize(128, 128));
    setWindowIcon(icon);

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
    connect(this, SIGNAL(messageReceived(QString)), this, SLOT(slotMessageReceived(QString)));
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION
}

PhoneApplication::~PhoneApplication()
{

}

ServiceManager* PhoneApplication::serviceManager() const
{
    Q_ASSERT(m_serviceManager && "Call PhoneApplication::init() first!");
    return m_serviceManager.data();
}

UserService* PhoneApplication::userService() const
{
    Q_ASSERT(m_userService && "Call PhoneApplication::init() first!");
    return m_userService.data();
}

SipService* PhoneApplication::sipService() const
{
	Q_ASSERT(m_sipService && "Call PhoneApplication::init() first!");
	return m_sipService.data();
}

AudioService* PhoneApplication::audioService() const
{
	Q_ASSERT(m_audioService && "Call PhoneApplication::init() first!");
	return m_audioService.data();
}

CommandService* PhoneApplication::commandService() const
{
	Q_ASSERT(m_commandService && "Call PhoneApplication::init() first!");
	return m_commandService.data();
}

MediaService* PhoneApplication::mediaService() const
{
	Q_ASSERT(m_mediaService && "Call PhoneApplication::init() first!");
	return m_mediaService.data();
}

CameraService* PhoneApplication::cameraService() const
{
	Q_ASSERT(m_cameraService && "Call PhoneApplication::init() first!");
	return m_cameraService.data();
}

PhotoService* PhoneApplication::photoService() const
{
	Q_ASSERT(m_photoService && "Call PhoneApplication::init() first!");
	return m_photoService.data();
}

Im* PhoneApplication::iMessage() const 
{
	Q_ASSERT(m_iMessage);
	return m_iMessage.data();
}

UserService* PhoneApplication::createUserService() const
{
    return serviceManager()->createUserService();
}

SipService* PhoneApplication::createSipService(UserService* userService) const
{
	QString userAgent = tr("%1 %2").arg(applicationName(), applicationVersion());
	return serviceManager()->createSipService(userAgent, userService);
}

AudioService* PhoneApplication::createAudioService(UserService* userService) const
{
	return serviceManager()->createAudioService(userService);
}

CommandService* PhoneApplication::createCommandService(UserService* userService) const
{
	return serviceManager()->createCommandService(userService);
}

MediaService* PhoneApplication::createMediaService() const
{
	return serviceManager()->createMediaService();
}

CameraService* PhoneApplication::createCameraService() const
{
	return serviceManager()->createCameraService();
}

PhotoService* PhoneApplication::createPhotoService() const
{
	return serviceManager()->createPhotoService();
}

Im* PhoneApplication::createImessage() const 
{
	Im* iMessage = new Im;
	return iMessage; 
}

QStringList PhoneApplication::links() const
{
    return m_links;
}

QStringList PhoneApplication::takeLinks()
{
    QStringList ret = m_links;
    m_links.clear();
    return ret;
}

bool PhoneApplication::init(QSplashScreen* splash)
{
    if (!Application::init(splash)) {
        return false;
    }

	if (!initService()) {
		qCritical() << DEBUG_PREFIX << "Failed to init service manager";
		// Not acceptable
		return false;
	}

    if (!autoCheckUpdates()) {
        qWarning() << DEBUG_PREFIX << "Failed to check updates automatically";
        // Acceptable
    }

	if (!initTones()) {
		qWarning() << DEBUG_PREFIX << "Failed to init tones";
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

	importLanguages();

    return true;
}

bool PhoneApplication::confirm(Preference* pref, const QString& text, QWidget* parent)
{
	Q_ASSERT(pref);

	//Apply preference
// 	typedef QuitDialogOptions Opt;
// 
// 	bool remembered = pref->getBool(Opt::remember_exit_, false);
// 	int hide = pref->getInt(Opt::window_hide_, 0);
// 	if (remembered) {
// 		return true;
// 	}
// 
// 	QuitDialog dlg(activeWindow());
// 	dlg.setWindowHide(hide);
// 	dlg.setRemember(remembered);
// 	dlg.adjustSize();
// 	if (dlg.exec()) {
// 		//Save preference
// 		pref->putBool(Opt::remember_exit_, dlg.getRemember());
// 		pref->putInt(Opt::window_hide_, dlg.getWindowHide());
// 		return true;
// 	}
 	return false;
}

bool PhoneApplication::confirm(const QString& prefPath, const QString& text, QWidget* parent)
{
	Preference pref(preferenceManager(), prefPath);
	return confirm(&pref, text, parent);
}

bool PhoneApplication::isHideWindow(const QString& prefPath)
{
	typedef QuitDialogOptions Opt;

	Preference pref(preferenceManager(), prefPath);
	return (pref.getInt(Opt::window_hide_, 0)==1);
}

void PhoneApplication::modifyLocation()
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

void PhoneApplication::escapeFileName(QString& fileName)
{
	QRegExp exp(QLatin1String("[\\/:*?\"<>|]"));
	fileName.replace(exp, QLatin1String("_"));
}

QIODevice* PhoneApplication::tone(const QString& tone)
{
	return m_tones.value(tone, NULL);
}

QString PhoneApplication::languageFull(const QString& languageShort)
{
	return m_languages.value(languageShort, QString(tr("-")));
}

bool PhoneApplication::initDefaultPreference()
{
    if (!Application::initDefaultPreference()) {
        return false;
    }

    DefaultPreference defaultRoot(preferenceManager());
    return defaultRoot.import(QLatin1String(":/data/default.xml"));
}

bool PhoneApplication::initImessage()
{
	m_iMessage.reset(createImessage());
	if (m_iMessage)
		return true;
	return false;
}

bool PhoneApplication::initService()
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
	} else {
		m_commandService->start();
	}

	m_mediaService.reset(m_serviceManager->createMediaService());
	if (!m_mediaService) {
		return false;
	} else {
		m_mediaService->start();
	}

	m_cameraService.reset(m_serviceManager->createCameraService());
	if (!m_cameraService) {
		return false;
	} else {
//		m_cameraService->start();
	}

	m_photoService.reset(m_serviceManager->createPhotoService());
	if (m_photoService)
		m_photoService->start();

    // TODO Subject to change
	modifyLocation();

    connect(m_userService.data(), SIGNAL(error(ns::Service::Error)),
        this, SLOT(slotServiceError()), Qt::QueuedConnection);
	connect(m_sipService.data(), SIGNAL(error(ns::Service::Error)),
		this, SLOT(slotServiceError()), Qt::QueuedConnection);
	connect(m_audioService.data(), SIGNAL(error(ns::Service::Error)),
		this, SLOT(slotServiceError()), Qt::QueuedConnection);
	connect(m_commandService.data(), SIGNAL(error(ns::Service::Error)),
		this, SLOT(slotServiceError()), Qt::QueuedConnection);
	connect(m_mediaService.data(), SIGNAL(error(ns::Service::Error)),
		this, SLOT(slotServiceError()), Qt::QueuedConnection);

    return true;
}

bool PhoneApplication::initTones()
{
	m_tones.insert(QLatin1String("ringing"), new QFile(QLatin1String(":/sounds/ringing.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_0"), new QFile(QLatin1String(":/sounds/dtmf_0.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_1"), new QFile(QLatin1String(":/sounds/dtmf_1.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_2"), new QFile(QLatin1String(":/sounds/dtmf_2.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_3"), new QFile(QLatin1String(":/sounds/dtmf_3.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_4"), new QFile(QLatin1String(":/sounds/dtmf_4.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_5"), new QFile(QLatin1String(":/sounds/dtmf_5.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_6"), new QFile(QLatin1String(":/sounds/dtmf_6.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_7"), new QFile(QLatin1String(":/sounds/dtmf_7.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_8"), new QFile(QLatin1String(":/sounds/dtmf_8.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_9"), new QFile(QLatin1String(":/sounds/dtmf_9.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_a"), new QFile(QLatin1String(":/sounds/dtmf_a.pcm"), this));
	m_tones.insert(QLatin1String("dtmf_p"), new QFile(QLatin1String(":/sounds/dtmf_p.pcm"), this));

	QMap<QString, QFile*>::Iterator it = m_tones.begin();
	for(; it != m_tones.end(); ++it) {
		if (!it.value() || !it.value()->open(QIODevice::ReadOnly))
			return false;
	}

	return true;
}

void PhoneApplication::about()
{
//     AboutDialog dlg(activeWindow());
//     dlg.adjustSize();
//     dlg.exec();
}

void PhoneApplication::editPreferences()
{
    Preference root(preferenceManager());

    PreferenceDialog dlg(&root, activeWindow());
    dlg.adjustSize();
    dlg.exec();
}

void PhoneApplication::checkUpdates(bool force)
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

    qDebug() << DEBUG_PREFIX << "Check updates using" << updaterPath;
    QProcess::startDetached(updaterPath, args, updaterDir.absolutePath());
#endif // Q_OS_WIN
}

bool PhoneApplication::autoCheckUpdates()
{
    QTimer::singleShot(5000, this, SLOT(checkUpdates()));
    return true;
}

void PhoneApplication::importLanguages()
{
	QFile file(QLatin1String(":/data/language.xml"));
	if (!file.open(QFile::ReadOnly | QFile::Text)) 
		return;

	QDomDocument doc;
	doc.setContent(&file);

	QDomElement root = doc.documentElement();
	if (root.isNull())
		return;

	QDomNodeList items = root.elementsByTagName(QLatin1String("item"));
	int index = 0;
	for (; index < items.count(); ++index) {
		QDomNode item = items.at(index);
		if (item.isNull())
			continue;
		QDomElement eleShort = item.firstChildElement(QLatin1String("short"));
		QDomElement eleFull = item.firstChildElement(QLatin1String("full"));
		if (eleShort.isNull() || eleFull.isNull())
			continue;
		m_languages.insert(eleShort.text(), eleFull.text());
	}
}

void PhoneApplication::importCountries()
{
// 	QFile file(QLatin1String(":/data/country.xml"));
// 	if (!file.open(QFile::ReadOnly | QFile::Text)) 
// 		return;
// 
// 	QDomDocument doc;
// 	doc.setContent(&file);
// 
// 	QDomElement root = doc.documentElement();
// 	if (root.isNull())
// 		return;
// 
// 	QDomNodeList items = root.elementsByTagName(QLatin1String("item"));
// 	int index = 0;
// 	for (; index < items.count(); ++index) {
// 		QDomNode item = items.at(index);
// 		if (item.isNull())
// 			continue;
// 		QDomElement eleShort = item.firstChildElement(QLatin1String("short"));
// 		QDomElement eleFull = item.firstChildElement(QLatin1String("full"));
// 		QDomElement eleCode = item.firstChildElement(QLatin1String("code"));
// 		if (eleShort.isNull() || eleFull.isNull() || eleCode.isNull())
// 			continue;
// 		QStringList list;
// 		list << eleFull.text() << eleCode.text();
// 		m_countries.insert(eleShort.text(), list);
// 	}
}

void PhoneApplication::slotServiceError()
{
    AbstractService* service = qobject_cast<AbstractService*>(sender());
    if (service) {
		qWarning() << DEBUG_PREFIX << service->error() << service->errorText();
//		reportError(service->errorText());
    }
}

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
void PhoneApplication::slotMessageReceived(const QString& message)
{

}
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

} // namespace ns
