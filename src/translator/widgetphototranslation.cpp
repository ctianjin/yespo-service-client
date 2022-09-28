#include "widgetphototranslation.h"
#include "ui_widgetphototranslation.h"
#include "phoneapplication.h"
#include "mediaservice.h"
#include "imageview.h"
#include "customloadinglabel.h"

#include <QDesktopServices>
#include <QFileInfo>
#include <QScrollBar>
#include <QDir>
#include <QDebug>
#include <QUuid>

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "WidgetPhotoTranslation:"
#endif // DEBUG_PREFIX

namespace ns {


WidgetPhotoTranslation::WidgetPhotoTranslation(QWidget* parent) :
    QWidget(parent),
	m_ui(new Ui_WidgetPhotoTranslation),
	m_photoIdList(),
	m_photoDirName()
{
	m_ui->setupUi(this);
	
// 	QPalette pal;
// 	const QColor color(Qt::transparent);
// 	pal.setColor(QPalette::Window, color);
// 	m_ui->scrollArea->setPalette(pal);
// 	m_ui->scrollAreaWidgetContents->setPalette(pal);

	m_ui->scrollArea->setStyleSheet(QLatin1String("background-color:transparent;"));

	if (!m_imageView) {
		m_imageView.reset(new ImageView);
	}

	m_ui->pushButtonLeft->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/bi_calling_left_slider.png);}\
		QPushButton:hover{border-image: url(:/images/ui/bi_calling_left_slider_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/bi_calling_left_slider_pressed.png);}"));

	m_ui->pushButtonRight->setStyleSheet(QLatin1String("QPushButton{border-image: url(:/images/ui/bi_calling_right_slider.png);}\
													  QPushButton:hover{border-image: url(:/images/ui/bi_calling_right_slider_hover.png);}QPushButton:pressed{border-image: url(:/images/ui/bi_calling_right_slider_pressed.png);}"));

	m_ui->pushButtonLeft->hide();
	m_ui->pushButtonRight->hide();
}

WidgetPhotoTranslation::~WidgetPhotoTranslation()
{
	
}

void WidgetPhotoTranslation::init(const QStringList& photoIdList)
{
	//photo translation while incoming call
	QUuid id = QUuid::createUuid();
	QString idName = id.toString();
	QString dirName;
	QDir dir(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));

	if (!dir.absolutePath().endsWith(QLatin1String("/"))) {
		dirName = dir.absolutePath().append(QLatin1String("/")).append(idName);
	} else {
		dirName = dir.absolutePath().append(idName);
	}

	m_photoDirName = dirName;

	int count = photoIdList.size();
	if (count <= 0)
		return;

	m_photoIdList = photoIdList;
	for (int i = 0; i < count; i++) {
		CustomLoadingLabel* label = new CustomLoadingLabel(this);
		label->setPhotoId(photoIdList.at(i));
 		connect(label, SIGNAL(signalClicked(const QString&, const QString&)), m_imageView.data(), SLOT(slotSetImageDir(const QString&, const QString&)));
 		//m_ui->horizontalLayout->insertWidget(i + 1, label);
		/*m_ui->scrollAreaWidgetContents->layout()->addWidget(label);*/
		m_ui->horizontalLayout_2->insertWidget(i + 1, label);
		m_labelPhotoList.append(label);
	}
}

void WidgetPhotoTranslation::init(const QString& photoId)
{
	//photo translation while calling
	if (photoId.isEmpty())
		return;

	if (m_photoDirName.isEmpty()) {
		QUuid id = QUuid::createUuid();

		QString idName = id.toString();
		QString dirName;
		QDir dir(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));

		if (!dir.absolutePath().endsWith(QLatin1String("/"))) {
			dirName = dir.absolutePath().append(QLatin1String("/")).append(idName);
		} else {
			dirName = dir.absolutePath().append(idName);
		}

		m_photoDirName = dirName;
	}

	m_photoIdList.append(photoId);
	int index = m_photoIdList.count();
	
	CustomLoadingLabel* label = new CustomLoadingLabel(this);
	label->setPhotoId(photoId);
	connect(label, SIGNAL(signalClicked(const QString&, const QString&)), m_imageView.data(), SLOT(slotSetImageDir(const QString&, const QString&)));
	/*m_ui->horizontalLayout->insertWidget(index, label);*/
	m_ui->horizontalLayout_2->insertWidget(index, label);
	m_labelPhotoList.append(label);

	m_ui->pushButtonLeft->setVisible(m_ui->scrollArea->horizontalScrollBar()->maximum() != m_ui->scrollArea->horizontalScrollBar()->minimum());
	m_ui->pushButtonRight->setVisible(m_ui->scrollArea->horizontalScrollBar()->maximum() != m_ui->scrollArea->horizontalScrollBar()->minimum());
}

