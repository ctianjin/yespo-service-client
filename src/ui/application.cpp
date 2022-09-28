#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QFile>
#ifndef QT_NO_DEBUG_OUTPUT
#include <QLibraryInfo>
#endif // QT_NO_DEBUG_OUTPUT
#include <QProcess>
#include <QSettings>
#if QT_VERSION >= 0x050000
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
#include <QMessageBox>
#include <QSplashScreen>
#include <QWidget>
#include <QtSql/QSqlError>
#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
#include <qtlocalpeer.h>
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION
#include "application.h"
#include "intl.h"
#include "database.h"
#include "databaseregistrar.h"
#include "preferencemanager.h"
#include "preferencetransaction.h"
#include "preference.h"
#include "uistyle_p.h"
#include "confirmbox.h"
#include "version.h"
#include "product.h"
#include "promptdialog.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "Application:"
#endif // DEBUG_PREFIX

namespace ns {

const QString Application::applicationGroup = QLatin1String("Application");
const QString Application::languageKey = QLatin1String("Language");

Application::Application(int& argc, char** argv, bool GUIenabled) :
    QApplication(argc, argv, GUIenabled)
{
    sysInit();
}

// Application::Application(int& argc, char** argv, Type type) :
//     QApplication(argc, argv, type)
// {
//     sysInit();
// }

Application::Application(const QString& id, int& argc, char** argv) :
    QApplication(argc, argv)
{
    sysInit(id);
}

#ifdef Q_WS_X11
Application::Application(Display* dpy, Qt::HANDLE visual, Qt::HANDLE cmap) :
    QApplication(dpy, visual, cmap)
{
    sysInit();
}

Application::Application(Display* dpy, int& argc, char** argv, Qt::HANDLE visual, Qt::HANDLE cmap) :
    QApplication(dpy, argc, argv, visual, cmap)
{
    sysInit();
}

Application::Application(Display* dpy, const QString& id, int& argc, char** argv, Qt::HANDLE visual, Qt::HANDLE cmap) :
    QApplication(dpy, argc, argv, visual, cmap)
{
    sysInit(id);
}
#endif // Q_WS_X11

Application::~Application()
{
    // Attempt to terminate all process, and wait for finished
    foreach (QProcess* process, m_processes.values()) {
        if (process && process->state() != QProcess::NotRunning) {
            process->terminate();
        }
    }

    foreach (QProcess* process, m_processes.values()) {
        process->waitForFinished(5000);
    }
}

void Application::sysInit(const QString& id)
{
    m_id = id;
    m_revision = QLatin1String(BUILDSYS_REVISION); 
    m_vendorName = QLatin1String(BUILDSYS_VENDOR_NAME);
    m_vendorDomain = QLatin1String(BUILDSYS_VENDOR_DOMAIN);
    m_productName = QLatin1String(BUILDSYS_PRODUCT_NAME); 
    m_productDomain = QLatin1String(BUILDSYS_PRODUCT_DOMAIN); 
#ifdef Q_WS_WIN
    m_styleSheetPath = QLatin1String(":/style/default.qss"); 
#endif // Q_WS_WIN 

	setApplicationVersion(QLatin1String(BUILDSYS_VERSION)); 
 
// #ifdef BUILDSYS_OFFICAL 
// 	setApplicationVersion(QString::number(BUILDSYS_VERSION_MAJOR) + QLatin1String(".") + QString::number(BUILDSYS_VERSION_MINOR)); 
// #else
//     setApplicationVersion(QLatin1String(BUILDSYS_VERSION)); 
// #endif

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
    m_activationWindow = 0;
    m_localPeer = new QtLocalPeer(this, id);
    m_id = m_localPeer->applicationId();

    connect(m_localPeer, SIGNAL(messageReceived(QString)), SIGNAL(messageReceived(QString)));
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION
}

QString Application::applicationId() const
{
    return m_id;
}

QString Application::applicationRevision() const
{
    return m_revision;
}

void Application::setApplicationRevision(const QString& revision)
{
    m_revision = revision;
}

QString Application::vendorName() const
{
    return m_vendorName;
}

void Application::setVendorName(const QString& vendorName)
{
    m_vendorName = vendorName;
}

QString Application::vendorDomain() const
{
    return m_vendorDomain;
}

void Application::setVendorDomain(const QString& vendorDomain)
{
    m_vendorDomain = vendorDomain;
}

QString Application::productName() const
{
    return m_productName;
}

void Application::setProductName(const QString& productName)
{
    m_productName = productName;
}

QString Application::productDomain() const
{
    return m_productDomain;
}

void Application::setProductDomain(const QString& productDomain)
{
    m_productDomain = productDomain;
}

QString Application::productApplication() const
{
    return m_productApplication;
}

void Application::setProductApplication(const QString& productApplication)
{
    m_productApplication = productApplication;
}

QString Application::styleSheetPath() const
{
    return m_styleSheetPath;
}

void Application::setStyleSheetPath(const QString& styleSheetPath)
{
    m_styleSheetPath = styleSheetPath;
}

QString Application::dataLocation() const
{
    return m_dataLocation;
}

void Application::setDataLocation(const QString& dataLocation)
{
    m_dataLocation = dataLocation;
}

QString Application::language() const
{
	return m_language;
}

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
bool Application::isRunning()
{
    Q_ASSERT(m_localPeer);
    return m_localPeer->isClient();
}

QWidget* Application::activationWindow() const
{
    return m_activationWindow;
}

void Application::setActivationWindow(QWidget* activationWindow, bool activateOnMessage)
{
    m_activationWindow = activationWindow;
    if (activateOnMessage) {
        connect(m_localPeer, SIGNAL(messageReceived(QString)), this, SLOT(activateWindow()));
    } else {
        disconnect(m_localPeer, SIGNAL(messageReceived(QString)), this, SLOT(activateWindow()));
    }
}
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

Intl* Application::intl() const
{
    Q_ASSERT(m_intl && "Call Application::init() first!");
    return m_intl.data();
}

void Application::changeLanguage(const QString& language)
{
	Q_ASSERT(m_intl && "Call Application::init() first!");
	m_intl->removeTranslators();
	m_intl->installLanguage(language);
	m_language = language;
}

DatabaseRegistrar* Application::databaseRegistrar() const
{
    Q_ASSERT(m_databaseRegistrar && "Call Application::init() first!");
    return m_databaseRegistrar.data();
}

PreferenceManager* Application::preferenceManager() const
{
    Q_ASSERT(m_preferenceManager && "Call Application::init() first!");
    return m_preferenceManager.data();
}

Preference* Application::newPreference(QObject* parent) const
{
    return new Preference(preferenceManager(), parent);
}

Preference* Application::newPreference(const QString& path, QObject* parent) const
{
    return new Preference(preferenceManager(), path, parent);
}

bool Application::init(QSplashScreen* splash)
{
#ifndef QT_NO_DEBUG_OUTPUT
    qDebug() << "------------------------------------------------------------";
    qDebug() << "| Qt library info:";
    qDebug() << "|  Prefix: " << QLibraryInfo::location(QLibraryInfo::PrefixPath);
    qDebug() << "|  Binaries: " << QLibraryInfo::location(QLibraryInfo::BinariesPath);
    qDebug() << "|  Libraries: " << QLibraryInfo::location(QLibraryInfo::LibrariesPath);
    qDebug() << "|  Plugins: " << QLibraryInfo::location(QLibraryInfo::PluginsPath);
    qDebug() << "|  Data: " << QLibraryInfo::location(QLibraryInfo::DataPath);
    qDebug() << "|  Translations: " << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    qDebug() << "|  Settings: " << QLibraryInfo::location(QLibraryInfo::SettingsPath);
    qDebug() << "|  Imports: " << QLibraryInfo::location(QLibraryInfo::ImportsPath);
    qDebug() << "------------------------------------------------------------";
#endif // QT_NO_DEBUG_OUTPUT

    if (!initInfo()) {
        qWarning() << DEBUG_PREFIX << "Failed to init informations";
        // Acceptable
    }

    if (!initSettings()) {
        qWarning() << DEBUG_PREFIX << "Failed to init settings";
        // Acceptable
    }

#ifndef QT_NO_DEBUG_OUTPUT
    qDebug() << "------------------------------------------------------------";
    qDebug() << "| Application info:";
    qDebug() << "|  Application Id: " << m_id;
    qDebug() << "|  Application Version: " << applicationVersion();
    qDebug() << "|  Application Revision: " << m_revision;
    qDebug() << "|  Vendor Name: " << m_vendorName;
    qDebug() << "|  Vendor Domain: " << m_vendorDomain;
    qDebug() << "|  Product Name: " << m_productName;
    qDebug() << "|  Product Domain: " << m_productDomain;
    qDebug() << "|  Product Application: " << m_productApplication;
    qDebug() << "|  Organization Name: " << organizationName();
    qDebug() << "|  Organization Domain: " << organizationDomain();
    qDebug() << "|  Application Name: " << applicationName();
    qDebug() << "|  Settings File: " << QSettings().fileName();
    qDebug() << "|  StyleSheet File: " << m_styleSheetPath;
    qDebug() << "|  Data Location: " << m_dataLocation;
    qDebug() << "------------------------------------------------------------";
#endif // QT_NO_DEBUG_OUTPUT

    if (splash) {
        splash->show();
        processEvents();
    }

    if (!initLanguage()) {
        qWarning() << DEBUG_PREFIX << "Failed to init language";
        // Acceptable
    }

    if (!initStyle()) {
        qWarning() << DEBUG_PREFIX << "Failed to init style";
        // Acceptable
    }

    if (splash) {
        splash->showMessage(tr("Initializing..."));
        processEvents();
    }

    if (!initDatabase()) {
        qCritical() << DEBUG_PREFIX << "Failed to init database";
        // Not acceptable
        return false;
    }

    if (!initPreference()) {
        qCritical() << DEBUG_PREFIX << "Failed to init preference";
        // Not acceptable
        return false;
    }

    return true;
}

bool Application::initInfo()
{
    if (m_productApplication.isEmpty()) {
        setOrganizationName(m_vendorName);
        setOrganizationDomain(m_vendorDomain);
        setApplicationName(m_productName);

    } else {
        if (m_productName.isEmpty()) {
            setOrganizationName(m_vendorName);
        } else {
            setOrganizationName(m_vendorName + QDir::separator() + m_productName);
        }

        if (m_productDomain.isEmpty()) {
            setOrganizationDomain(m_vendorDomain);
        } else {
            setOrganizationDomain(/*m_productDomain + QLatin1Char('.') + */m_vendorDomain);
        }

        setApplicationName(m_productApplication);
    }

	qDebug() << m_dataLocation;

    if (m_dataLocation.isEmpty()) {
		const QString dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

		qDebug() << dataLocation;

        // Move to parent directory if productApplication is set
        if (m_productApplication.isEmpty()) {
            m_dataLocation = dataLocation;
        } else {
            QDir dir = dataLocation;
            dir.mkpath(QLatin1String("..")); // XXX This is needed, otherwise cdUp fail.
            dir.cdUp();
            m_dataLocation = dir.path();
        }

		m_dataLocation = dataLocation;

		qDebug() << m_dataLocation;
    }

    return true;
}

bool Application::initSettings()
{
    // Use INI format under Windows, use native format otherwise
#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif // Q_OS_WIN

    return true;
}

bool Application::initLanguage()
{
    Q_ASSERT(!m_intl);

    m_intl.reset(new Intl);
    Q_CHECK_PTR(m_intl);

    QSettings settings;
    settings.beginGroup(applicationGroup);

    const QVariant val = settings.value(languageKey);
    if (val.isValid()) {
        m_language = val.toString();
	} else {
		m_language = QLatin1String("en_US");
	}

    settings.endGroup();

    return m_intl->installLanguage(m_language);
}

bool Application::initStyle()
{
#if defined(BUILDSYS_UI_USE_QTCURVE) || defined(BUILDSYS_UI_USE_CLEANLOOKS)
    qApp->setStyle(new UiStyle);
#endif

    if (!m_styleSheetPath.isEmpty()) {
        // If -stylesheet command line option is supplied, just use it, otherwise use supplied stylesheet
        QString sheet = qApp->styleSheet();
        if (!sheet.isEmpty()) {
            qDebug() << DEBUG_PREFIX << "Using stylesheet: " << sheet;
            return true;
        }

        qDebug() << DEBUG_PREFIX << "Using stylesheet" << m_styleSheetPath;

        QFile file(m_styleSheetPath);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            qWarning() << DEBUG_PREFIX << "Failed to open" << m_styleSheetPath;
            return false;
        }

        const QByteArray data = file.readAll();
        sheet = QString::fromUtf8(data.data(), data.size());
        qApp->setStyleSheet(sheet);
    }

