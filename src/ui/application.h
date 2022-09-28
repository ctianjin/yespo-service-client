#ifndef APPLICATION_H
#define APPLICATION_H

#include <QHash>
#include <QScopedPointer>
#include <QString>
#include <QApplication>
#include "uiglobal.h"
#include "promptdialog.h"

QT_BEGIN_NAMESPACE
class QProcess;
class QSplashScreen;
class QWidget;
class QSqlError;
QT_END_NAMESPACE

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
class QtLocalPeer;
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

#ifdef qApp
#  undef qApp
#endif
#define qApp (static_cast<ns::Application*>(QCoreApplication::instance()))

namespace ns {

class Intl;
class Database;
class DatabaseRegistrar;
class Preference;
class PreferenceManager;

class BUILDSYS_UI_DECL Application : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(QString applicationId READ applicationId)
    Q_PROPERTY(QString applicationRevision READ applicationRevision WRITE setApplicationRevision)
    Q_PROPERTY(QString vendorName READ vendorName WRITE setVendorName)
    Q_PROPERTY(QString vendorDomain READ vendorDomain WRITE setVendorDomain)
    Q_PROPERTY(QString productName READ productName WRITE setProductName)
    Q_PROPERTY(QString productDomain READ productDomain WRITE setProductDomain)
    Q_PROPERTY(QString productApplication READ productApplication WRITE setProductApplication)
    Q_PROPERTY(QString styleSheetPath READ styleSheetPath WRITE setStyleSheetPath)
    Q_PROPERTY(QString dataLocation READ dataLocation WRITE setDataLocation)

public:
    Application(int& argc, char** argv, bool GUIenabled = true);
//    Application(int& argc, char** argv, Type type);
    Application(const QString& id, int& argc, char** argv);
#ifdef Q_WS_X11
    Application(Display* dpy, Qt::HANDLE visual = 0, Qt::HANDLE cmap = 0);
    Application(Display* dpy, int& argc, char** argv, Qt::HANDLE visual = 0, Qt::HANDLE cmap = 0);
    Application(Display* dpy, const QString& id, int& argc, char** argv, Qt::HANDLE visual = 0, Qt::HANDLE cmap = 0);
#endif // Q_WS_X11

    virtual ~Application();

    QString applicationId() const;

    QString applicationRevision() const;
    void setApplicationRevision(const QString& revision);

    QString vendorName() const;
    void setVendorName(const QString& vendorName);

    QString vendorDomain() const;
    void setVendorDomain(const QString& vendorDomain);

    QString productName() const;
    void setProductName(const QString& productName);

    QString productDomain() const;
    void setProductDomain(const QString& productDomain);

    QString productApplication() const;
    void setProductApplication(const QString& productApplication);

    QString styleSheetPath() const;
    void setStyleSheetPath(const QString& styleSheetPath);

    QString dataLocation() const;
    void setDataLocation(const QString& dataLocation);

	QString language() const;

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
    bool isRunning();

    QWidget* activationWindow() const;
    void setActivationWindow(QWidget* activationWindow, bool activateOnMessage = true);
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

    Intl* intl() const;
	void changeLanguage(const QString& language);

    DatabaseRegistrar* databaseRegistrar() const;

    PreferenceManager* preferenceManager() const;
    Preference* newPreference(QObject* parent = 0) const;
    Preference* newPreference(const QString& path, QObject* parent = 0) const;

    virtual bool init(QSplashScreen* splash = 0);
    virtual bool confirm(Preference* pref, const QString& text, QWidget* parent = 0);
    virtual bool confirm(const QString& prefPath, const QString& text, QWidget* parent = 0);
    virtual void reportError(const QString& errorText, QWidget* parent = 0);

	virtual int information(const QString& text, QWidget* parent = 0, PromptDialog::PromptButton button = PromptDialog::ok);
	virtual int critical(const QString& text, QWidget* parent = 0, PromptDialog::PromptButton button = PromptDialog::ok);
	virtual int question(const QString& text, QWidget* parent = 0, PromptDialog::PromptButton button = PromptDialog::ok);
	virtual int warning(const QString& text, QWidget* parent = 0, PromptDialog::PromptButton button = PromptDialog::ok);
	virtual int however(const QString& text, QWidget* parent = 0, PromptDialog::PromptButton button = PromptDialog::ok);
	virtual int weep(const QString& text, QWidget* parent = 0, PromptDialog::PromptButton button = PromptDialog::ok);

public Q_SLOTS:
    virtual void about();

    void execute(const QString& program);

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
    bool sendMessage(const QString& message, int timeout = 5000);
    void activateWindow();

Q_SIGNALS:
    void messageReceived(const QString& message);
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

protected:
    static const QString applicationGroup;
    static const QString languageKey;

    virtual bool initInfo();
    virtual bool initSettings();
    virtual bool initLanguage();
    virtual bool initStyle();
    virtual bool initDatabase();
    virtual bool initPreference();
    virtual bool initDefaultPreference();

private Q_SLOTS:
    void slotDatabaseError(Database* db, const QSqlError& e);

private:
    void sysInit(const QString& id = QString());

    QString m_id;
    QString m_revision;
    QString m_vendorName;
    QString m_vendorDomain;
    QString m_productName;
    QString m_productDomain;
    QString m_productApplication;
    QString m_styleSheetPath;
    QString m_dataLocation;
	QString m_language;

    QScopedPointer<Intl> m_intl;
    QScopedPointer<DatabaseRegistrar> m_databaseRegistrar;
    QScopedPointer<PreferenceManager> m_preferenceManager;

    QHash<QString, QProcess*> m_processes;

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
    QWidget* m_activationWindow;
    QtLocalPeer* m_localPeer;
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

    Q_DISABLE_COPY(Application)
};

} // namespace ns

#endif // APPLICATION_H