void WidgetPhotoTranslation::uninit()
{
	if (m_photoIdList.count() <= 0)
		return;

	foreach(CustomLoadingLabel * label, m_labelPhotoList) {
		disconnect(label, SIGNAL(signalClicked(const QString&, const QString&)), m_imageView.data(), SLOT(slotSetImageDir(const QString&, const QString&)));
		delete label;
	}

	m_photoIdList.clear();
	m_labelPhotoList.clear();

	if (m_imageView) {
		m_imageView->resize(m_imageView->minimumSize());
		m_imageView->hide();
	}

	if (!m_photoDirName.isEmpty() && !removePhotoDir(m_photoDirName)) {
		qWarning() << DEBUG_PREFIX << "remove translation photo directory failed";
	}

	m_photoDirName = QString();
	m_ui->pushButtonLeft->hide();
	m_ui->pushButtonRight->hide();
}

MediaService* WidgetPhotoTranslation::mediaService() const 
{
	return m_mediaService;
}

void WidgetPhotoTranslation::setMediaService(MediaService* mediaService)
{
	if (m_mediaService) {
		disconnect(m_mediaService, SIGNAL(fileDownloadLength(const QString&, qint64)), 
			this, SLOT(slotFileDownloadLength(const QString&, qint64)));
		disconnect(m_mediaService, SIGNAL(fileDownloadBytes(const QString&, qint64)), 
			this, SLOT(slotFileDownloadBytes(const QString&, qint64)));
		disconnect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)), 
			this, SLOT(slotFileDownloadFailed(const QString&, const QString&)));
		disconnect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
			this, SLOT(slotFileDownloaded(const QString&, const QString&)));
	}

	m_mediaService = mediaService;

	if (m_mediaService) {
		connect(m_mediaService, SIGNAL(fileDownloadLength(const QString&, qint64)), 
			this, SLOT(slotFileDownloadLength(const QString&, qint64)));
		connect(m_mediaService, SIGNAL(fileDownloadBytes(const QString&, qint64)), 
			this, SLOT(slotFileDownloadBytes(const QString&, qint64)));
		connect(m_mediaService, SIGNAL(fileDownloadFailed(const QString&, const QString&)), 
			this, SLOT(slotFileDownloadFailed(const QString&, const QString&)));
		connect(m_mediaService, SIGNAL(fileDownloaded(const QString&, const QString&)), 
			this, SLOT(slotFileDownloaded(const QString&, const QString&)));
	}
}

void WidgetPhotoTranslation::downloadPhoto(const QString& largePhotolink, const QString& photoId)
{
	Q_ASSERT(m_mediaService);

	QFileInfo info(largePhotolink);
	QString suffix = info.suffix();

	if (m_photoDirName.isEmpty()) {
		qWarning() << DEBUG_PREFIX << "download translation photo failed because create photo directory failed";
		return ;
	}

	QDir photoDir(m_photoDirName);

	if (!photoDir.exists(m_photoDirName)) {
		if(!photoDir.mkdir(m_photoDirName)) {
			qApp->critical(tr("Create translation photo directory failed."));
			return;
		}
	}
	QString localFile = photoDir.absoluteFilePath(QString(QLatin1String("%1.%2")).arg(photoId).arg(suffix));
	m_mediaService->downloadFile(largePhotolink, localFile);

	startAnimation(photoId, largePhotolink, localFile, m_photoDirName);
}

void WidgetPhotoTranslation::setPhotoPicture(const QString& photoLink, const QString& photoFile)
{
	if (m_photoIdList.size() <= 0)
		return ;

	QPixmap pic(photoFile);

	for (int i = 0; i < m_labelPhotoList.count(); i++) {
		if (photoLink == m_labelPhotoList.at(i)->smallPhotoLink()) {
			m_labelPhotoList.at(i)->setSmallPhoto(pic);
			break;
		}
	}
}

void WidgetPhotoTranslation::setPhotoPicture(const QString& photoId, const QString& photoLink, const QString& photoFile)
{
	if (m_photoIdList.size() <= 0)
		return ;

	QPixmap pic(photoFile);

	for (int i = 0; i < m_labelPhotoList.count(); i++) {
		if (photoLink == m_labelPhotoList.at(i)->smallPhotoLink() && photoId == m_labelPhotoList.at(i)->photoId()) {
			m_labelPhotoList.at(i)->setSmallPhoto(pic);
			break;
		}
	}
}