    return true;
}

bool Application::initDatabase()
{
    Q_ASSERT(!m_databaseRegistrar);

    m_databaseRegistrar.reset(new DatabaseRegistrar);
    Q_CHECK_PTR(m_databaseRegistrar);

    connect(m_databaseRegistrar.data(), SIGNAL(error(Database*, QSqlError)),
        this, SLOT(slotDatabaseError(Database*, QSqlError)), Qt::QueuedConnection);

    return true;
}

bool Application::initPreference()
{
    Q_ASSERT(!m_preferenceManager);

    m_preferenceManager.reset(new PreferenceManager(m_productApplication));
    Q_CHECK_PTR(m_preferenceManager);

    m_preferenceManager->setDataLocation(m_dataLocation);
    m_preferenceManager->setDatabaseRegistrar(databaseRegistrar());
    if (!m_preferenceManager->open()) {
        return false;
    }

    Version appVersion(applicationVersion());
    if (!m_preferenceManager->isDefaultPreferenceSet() || m_preferenceManager->version() < appVersion) {
        PreferenceTransaction trans(m_preferenceManager.data());

        if (!initDefaultPreference())
            return false;

        m_preferenceManager->setVersion(appVersion);
        m_preferenceManager->setDefaultPreferenceSet(true);

        if (!trans.commit())
            return false;
    }

    return true;
}

