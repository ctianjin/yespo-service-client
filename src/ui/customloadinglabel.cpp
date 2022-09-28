#include "customloadinglabel.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QTimerEvent>

#ifdef Q_WS_WIN
#include <windows.h>
#include <shellapi.h>
#endif

namespace ns {

CustomLoadingLabel::CustomLoadingLabel(QWidget* parent )
	: QLabel(parent),
	m_localFile(),
	m_photoId(-1),
	m_largePhotoLink(),
	m_smallPhotoLink(),
	m_angle(0),
	m_timerId(-1),
	m_delay(100),
	m_progress(0),
	m_displayedWhenStopped(false),
	m_color(Qt::white),
	m_bytes(0),
	m_totalLength(0),
	m_pic()
{
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFocusPolicy(Qt::NoFocus);

	resize(60, 60);

	hide();
}

CustomLoadingLabel::~CustomLoadingLabel()
{

}

void CustomLoadingLabel::uninit()
{
	m_timerId = -1;
	m_photoId = -1;
	m_totalLength = 0;
	m_bytes = 0;
	m_progress = 0;
	m_pic = NULL;
	m_localFile = QString();
	m_smallPhotoLink = QString();
	m_largePhotoLink = QString();

	hide();
}

QString CustomLoadingLabel::largePhotolink() const
{
	return m_largePhotoLink;
}

void CustomLoadingLabel::setLargePhotolink(const QString& link)
{
	m_largePhotoLink = link;
}

QString CustomLoadingLabel::smallPhotoLink() const 
{
	return m_smallPhotoLink;
}

void CustomLoadingLabel::setSmallPhotoLink(const QString& link)
{
	m_smallPhotoLink = link;
}

QString CustomLoadingLabel::localFile() const 
{
	return m_localFile;
}

void CustomLoadingLabel::setLocalFile(const QString& localFile)
{
	m_localFile = localFile;
}

QString CustomLoadingLabel::photoId() const 
{
	return m_photoId;
}

void CustomLoadingLabel::setPhotoId(const QString& id)
{
	m_photoId = id;
	if (id.toInt() != -1)
		show();
}

void CustomLoadingLabel::setLocalDir(const QString& localDir)
{
	m_localDir = localDir;
}

bool CustomLoadingLabel::isAnimated () const
{
	return (m_timerId != -1);
}

void CustomLoadingLabel::setDisplayedWhenStopped(bool state)
{
	m_displayedWhenStopped = state;

	update();
}

bool CustomLoadingLabel::isDisplayedWhenStopped() const
{
	return m_displayedWhenStopped;
}

void CustomLoadingLabel::startAnimation()
{
	m_angle = 0;

	if (m_timerId == -1)
		m_timerId = startTimer(m_delay);
}

void CustomLoadingLabel::stopAnimation()
{
	if (m_timerId != -1)
		killTimer(m_timerId);

	m_timerId = -1;

	update();
}

void CustomLoadingLabel::setAnimationDelay(int delay)
{
	if (m_timerId != -1)
		killTimer(m_timerId);

	m_delay = delay;

	if (m_timerId != -1)
		m_timerId = startTimer(m_delay);
}

void CustomLoadingLabel::setColor(const QColor & color)
{
	m_color = color;

	update();
}

QSize CustomLoadingLabel::sizeHint() const
{
	return QSize(60,60);
}

int CustomLoadingLabel::heightForWidth(int w) const
{
	return w;
}

void CustomLoadingLabel::setSmallPhoto(QPixmap pic)
{
	m_pic = pic;

	int h = m_pic.height();
	int w = m_pic.width();

	m_pic = m_pic.scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

	update();
}

void CustomLoadingLabel::setLoadTotalLength(qint64 length)
{
	m_totalLength = length;
	startAnimation();
	update();
}

void CustomLoadingLabel::setLoadedBytes(qint64 bytes)
{
	m_bytes += bytes;
	m_progress = (float)m_bytes / (float) m_totalLength * 100;
	update();
	if (m_bytes >= m_totalLength) {
		stopAnimation();
	}	
}

void CustomLoadingLabel::timerEvent(QTimerEvent * /*event*/)
{
	m_angle = (m_angle+30)%360;

	update();
}

void CustomLoadingLabel::paintEvent(QPaintEvent * /* event*/)
{
	QPainter p(this);
	p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	//p.fillRect(rect(), QColor(0,0,0));

	if (!m_pic.isNull()) {
//		p.drawPixmap(0, 0, m_pic.scaled(size()));
		p.drawPixmap(0, 0, m_pic);
		//p.drawPixmap((width() - m_pic.width()) / 2, (height() - m_pic.height()) / 2, m_pic);
	}

	if (!m_displayedWhenStopped && !isAnimated())
		return;

	int width = qMin(this->width(), this->height());

	int outerRadius = (width - (width + 30))*0.5;
	int innerRadius = (width - (width + 30))*0.5*0.38;

	int capsuleHeight = outerRadius - innerRadius;
	int capsuleWidth  = (width > 32 ) ? capsuleHeight *.23 : capsuleHeight *.35;
	int capsuleRadius = capsuleWidth/2;

	p.setPen(Qt::white);
	p.drawText(rect(), Qt::AlignCenter, QString::number(m_progress).append(QLatin1String("%")));

	for (int i = 0; i < 12; i++)
	{
		QColor color = m_color;
		color.setAlphaF(1.0f - (i/12.0f));
		p.setPen(Qt::NoPen);
		p.setBrush(color);       
		p.save();
		p.translate(rect().center());
		p.rotate(m_angle - i*30.0f);
		p.drawRoundedRect(-capsuleWidth*0.5, -(innerRadius+capsuleHeight), capsuleWidth, capsuleHeight, capsuleRadius, capsuleRadius);

		p.restore();
	}
}

void CustomLoadingLabel::mousePressEvent(QMouseEvent *event)
{
	if (m_timerId != -1)
		return;

	Q_EMIT signalClicked(m_localDir, m_localFile);

// #ifdef Q_OS_WIN
// 	QString picturePath = m_localFile;
// 	QString picturePara= QLatin1String("shimgvw.dll ImageView_Fullscreen ") + picturePath;
// 	qDebug() << "imageView:" << picturePath << picturePara;
// 	picturePath.replace(QLatin1String("/"), QLatin1String("\\"));
// 	picturePara.replace(QLatin1String("/"), QLatin1String("\\"));
// 	ShellExecuteA(NULL, NULL, "rundll32.exe", 
// 		picturePara.toUtf8().constData(),"C:\\WINDOWS\\system32", SW_SHOW);
// #endif

}

} // namespace ns
