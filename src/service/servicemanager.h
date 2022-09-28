#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include <QtCore/QObject>
#include "preferencesupport.h"
#include "serviceglobal.h"

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

namespace ns {

class ServiceManagerPrivate;
class UserService;
class SipService;
class AudioService;
class CommandService;
class MediaService;
class CameraService;
class PhotoService;
class DatabaseRegistrar;

class BUILDSYS_SERVICE_DECL ServiceManager : public QObject, public PreferenceSupport
{
    Q_OBJECT
    Q_PROPERTY(QString dataLocation READ dataLocation WRITE setDataLocation)
    Q_PROPERTY(DatabaseRegistrar* databaseRegistrar READ databaseRegistrar WRITE setDatabaseRegistrar)
    Q_PROPERTY(QNetworkAccessManager* networkAccessManager READ networkAccessManager WRITE setNetworkAccessManager)

public:
    explicit ServiceManager(QObject* parent = 0);
    virtual ~ServiceManager();

    QString dataLocation() const;
    void setDataLocation(const QString& dataLocation);

    DatabaseRegistrar* databaseRegistrar() const;
    void setDatabaseRegistrar(DatabaseRegistrar* databaseRegistrar);

    QNetworkAccessManager* networkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager* networkAccessManager);

    bool isOpen() const;
    bool open();
    void close();

    UserService* createUserService();
	SipService* createSipService(const QString& userAgent, UserService* userService);
	AudioService* createAudioService(UserService* userService);
	CommandService* createCommandService(UserService* userService=0);
	MediaService* createMediaService();
	CameraService* createCameraService();
	PhotoService* createPhotoService();

protected:
    virtual void applyPreference(const QSet<QString>& paths, bool force);

private:
    friend class AbstractService;
    friend class AbstractServicePrivate;

    ServiceManagerPrivate* const d_ptr;

    Q_DECLARE_PRIVATE(ServiceManager)
    Q_DISABLE_COPY(ServiceManager)
};

} // namespace ns

#endif // SERVICEMANAGER_H