void WidgetPhotoTranslation::setSmallPhotoUrl(const QString& photoId, const QString& photoLink)
{
	if (m_photoIdList.size() <= 0)
		return ;

	for (int i = 0; i < m_labelPhotoList.count(); i++) {
		if (photoId == m_labelPhotoList.at(i)->photoId()) {
			m_labelPhotoList.at(i)->setSmallPhotoLink(photoLink);
			break;
		}
	}
}

QString WidgetPhotoTranslation::photoDirPath() const 
{
	return m_photoDirName;
}

QStringList WidgetPhotoTranslation::photoIdList() const 
{
	return m_photoIdList;
}

bool WidgetPhotoTranslation::removePhotoDir(const QString& path)
{
	if (path.isEmpty())  
		return false;

	QDir dir(path);  
	if(!dir.exists())  
		return true;  

	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);  
	QFileInfoList fileList = dir.entryInfoList();  
	foreach (QFileInfo fi, fileList)  
	{  
		if (fi.isFile()) {
			if(!fi.dir().remove(fi.fileName())) {
				qWarning() << DEBUG_PREFIX << "remove translation photo failed" << fi.fileName();
				return false;
			}
		} else  {
			removePhotoDir(fi.absoluteFilePath());  
		}
	}  
	return dir.rmpath(dir.absolutePath());
}

void WidgetPhotoTranslation::changeEvent(QEvent* event)
{
	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void WidgetPhotoTranslation::slotFileDownloadLength(const QString& link, qint64 length)
{
	qDebug() << QString::number(length);
	if (length == -1) {
		return;
	}

	for (int i = 0; i < m_labelPhotoList.count(); i++) {
		if (link == m_labelPhotoList.at(i)->largePhotolink()) {
			m_labelPhotoList.at(i)->setLoadTotalLength(length);
			break;
		}
	}
}

void WidgetPhotoTranslation::slotFileDownloadBytes(const QString& link,qint64 bytes)
{
	static qint64 i = 0;
	i += bytes;
	qDebug() << QString::number(bytes);
	qDebug() << "Current size "  << i;

	for (int i = 0; i < m_labelPhotoList.count(); i++) {
		if (link == m_labelPhotoList.at(i)->largePhotolink()) {
			m_labelPhotoList.at(i)->setLoadedBytes(bytes);
			if (m_labelPhotoList.at(i)->cursor().shape() != Qt::ArrowCursor) {
				m_labelPhotoList.at(i)->setCursor(Qt::ArrowCursor);
			}
			break;
		}
	}
}

void WidgetPhotoTranslation::slotFileDownloaded(const QString& link, const QString& localFile)
{
	for (int i = 0; i < m_labelPhotoList.count(); i++) {
		if (link == m_labelPhotoList.at(i)->largePhotolink()) {
			m_labelPhotoList.at(i)->stopAnimation();
			m_labelPhotoList.at(i)->setCursor(Qt::PointingHandCursor);
			m_imageView->slotSetImageDir(m_photoDirName, localFile);
			break;
		}
	}
}

void WidgetPhotoTranslation::slotFileDownloadFailed(const QString& link, const QString& reason)
{
	for (int i = 0; i < m_labelPhotoList.count(); i++) {
		if (link == m_labelPhotoList.at(i)->largePhotolink()) {
			if (!m_labelPhotoList.at(i)->localFile().isEmpty())
				m_mediaService->abortDownloadFile(link, m_labelPhotoList.at(i)->localFile());
			break;
		}
	}
}

void WidgetPhotoTranslation::on_pushButtonLeft_clicked()
{
	m_ui->scrollArea->horizontalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
//	if (m_ui->scrollArea->horizontalScrollBar())
}

void WidgetPhotoTranslation::on_pushButtonRight_clicked()
{
	m_ui->scrollArea->horizontalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
}

void WidgetPhotoTranslation::startAnimation(const QString& id, const QString& link, const QString& localFile, const QString& localDir)
{
	for (int i = 0; i < m_labelPhotoList.count(); i++) {
		if (id == m_labelPhotoList.at(i)->photoId()) {
			m_labelPhotoList.at(i)->startAnimation();
			m_labelPhotoList.at(i)->setLargePhotolink(link);
			m_labelPhotoList.at(i)->setLocalFile(localFile);
			m_labelPhotoList.at(i)->setLocalDir(localDir);
			break;
		}
	}
}

} // namespace ns