bool Application::initDefaultPreference()
{
    return true;
}

void Application::about()
{
    // Noop
}

bool Application::confirm(Preference* pref, const QString& text, QWidget* parent)
{
    return ConfirmBox::confirm(parent ? parent : activeWindow(), pref, text);
}

bool Application::confirm(const QString& prefPath, const QString& text, QWidget* parent)
{
    Preference pref(preferenceManager(), prefPath);
    return confirm(&pref, text, parent);
}

void Application::reportError(const QString& errorText, QWidget* parent)
{
    QString text = errorText;
    if (text.isEmpty()) {
        text = tr("Operation failed");
    }
	critical(text, parent ? parent : activeWindow());
//    QMessageBox::critical(parent ? parent : activeWindow(), applicationName(), text);
}

int Application::information(const QString& text, QWidget* parent, PromptDialog::PromptButton button)
{
	PromptDialog dlg(parent ? parent : activeWindow());
	return dlg.information(text, button);
}

int Application::critical(const QString& text, QWidget* parent, PromptDialog::PromptButton button)
{
	PromptDialog dlg(parent ? parent : activeWindow());
	return dlg.critical(text, button);
}

int Application::question(const QString& text, QWidget* parent, PromptDialog::PromptButton button)
{
	PromptDialog dlg(parent ? parent : activeWindow());
	return dlg.question(text, button);
}

