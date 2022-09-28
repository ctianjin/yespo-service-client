#ifndef SERVICEMANAGER_P_H
#define SERVICEMANAGER_P_H

#include <QtCore/QPointer>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include "servicemanager.h"

namespace ns {

struct ServiceManagerOptions
{
    static const QString user_;
	static const QString sip_;
	static const QString audio_;
	static const QString command_;
	static const QString media_;
	static const QString camera_;
	static const QString photo_;
};

class ServiceManagerPrivate
{
    Q_DECLARE_PUBLIC(ServiceManager)

public:
    ServiceManager* q_ptr;
    ServiceManagerOptions m_opt;

    QString m_dataLocation;
    QPointer<DatabaseRegistrar> m_databaseRegistrar;
    mutable QScopedPointer<QNetworkAccessManager> m_dedicatedNetworkAccessManager;
    mutable QPointer<QNetworkAccessManager> m_networkAccessManager;
    bool m_open;

    ServiceManagerPrivate();
    virtual ~ServiceManagerPrivate();

    QString nextUserServiceName();
	QString nextSipServiceName();
	QString nextAudioServiceName();
	QString nextCommandServiceName();
	QString nextMediaServiceName();
	QString nextCameraServiceName();
	QString nextPhotoServiceName();

    void initNetworkAccessManager() const;

private:
    int m_userServiceNum;
	int m_sipServiceNum;
	int m_audioServiceNum;
	int m_commandServiceNum;
	int m_mediaServiceNum;
	int m_cameraServiceNum;
	int m_photoServiceNum;
};

} // namespace ns

#endif // SERVICEMANAGER_P_H
