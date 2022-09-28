#include <QtCore/QDebug>
#include <QDesktopServices>
#include <QCryptographicHash>
#include "photoservice.h"
#include "photoservice_p.h"
#include "mediaservice.h"
#include "servicemanager.h"
#include "media/mediadatabase.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "PhotoService:"
#endif // DEBUG_PREFIX

namespace ns {

PhotoServiceOptions::PhotoServiceOptions()
{
}

PhotoServicePrivate::PhotoServicePrivate(ServiceManager* manager, const QString& name) :
    AbstractDataServicePrivate(manager, name)
{
    setDatabase(m_name.isEmpty() ? new Media::MediaDatabase : new Media::MediaDatabase(m_name));
    m_daos.setDatabase(m_db);


	const QString dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation)+QLatin1String("/photo");
	QDir dir = dataLocation;
	dir.mkpath(dataLocation); // XXX This is needed, otherwise cdUp fail.
//	dir.cdUp();
	m_photoLocation = dir;
	qDebug() << DEBUG_PREFIX << m_photoLocation;
	
}

PhotoServicePrivate::~PhotoServicePrivate()
{
    cleanup();
}

void PhotoServicePrivate::init()
{
    Q_Q(PhotoService);
    Q_ASSERT(q);
    Q_ASSERT(m_manager);

	 m_mediaService = m_manager->createMediaService();

	 QObject::connect(m_mediaService, SIGNAL(fileDownloadLength(const QString&, qint64)), q, SLOT(_q_photoLength(const QString&, qint64)));
	 QObject::connect(m_mediaService, SIGNAL(fileDownloadBytes(const QString&, qint64)), q, SLOT(_q_photoBytes(const QString&, qint64)));
	 QObject::connect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)), q, SLOT(_q_photoDownloadFailed(const QString&, const QString&)));
	 QObject::connect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), q, SLOT(_q_photoDownloaded(const QString&, const QString&)));
}

void PhotoServicePrivate::cleanup()
{
	 if (!m_mediaService->isStopped()) {
		 m_mediaService->stop();
	 }

    AbstractDataServicePrivate::cleanup();
}

void PhotoServicePrivate::start()
{
	Q_Q(PhotoService);
	Q_ASSERT(q);

    if (!isStopped()) {
        qDebug() << DEBUG_PREFIX << "start rejected because not stopped";
        return;
    }

    setError(Service::NoError);
    setState(Service::InitState);

    Q_ASSERT(!isDatabaseOpen());
    if (!openDatabase()) {
        stopForError(Service::DataError);
        return;
    } 

	if (m_mediaService)
		m_mediaService->start();

	setState(Service::ActiveState);
}

void PhotoServicePrivate::stop()
{
	Q_Q(PhotoService);
	Q_ASSERT(q);

    if (isStopped()) {
        qDebug() << DEBUG_PREFIX << "stop rejected because already stopped";
        return;
    }

    if (!isActive()) {
        stopForNoError();
        return;
    }

    stopForNoError();
}

void PhotoServicePrivate::downloadPhoto(const QString& url)
{
	if (m_mediaService) {

		QString extensionName;
		int dotIndex = url.lastIndexOf(QLatin1Char('.'));
		if (dotIndex != -1)
			extensionName = url.mid(dotIndex);

		QString fileName = QString::fromLocal8Bit(QCryptographicHash::hash(url.toLocal8Bit(), QCryptographicHash::Md5).toHex().constData());
		fileName = m_photoLocation.absoluteFilePath(fileName+extensionName);
		Media::MediaPhoto photo;
		photo.setDownloadUrl(url);
		photo.setStatus(Media::DownloadingStatus);
		if (m_daos.mediaPhotoDao.deleteByDownloadUrl(url) && m_daos.mediaPhotoDao.merge(photo)) {
			m_mediaService->downloadFile(url, fileName);
			qDebug() << DEBUG_PREFIX << "Download file " << url << " save to " << fileName;
		} else {
			qWarning() << DEBUG_PREFIX << "Can not download photo " << url << ", Database error.";
		}		
	}
}

QString PhotoServicePrivate::photoFile(const QString& url)
{
	Media::MediaPhoto photo = m_daos.mediaPhotoDao.findByDownloadUrl(url);
	if (photo.isValid())
		return photo.filename();
	return QString();
}

bool PhotoServicePrivate::isDownloading(const QString& url)
{
	Media::MediaPhoto photo = m_daos.mediaPhotoDao.findByDownloadUrl(url);
	if (photo.isValid())
		return (photo.status() == Media::DownloadingStatus);
	return false;
}

