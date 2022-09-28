#include "imagelabel.h"
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QBitmap>

namespace ns {
 
ImageLabel::ImageLabel(QWidget *parent) 
	: QLabel(parent),
	m_angel(0.0),
	m_scaleFactor(1.0),
	m_imageIndex(-1),
	m_fitWindow(true),
	m_fullScreen(false)
{
}

ImageLabel::~ImageLabel()
{

}

void ImageLabel::scaleImage(bool zoomin)
{
	if (m_pixmap.isNull())
		return;

	if (zoomin)
		m_scaleFactor *= 1.2;
	else
		m_scaleFactor /= 1.2;

	Q_EMIT scaleFactorChanged(m_scaleFactor);
}

void ImageLabel::rotateImage(bool isClockwise)
{
	if (m_pixmap.isNull())
		return;

	/*m_scaleFactor = 1.0;*/

	if (isClockwise) {
		m_angel = 90;
	} else {
		m_angel =- 90;
	}

	QMatrix matrix;
	matrix.rotate(m_angel);
	m_pixmap = m_pixmap.transformed(matrix);

/*	Q_EMIT scaleFactorChanged(m_scaleFactor);*/

	update();
}

void ImageLabel::actualSize()
{
	if (m_pixmap.isNull())
		return;
	m_scaleFactor = 1.0;
	m_fitWindow = false;
	Q_EMIT scaleFactorChanged(m_scaleFactor);
	update();
}

void ImageLabel::fitSize()
{
	if (m_pixmap.isNull())
		return;

	//m_scaleFactor = 1.0;
	m_scaleFactor = (double)width() / (double)m_pixmap.width();
	m_fitWindow = true;
	Q_EMIT scaleFactorChanged(m_scaleFactor);
	update();
}

void ImageLabel::setImageDir(const QString& dir, const QString& fileName /* = "" */)
{
	if (dir.isEmpty())
		return;

	m_imageDir.setPath(dir);
	QStringList filter;
	filter<< QLatin1String("*.jpg") << QLatin1String("*.bmp") << QLatin1String("*.jpeg") << QLatin1String("*.png") << QLatin1String("*.xpm");
	m_imageList = m_imageDir.entryList(filter, QDir::Files, QDir::Time | QDir::Reversed);

	if (fileName.isEmpty()) {
		nextImage();
	} else {
		QString name = fileName;
		QImage image(name);
		m_pixmap = QPixmap::fromImage(image);		
/*		m_imageIndex = m_imageList.size();*/
		int index = name.lastIndexOf(QLatin1String("/"));
		name = name.right(fileName.length() - index - 1);
		m_imageIndex = m_imageList.indexOf(name);
	}
}

void ImageLabel::nextImage()
{
	m_imageIndex++;
	if (m_imageIndex >= m_imageList.size()) {
		m_imageIndex = 0;
	}

	m_pixmap.load(m_imageDir.absolutePath() + QDir::separator() + m_imageList.at(m_imageIndex));

	update();
}

void ImageLabel::prevImage()
{
	m_imageIndex--;
	if (m_imageIndex < 0) {
		m_imageIndex = m_imageList.size() - 1;
	}

	m_pixmap.load(m_imageDir.absolutePath() + QDir::separator()	+ m_imageList.at(m_imageIndex));

	update();
}

double ImageLabel::scaleValue() const
{
	return m_scaleFactor;
}

QPixmap ImageLabel::getPixmap() const
{
	return m_pixmap;
}

void ImageLabel::setFitWindow(bool fit)
{
	m_fitWindow = fit;
}

void ImageLabel::resetScaleValue()
{
	m_scaleFactor = (double)width() / (double)m_pixmap.width();
	//m_scaleFactor = 1.0;
	Q_EMIT scaleFactorChanged(m_scaleFactor);
}

void ImageLabel::setFullScreen(bool full)
{
	m_fullScreen = full;
}

void ImageLabel::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
// 	if(m_angel) {
// 		QPointF center(width()/2.0,height()/2.0);
// 		painter.translate(center);
// 		painter.rotate(m_angel);
// 		painter.translate(-center);
// 	}

	if(m_fitWindow) {
		QPixmap fitPixmap = m_pixmap.scaled(width(),height(),Qt::IgnoreAspectRatio);
		painter.drawPixmap(0,0,fitPixmap);
	} else {
		painter.drawPixmap(0,0,m_pixmap);
	}
}

void ImageLabel::mousePressEvent(QMouseEvent *ev)
{
	if(ev->button() == Qt::LeftButton) {   
		setCursor(Qt::ClosedHandCursor);
		dragPos = ev->globalPos();
		leftButtonPressed = true;
	}
}

void ImageLabel::mouseMoveEvent(QMouseEvent *ev)
{
	if( ev->buttons().testFlag(Qt::LeftButton) && leftButtonPressed) {
		setCursor(Qt::ClosedHandCursor);
		move(pos() + (ev->globalPos() - dragPos));
		dragPos = ev->globalPos();
	}
}

void ImageLabel::mouseDoubleClickEvent(QMouseEvent *ev)
{
	Q_EMIT requestFullScreen(!m_fullScreen);
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *ev)
{
	leftButtonPressed = false;
	setCursor(Qt::OpenHandCursor);
}

void ImageLabel::enterEvent(QEvent *event)
{
	setCursor(Qt::OpenHandCursor);

	QLabel::enterEvent(event);
}

void ImageLabel::leaveEvent(QEvent *event)
{
	setCursor(Qt::ArrowCursor);

	QLabel::leaveEvent(event);
}

}

