#ifndef PHONEAPPLICATION_H
#define PHONEAPPLICATION_H

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
#define qApp (static_cast<ns::PhoneApplication*>(QCoreApplication::instance()))

namespace ns {

class ServiceManager;
class UserService;
class SipService;
class AudioService;
class CommandService;
class MediaService;
class CameraService;
class PhotoService;
class Im;

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
	CameraService* cameraService() const;
	PhotoService* photoService() const;
	Im* iMessage() const;

    UserService* createUserService() const;
	SipService* createSipService(UserService* userService = 0) const;
	AudioService* createAudioService(UserService* userService = 0) const;
	CommandService* createCommandService(UserService* userService = 0) const;
	MediaService* createMediaService() const;
	CameraService* createCameraService() const;
	PhotoService* createPhotoService() const;
	Im* createImessage() const;

    QStringList links() const;
    QStringList takeLinks();

    virtual bool init(QSplashScreen* splash = 0);

	virtual bool confirm(Preference* pref, const QString& text, QWidget* parent = 0);
	virtual bool confirm(const QString& prefPath, const QString& text, QWidget* parent = 0);
	bool isHideWindow(const QString& prefPath);

	void modifyLocation();
	void escapeFileName(QString& fileName);

	QIODevice* tone(const QString& tone);

	QString languageFull(const QString& languageShort);

public Q_SLOTS:
    void editPreferences();
    void checkUpdates(bool force = false);

    virtual void about();

Q_SIGNALS:
	void stopDownload();
	void sessionTimeOut();

protected:
    virtual bool initDefaultPreference();
	virtual bool initImessage();
    virtual bool initService();
	virtual bool initTones();

    bool autoCheckUpdates();

	void importLanguages();
	void importCountries();

private Q_SLOTS:
    void slotServiceError();
#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
    void slotMessageReceived(const QString& message);
#endif // QPID_ENABLE_SINGLE_APPLICATION

private:
    QScopedPointer<ServiceManager> m_serviceManager;
    QScopedPointer<UserService> m_userService;
	QScopedPointer<SipService> m_sipService;
	QScopedPointer<AudioService> m_audioService;
	QScopedPointer<CommandService> m_commandService;
	QScopedPointer<MediaService> m_mediaService;
	QScopedPointer<CameraService> m_cameraService;
	QScopedPointer<PhotoService> m_photoService;
	QScopedPointer<Im> m_iMessage;

    QStringList m_links;
	QMap<QString, QFile*> m_tones;
	QHash<QString, QString> m_languages;

    Q_DISABLE_COPY(PhoneApplication)
};

} // namespace ns

#endif // PHONEAPPLICATION_H