bool PhotoServicePrivate::isReady(const QString& url)
{
	Media::MediaPhoto photo = m_daos.mediaPhotoDao.findByDownloadUrl(url);
	if (photo.isValid())
		return (photo.status() == Media::ReadyStatus);
	return false;
}

void PhotoServicePrivate::_q_photoLength(const QString& link, qint64 length)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "_q_photoLength rejected because already stopped";
		return;
	}

	QString totalSize = QString::number(length);
	Media::MediaPhoto photo = m_daos.mediaPhotoDao.findByDownloadUrl(link);
	if (!photo.isIdNull() && photo.totalSize()!=totalSize) {
		photo.setTotalSize(totalSize);
		photo.setAvailableSize(QLatin1String("0"));
		if (!m_daos.mediaPhotoDao.updateByDownloadUrl(link, photo)) {
			qWarning() << DEBUG_PREFIX << "Can not update photo total size " << link << ", Database error.";
		}
	} 
}

void PhotoServicePrivate::_q_photoBytes(const QString& link, qint64 bytes)
{
	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "_q_photoBytes rejected because already stopped";
		return;
	}

	Media::MediaPhoto photo = m_daos.mediaPhotoDao.findByDownloadUrl(link);
	if (!photo.isIdNull()) {
		qint64 availableSize = bytes + photo.availableSize().toInt();
		photo.setAvailableSize(QString::number(availableSize));
		if (!m_daos.mediaPhotoDao.updateByDownloadUrl(link, photo)) {
			qWarning() << DEBUG_PREFIX << "Can not update photo available size " << link << ", Database error.";
		}
	}
}

void PhotoServicePrivate::_q_photoDownloadFailed(const QString& link, const QString& reason)
{
	Q_Q(PhotoService);
	Q_ASSERT(q);

	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "_q_photoDownloadFailed rejected because already stopped";
		return;
	}

	if (!m_daos.mediaPhotoDao.deleteByDownloadUrl(link)) {
		qWarning() << DEBUG_PREFIX << "Can not delete photo " << link << ", Database error.";
	}

	Q_EMIT q->photoDownloadFailed(link);
}

void PhotoServicePrivate::_q_photoDownloaded(const QString& link, const QString& fileName)
{
	Q_Q(PhotoService);
	Q_ASSERT(q);

	if (isStopped()) {
		qDebug() << DEBUG_PREFIX << "_q_photoDownloaded rejected because already stopped";
		return;
	}

	Media::MediaPhoto photo = m_daos.mediaPhotoDao.findByDownloadUrl(link);
	if (!photo.isIdNull()) {
		photo.setFilename(fileName);
		photo.setStatus(Media::ReadyStatus);
		if (!m_daos.mediaPhotoDao.updateByDownloadUrl(link, photo)) {
			qWarning() << DEBUG_PREFIX << "Can not update photo file name" << link << ", Database error.";
		}

		Q_EMIT q->photoDownloaded(link, fileName);
	}
}

PhotoService::PhotoService(ServiceManager* manager, QObject* parent) :
    AbstractDataService(*new PhotoServicePrivate(manager, QString()), parent)
{
    Q_D(PhotoService);
    d->init();
}

PhotoService::PhotoService(ServiceManager* manager, const QString& name, QObject* parent) :
    AbstractDataService(*new PhotoServicePrivate(manager, name), parent)
{
    Q_D(PhotoService);
    d->init();
}

PhotoService::PhotoService(PhotoServicePrivate& dd, QObject* parent) :
    AbstractDataService(dd, parent)
{
    Q_D(PhotoService);
    d->init();
}

PhotoService::~PhotoService()
{
}

void PhotoService::applyPreference(const QSet<QString>& paths, bool force)
{
    // TODO
}

void PhotoService::start()
{
    Q_D(PhotoService);
    d->start();
}

void PhotoService::stop()
{
    Q_D(PhotoService);
    d->stop();
}

void PhotoService::downloadPhoto(const QString& url)
{
	Q_D(PhotoService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "downloadPhoto rejected because of invalid state";
		return;
	}

	d->downloadPhoto(url);
}

QString PhotoService::photoFile(const QString& url)
{
	Q_D(PhotoService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "photoFile rejected because of invalid state";
		return QString();
	}

	return d->photoFile(url);
}

bool PhotoService::isDownloading(const QString& url)
{
	Q_D(PhotoService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "isDownloading rejected because of invalid state";
		return false;
	}

	return d->isDownloading(url);
}

bool PhotoService::isReady(const QString& url)
{
	Q_D(PhotoService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "isReady rejected because of invalid state";
		return false;
	}

	return d->isReady(url);
}

} // namespace ns

#include "moc_photoservice.cpp"
