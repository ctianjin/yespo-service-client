#ifndef QPID_PHONEAPPLICATION_H
#define QPID_PHONEAPPLICATION_H

#include <QtCore/QScopedPointer>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QHash>
#include "application.h"

QT_BEGIN_NAMESPACE
class QFile;
QT_END_NAMESPACE

#ifdef qApp
#  undef qApp
#endif
#define qApp (static_cast<Qpid::PhoneApplication*>(QCoreApplication::instance()))

namespace Qpid {

class ServiceManager;
class UserService;
class SipService;
class AudioService;
class CommandService;
class MediaService;
class Call;

class PhoneApplication : public Application
{
    Q_OBJECT

public:
    PhoneApplication(int& argc, char** argv);
    virtual ~PhoneApplication();

    ServiceManager* serviceManager() const;

    UserService* userService() const;
	SipService* sipService() const;
	AudioService* audioService() const;
	CommandService* commandService() const;
	MediaService* mediaService() const;

    UserService* createUserService() const;
	SipService* createSipService(UserService* userService = 0) const;
	AudioService* createAudioService(UserService* userService = 0) const;
	CommandService* createCommandService(UserService* userService = 0) const;
	MediaService* createMediaService() const;

    QStringList links() const;
    QStringList takeLinks();

    virtual bool init(QSplashScreen* splash = 0);

	virtual bool confirm(Preference* pref, const QString& text, QWidget* parent = 0);
	virtual bool confirm(const QString& prefPath, const QString& text, QWidget* parent = 0);
	bool isHideWindow(const QString& prefPath);

	void modifyLocation();
	void escapeFileName(QString& fileName);

	QIODevice* tone(const QString& tone);

	void clearSupportedService();
	void addSupportedService(const QString& source, const QStringList& target);
	QStringList supportedSource();
	QStringList supportedTarget(const QString& source);

	void setPSTNSupported(const QStringList& supported);
	QStringList PSTNSupported();

	void clearMajor();
	void addMajor(const QString& majorCode, const QString& majorName);
	QString majorName(const QString& majorCode);

	int SIPAccountID() const;
	void setSIPAccountID(int ID);
	QString makeDestinationUrl(const QString& number, bool interpreter);
	QString languageFull(const QString& languageShort);
	QString countryFull(const QString& countryShort);
	QString countryCode(const QString& countryShort);

	Call* call();  //Current Call Information Interface

public Q_SLOTS:
    void editPreferences();
    void checkUpdates(bool force = false);

    virtual void about();

Q_SIGNALS:
    void linksAvailable();
	void stopDownload();
	void sessionTimeOut();

protected:
    virtual bool initDefaultPreference();
    virtual bool initService();
	virtual bool initTones();

    bool autoCheckUpdates();

	void importLanguages();
	void importCountries();

private Q_SLOTS:
    void slotServiceError();
#ifdef QPID_ENABLE_SINGLE_APPLICATION
    void slotMessageReceived(const QString& message);
#endif // QPID_ENABLE_SINGLE_APPLICATION

private:
    QScopedPointer<ServiceManager> m_serviceManager;
    QScopedPointer<UserService> m_userService;
	QScopedPointer<SipService> m_sipService;
	QScopedPointer<AudioService> m_audioService;
	QScopedPointer<CommandService> m_commandService;
	QScopedPointer<MediaService> m_mediaService;

    QStringList m_links;
	QMap<QString, QFile*> m_tones;
	QHash<QString, QStringList> m_supportedService;
	QHash<QString, QString> m_majors;
	QHash<QString, QString> m_languages;
	QHash<QString, QStringList> m_countries;
	QStringList m_PSTNSupported;

	int m_SIPAccountID;
	Call* m_currentCall;

    Q_DISABLE_COPY(PhoneApplication)
};

} // namespace Qpid

#endif // QPID_PHONEAPPLICATION_H
