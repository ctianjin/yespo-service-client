#include <QDebug>
#include <QDir>
#ifdef Q_OS_WIN
#include <QProcess>
#endif // Q_OS_WIN
#include <QTimer>
#include <QIcon>
#include <QRegExp>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
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
#include "product.h"
#include "callinfo.h"
//#include "quitdialog.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneApplication:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

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
    setProductApplication(QLatin1String(QPID_APPLICATION_TELEPHONE));
    setStyleSheetPath(QLatin1String(":/style/phone.qss"));

    QIcon icon;
    icon.addFile(QLatin1String(":/images/phone_16.png"), QSize(16, 16));
    icon.addFile(QLatin1String(":/images/phone_32.png"), QSize(32, 32));
    icon.addFile(QLatin1String(":/images/phone_48.png"), QSize(48, 48));
    icon.addFile(QLatin1String(":/images/phone_64.png"), QSize(64, 64));
    icon.addFile(QLatin1String(":/images/phone_128.png"), QSize(128, 128));
    setWindowIcon(icon);

#ifdef QPID_ENABLE_SINGLE_APPLICATION
    connect(this, SIGNAL(messageReceived(QString)), this, SLOT(slotMessageReceived(QString)));
#endif // QPID_ENABLE_SINGLE_APPLICATION

	m_currentCall = new Call;

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
		qCritical() << QPID_DEBUG_PREFIX << "Failed to init service manager";
		// Not acceptable
		return false;
	}

    if (!autoCheckUpdates()) {
        qWarning() << QPID_DEBUG_PREFIX << "Failed to check updates automatically";
        // Acceptable
    }

	if (!initTones()) {
		qWarning() << QPID_DEBUG_PREFIX << "Failed to init tones";
		// Acceptable
	}

    // Enable DNS prefetch
//    QWebSettings* settings = QWebSettings::globalSettings();
//    settings->setAttribute(QWebSettings::DnsPrefetchEnabled, true);

	importLanguages();

	importCountries();

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

void PhoneApplication::clearSupportedService()
{
	m_supportedService.clear();
}

void PhoneApplication::addSupportedService(const QString& source, const QStringList& target)
{
	m_supportedService.insert(source, target);
}

QStringList PhoneApplication::supportedSource()
{
	return m_supportedService.keys();
}

QStringList PhoneApplication::supportedTarget(const QString& source)
{
	return m_supportedService.value(source, QStringList());
}

void PhoneApplication::setPSTNSupported(const QStringList& supported)
{
	m_PSTNSupported = supported;
}

QStringList PhoneApplication::PSTNSupported()
{
	return m_PSTNSupported;
}

void PhoneApplication::clearMajor()
{
	m_majors.clear();
}

void PhoneApplication::addMajor(const QString& majorCode, const QString& majorName)
{
	m_majors.insert(majorCode, majorName);
}

QString PhoneApplication::majorName(const QString& majorCode)
{
	return m_majors.value(majorCode, QString());
}

int PhoneApplication::SIPAccountID() const
{
	return m_SIPAccountID;
}

void PhoneApplication::setSIPAccountID(int ID)
{
	m_SIPAccountID = ID;
}

QString PhoneApplication::makeDestinationUrl(const QString& number, bool interpreter)
{
	if (interpreter) {
		return QString(QLatin1String("sip:4001%1@%2")).arg(number, m_userService->userServiceHostNoPort());
	} else {
		return QString(QLatin1String("sip:%1@%2")).arg(number, m_userService->userServiceHostNoPort());
	}	
}

QString PhoneApplication::languageFull(const QString& languageShort)
{
	return m_languages.value(languageShort, QString(tr("-")));
}

QString PhoneApplication::countryFull(const QString& countryShort)
{
	QStringList list = m_countries.value(countryShort, QStringList());
	if (list.empty())
		return QString(tr("-"));
	else
		return list.at(0);
}

QString PhoneApplication::countryCode(const QString& countryShort)
{
	QStringList list = m_countries.value(countryShort, QStringList());
	if (list.empty())
		return QString();
	else
		return list.at(1);
}

Call* PhoneApplication::call()
{
	return m_currentCall;
}

bool PhoneApplication::initDefaultPreference()
{
    if (!Application::initDefaultPreference()) {
        return false;
    }

    DefaultPreference defaultRoot(preferenceManager());
    return defaultRoot.import(QLatin1String(":/data/default.xml"));
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
	} else {
		m_userService->setUserType(UserService::Phone);
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

bool PhoneApplication::initTones()
{
	m_tones.insert(QLatin1String("ringing"), new QFile(QLatin1String(":/sounds/ringing.pcm"), this));
	m_tones.insert(QLatin1String("ringback"), new QFile(QLatin1String(":/sounds/ringback.pcm"), this));
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

    qDebug() << QPID_DEBUG_PREFIX << "Check updates using" << updaterPath;
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
	QFile file(QLatin1String(":/data/country.xml"));
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
		QDomElement eleCode = item.firstChildElement(QLatin1String("code"));
		if (eleShort.isNull() || eleFull.isNull() || eleCode.isNull())
			continue;
		QStringList list;
		list << eleFull.text() << eleCode.text();
		m_countries.insert(eleShort.text(), list);
	}
}

void PhoneApplication::slotServiceError()
{
    AbstractService* service = qobject_cast<AbstractService*>(sender());
    if (service) {
		QString errorText = service->errorText();
		qWarning() << QPID_DEBUG_PREFIX << service->error() << errorText;
		reportError(errorText);
    }
}

#ifdef QPID_ENABLE_SINGLE_APPLICATION
void PhoneApplication::slotMessageReceived(const QString& message)
{
    m_links << message;
    Q_EMIT linksAvailable();
}
#endif // QPID_ENABLE_SINGLE_APPLICATION

} // namespace Qpid
