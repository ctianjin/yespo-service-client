#ifndef CAPABILITYAPPLICATION_H
#define CAPABILITYAPPLICATION_H

#include <QtCore/QScopedPointer>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include "application.h"

QT_BEGIN_NAMESPACE
class QFile;
QT_END_NAMESPACE

#ifdef qApp
#  undef qApp
#endif
#define qApp (static_cast<Qpid::CapabilityApplication*>(QCoreApplication::instance()))

namespace ns {

class ServiceManager;
class UserService;
class SipService;
class AudioService;
class CommandService;
class MediaService;

class CapabilityApplication : public Application
{
    Q_OBJECT

public:
    CapabilityApplication(int& argc, char** argv);
    virtual ~CapabilityApplication();

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

	void modifyLocation();
	void escapeFileName(QString& fileName);

	QIODevice* tone(const QString& tone);

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

    bool autoCheckUpdates();

private Q_SLOTS:
    void slotServiceError();

private:
    QScopedPointer<ServiceManager> m_serviceManager;
    QScopedPointer<UserService> m_userService;
	QScopedPointer<SipService> m_sipService;
	QScopedPointer<AudioService> m_audioService;
	QScopedPointer<CommandService> m_commandService;
	QScopedPointer<MediaService> m_mediaService;

    QStringList m_links;
	QMap<QString, QFile*> m_tones;

    Q_DISABLE_COPY(CapabilityApplication)
};

} // namespace Qpid

#endif // CAPABILITYAPPLICATION_H