int Application::warning(const QString& text, QWidget* parent, PromptDialog::PromptButton button)
{
	PromptDialog dlg(parent ? parent : activeWindow());
	return dlg.warning(text, button);
}

int Application::however(const QString& text, QWidget* parent, PromptDialog::PromptButton button)
{
	PromptDialog dlg(parent ? parent : activeWindow());
	return dlg.however(text, button);
}

int Application::weep(const QString& text, QWidget* parent /* = 0 */, PromptDialog::PromptButton button /* = PromptDialog::ok */)
{
	PromptDialog dlg(parent ? parent : activeWindow());
	return dlg.weep(text, button);
}

void Application::slotDatabaseError(Database* db, const QSqlError& e)
{
    QString errorText = tr("Database error: (%1) %2").arg(db->displayName(), e.text());
    reportError(errorText);
}

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
bool Application::sendMessage(const QString& message, int timeout)
{
    return m_localPeer->sendMessage(message, timeout);
}

void Application::activateWindow()
{
    if (m_activationWindow) {
		if (m_activationWindow->isMaximized())
			m_activationWindow->showNormal();
		if (m_activationWindow->isHidden())
			m_activationWindow->show();
        m_activationWindow->setWindowState(m_activationWindow->windowState() & ~Qt::WindowMinimized);
        m_activationWindow->raise();
        m_activationWindow->activateWindow();
    }
}
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

void Application::execute(const QString& program)
{
    QString actualProgram = program;
#ifdef Q_OS_WIN
    if (!actualProgram.endsWith(QLatin1String(".exe"), Qt::CaseInsensitive)) {
        actualProgram += QLatin1String(".exe");
    }
#endif // Q_OS_WIN

    QProcess* process = m_processes.value(actualProgram);
    if (!process) {
        process = new QProcess(this);
        Q_CHECK_PTR(process);
        m_processes.insert(actualProgram, process);
    }

    if (process->state() == QProcess::NotRunning) {
        process->start(actualProgram, QStringList());
    }
}

} // namespace ns
