#include <QtGui/QPixmap>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QBitmap>
#include <QDebug>
#include "remotevideodialog.h"
#include "phoneapplication.h"
#include "promptdialog.h"
#include "sipservice.h"
#include "ui_remotevideodialog.h"

#define MARGIN 20

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "RemoteVideoDialog:"
#endif // DEBUG_PREFIX

static const QSize orignalSize = QSize(400, 600);

namespace ns {

const QString RemoteVideoDialog::stylesheetNetworkEnable = QLatin1String("background-image: url(:/images/ui/vs_network_enable.png);");
const QString RemoteVideoDialog::stylesheetNetworkDisable = QLatin1String("background-image: url(:/images/ui/vs_network_disable.png);");

RemoteVideoDialog::RemoteVideoDialog(QWidget* parent) :
    FramelessVideoDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_RemoteVideoDialog),
	m_promptDialog(new PromptDialog(this)),
	m_proportionW(2),
	m_proportionH(3),
	m_proportion(2),
	m_duration(0),
	m_rotation(0),
	m_rtpByteRecv(0),
	m_showFullScreen(false)
{
    m_ui->setupUi(this);

	if (!m_remoteVideoSetting) {
		m_remoteVideoSetting.reset(new RemoteVideoSetting(this));
		connect(m_remoteVideoSetting.data(), SIGNAL(rotationChanged(ns::RemoteVideoSetting::RotationStyle)), 
			this, SLOT(slotRotationChanged(ns::RemoteVideoSetting::RotationStyle)));
		connect(m_remoteVideoSetting.data(), SIGNAL(proportionChanged(ns::RemoteVideoSetting::ProportionStyle)), 
			this, SLOT(slotProportionChanged(ns::RemoteVideoSetting::ProportionStyle)));
		connect(m_remoteVideoSetting.data(), SIGNAL(brightnessChanged(int)), this, SLOT(slotBrightnessChanged(int)));
	}
	m_remoteVideoSetting->hide();

/* 	setAttribute(Qt::WA_Hover);*/
	installEventFilter(this);
	setMouseTracking(true);
 	m_ui->widgetOuter->setMouseTracking(true);
 	m_ui->widgetInner->setMouseTracking(true);
  	m_ui->widgetRemoteVideo->setMouseTracking(true);
	m_ui->pushButtonNormal->hide();
	connect(m_ui->labelRemoteVideo, SIGNAL(fullScreen(bool)), this, SLOT(slotShowFullScreen(bool)));
	m_ui->pushButtonClose->hide();
	m_ui->pushButtonMin->hide();

	m_timer = new QTimer();
	m_timer->setInterval(1000);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(slotDuration()));

	m_networkTimer = new QTimer();
	m_networkTimer->setInterval(5000);
	connect(m_networkTimer, SIGNAL(timeout()), this, SLOT(slotNetworkDetect()));
}

RemoteVideoDialog::~RemoteVideoDialog()
{
}

SipService* RemoteVideoDialog::sipService() const 
{
	return m_sipService;
}

void RemoteVideoDialog::setSipService(SipService* sipService)
{
	if (m_sipService) {
	}

	m_sipService = sipService;

	if (m_sipService) {
	}
}

QWidget* RemoteVideoDialog::widgetVideo() const 
{
	return m_ui->labelRemoteVideo;
}

void RemoteVideoDialog::setPosition(int position)
{
	m_position = position;
}

void RemoteVideoDialog::resetCommonControls()
{
	m_proportion = 2;
	m_proportionW = 2;
	m_proportionH = 3;
	m_rotation = 0;
	resize(600, 600);
	hide();
	m_ui->pushButtonMaxmize->setVisible(true);
	m_ui->pushButtonNormal->setVisible(false);
	m_ui->pushButtonMax->setVisible(true);
	m_ui->pushButtonMin->setVisible(false);
	m_ui->labelRemoteVideo->setFullScreen(false);

	if (m_remoteVideoSetting) {
		m_remoteVideoSetting->resetCommonControls();
		m_remoteVideoSetting->hide();
	}
}

void RemoteVideoDialog::closeWindow()
{
	resize(600, 600);
	hide();
	m_ui->pushButtonMaxmize->setVisible(true);
	m_ui->pushButtonNormal->setVisible(false);
	m_ui->pushButtonMax->setVisible(true);
	m_ui->pushButtonMin->setVisible(false);
	m_ui->labelRemoteVideo->setFullScreen(false);
	if (m_remoteVideoSetting)
		m_remoteVideoSetting->hide();
}

void RemoteVideoDialog::startDuration()
{
	if (m_timer && !m_timer->isActive()) {
		m_timer->start();
		m_ui->labelRemoteVideo->setGeometry((minimumWidth() - orignalSize.width()) / 2, 0 ,orignalSize.width(), minimumHeight());
	}
}

void RemoteVideoDialog::stopDuration()
{
	if (m_timer && m_timer->isActive()) {
		m_timer->stop();
		m_duration = 0;
	}
	m_ui->labelDuration->setText(QLatin1String("00:00:00"));
}

void RemoteVideoDialog::startNetworkDetect()
{
	if (m_networkTimer && !m_networkTimer->isActive()) {
		m_networkTimer->start();
	}
}

void RemoteVideoDialog::stopNetworkDetect()
{
	if (m_networkTimer && m_networkTimer->isActive()) {
		m_networkTimer->stop();
		m_rtpByteRecv = 0;
	}
}

void RemoteVideoDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
	QDialog::mouseDoubleClickEvent(event);

	showFull(!isFullScreen());
}

void RemoteVideoDialog::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    switch (event->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool RemoteVideoDialog::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent *e = static_cast<QMouseEvent *> (event);
		if (e->buttons() && Qt::LeftButton) {
			m_isLeftPressed = true;
			m_pLast = e->globalPos();
			m_curPos = countFlag(e->pos(), countRow(e->pos()));
			setCursorType(m_curPos);
		}
	}
	if (event->type() == QEvent::MouseMove) {
		QMouseEvent *e = static_cast<QMouseEvent *> (event);
		int poss=countFlag(e->pos(),countRow(e->pos()));
		setCursorType(poss);
		if(m_isLeftPressed) {
			QPoint ptemp = e->globalPos();
			ptemp = ptemp- m_pLast;
			if(m_curPos == 22 || m_curPos == 12) {
				ptemp=ptemp+pos();
				move(ptemp);
			} else {
				QRect wid = geometry();

				switch(m_curPos)
				{
					case 11:wid.setTopLeft(wid.topLeft()+ptemp);break;

					case 13:wid.setTopRight(wid.topRight()+ptemp);break;

					case 31:wid.setBottomLeft(wid.bottomLeft()+ptemp);break;

					case 33:wid.setBottomRight(wid.bottomRight()+ptemp);break;

					//case 12:wid.setTop(wid.top()+ptemp.y());break;

					case 21:wid.setLeft(wid.left()+ptemp.x());break;

					case 23:wid.setRight(wid.right()+ptemp.x());break;

					case 32:wid.setBottom(wid.bottom()+ptemp.y());break;
				}
				setGeometry(wid);
			}
			m_pLast = e->globalPos();
		}
	}
	if (event->type() == QEvent::MouseButtonRelease) {
		m_isLeftPressed = false;
		setCursor(Qt::ArrowCursor);
	}
// 	if (event->type() == QEvent::HoverMove) {
// 		QHoverEvent* e = static_cast<QHoverEvent*> (event);
// 		if (e->pos().y() >= height() - m_ui->widgetBottom->height()) {
// 			m_ui->widgetBottom->show();
// 		} else {
// 			m_ui->widgetBottom->hide();
// 		}
// 	}
	return QDialog::eventFilter(obj, event);
}

void RemoteVideoDialog::resizeEvent(QResizeEvent* event)
{
	QDialog::resizeEvent(event);

	int videoWidth, videoHeight, videoTop, videoLeft;
	if (m_proportion == 1) {
		videoTop = 0;
		videoLeft = 0;
		videoWidth = m_ui->widgetRemoteVideo->width();
		videoHeight = m_ui->widgetRemoteVideo->height();		
	} else {
		if (m_ui->widgetRemoteVideo->width() > m_ui->widgetRemoteVideo->height()) {
			videoHeight = m_ui->widgetRemoteVideo->height();
			videoWidth = videoHeight * m_proportionW / m_proportionH;
			if (videoWidth > m_ui->widgetRemoteVideo->width())
				videoWidth = m_ui->widgetRemoteVideo->width();
		} else {
			videoWidth = m_ui->widgetRemoteVideo->width();
			videoHeight = videoWidth * m_proportionH / m_proportionW;
			if (videoHeight > m_ui->widgetRemoteVideo->height())
				videoHeight = m_ui->widgetRemoteVideo->height();
		} 

		videoTop =  (m_ui->widgetRemoteVideo->height() - videoHeight) / m_proportion;
		videoLeft = (m_ui->widgetRemoteVideo->width() - videoWidth) / m_proportion;
	}

	/*m_ui->labelRemoteVideo->resize(videoWidth, videoHeight);*/
	m_ui->labelRemoteVideo->setGeometry(videoLeft, videoTop, videoWidth, videoHeight);

	if (m_remoteVideoSetting->isVisible()) {
		m_remoteVideoSetting->setGeometry(m_ui->widgetBottom->layout()->contentsMargins().left() + m_ui->widgetDuration->width() + m_ui->widgetNetwork->width() 
			+ 2 * m_ui->widgetBottom->layout()->spacing() + m_ui->pushButtonSetting->width() / 3, height() - m_remoteVideoSetting->height() - m_ui->widgetBottom->height() - 2, m_remoteVideoSetting->width(), m_remoteVideoSetting->height());
	}
}

void RemoteVideoDialog::slotShowFullScreen(bool fullScreen)
{
	if (fullScreen)
		showMaximized();
	else
		showNormal();
	m_ui->pushButtonMaxmize->setVisible(!fullScreen);
	m_ui->pushButtonMax->setVisible(!fullScreen);
	m_ui->pushButtonNormal->setVisible(fullScreen);
	m_ui->pushButtonMin->setVisible(fullScreen);
	m_ui->widgetBottom->setVisible(!fullScreen);
	m_ui->frameCaption->setVisible(!fullScreen);
}

void RemoteVideoDialog::slotMove(const QPoint& point)
{
	if (isVisible()) {
		//move(point.x() + m_position/* + 8*/, point.y()/* + 30*/);
		move(point.x() + m_position - 36, point.y() + 35);
	}
}

void RemoteVideoDialog::on_pushButtonMaxmize_clicked()
{
	//setWindowFlags(Qt::Window);
	showMaximized();
	m_ui->pushButtonMaxmize->setVisible(!isMaximized());
	m_ui->pushButtonNormal->setVisible(isMaximized());
	m_ui->pushButtonMax->setVisible(!isMaximized());
	m_ui->pushButtonMin->setVisible(isMaximized());
	m_ui->labelRemoteVideo->setFullScreen(isMaximized());
	/*m_ui->widgetBottom->setVisible(!isMaximized());*/
	/*m_ui->frameCaption->setVisible(!isMaximized());*/
}

void RemoteVideoDialog::on_pushButtonNormal_clicked()
{
	//setWindowFlags (Qt::SubWindow);
	showNormal();
	m_ui->pushButtonMaxmize->setVisible(!isMaximized());
	m_ui->pushButtonNormal->setVisible(isMaximized());
	m_ui->pushButtonMax->setVisible(!isMaximized());
	m_ui->pushButtonMin->setVisible(isMaximized());
	m_ui->labelRemoteVideo->setFullScreen(isMaximized());
	/*m_ui->widgetBottom->setVisible(!isMaximized());*/
	/*m_ui->frameCaption->setVisible(!isMaximized());*/
}

void RemoteVideoDialog::on_pushButtonMax_clicked()
{
//	setWindowFlags(Qt::Window);
	showMaximized();
	m_ui->pushButtonMaxmize->setVisible(!isMaximized());
	m_ui->pushButtonNormal->setVisible(isMaximized());
	m_ui->pushButtonMax->setVisible(!isMaximized());
	m_ui->pushButtonMin->setVisible(isMaximized());
	m_ui->labelRemoteVideo->setFullScreen(isMaximized());
//  	m_ui->widgetBottom->setVisible(!isMaximized());
//  	m_ui->frameCaption->setVisible(!isMaximized());
// 	m_showFullScreen = true;
}

void RemoteVideoDialog::on_pushButtonMin_clicked()
{
//	setWindowFlags (Qt::SubWindow);
	showNormal();

	m_ui->pushButtonMaxmize->setVisible(!isMaximized());
	m_ui->pushButtonNormal->setVisible(isMaximized());
	m_ui->pushButtonMax->setVisible(!isMaximized());
	m_ui->pushButtonMin->setVisible(isMaximized());
	m_ui->labelRemoteVideo->setFullScreen(isMaximized());
// 	m_ui->widgetBottom->setVisible(!isMaximized());
// 	m_ui->frameCaption->setVisible(!isMaximized());
// 	m_showFullScreen = false;
}

void RemoteVideoDialog::on_pushButtonSetting_clicked()
{
	if (m_remoteVideoSetting){
		if (!m_remoteVideoSetting->isVisible()) {
			m_remoteVideoSetting->show();
			m_remoteVideoSetting->setGeometry(m_ui->widgetBottom->layout()->contentsMargins().left() + m_ui->widgetDuration->width() + m_ui->widgetNetwork->width() 
				+ 2 * m_ui->widgetBottom->layout()->spacing() + m_ui->pushButtonSetting->width() / 3, height() - m_remoteVideoSetting->height() - m_ui->widgetBottom->height() - 2, m_remoteVideoSetting->width(), m_remoteVideoSetting->height());
		}	else {
			m_remoteVideoSetting->hide();
		}
	}
}

void RemoteVideoDialog::slotRotationChanged(RemoteVideoSetting::RotationStyle style)
{
	if (!m_sipService) 
		return;

	switch(style)
	{
	case RemoteVideoSetting::RotationNormal:
		m_sipService->setVideoRotation(0);
		break;
	case RemoteVideoSetting::RotationRightAngel:
		++m_rotation;
		if (m_rotation > 3)
			m_rotation = 0;
		m_sipService->setVideoRotation(m_rotation);
		if (m_proportion == 1)
			return;
		if (m_ui->labelRemoteVideo->width() >= m_ui->labelRemoteVideo->height()) {
			m_ui->labelRemoteVideo->setGeometry((m_ui->widgetRemoteVideo->width() - m_ui->labelRemoteVideo->height()) / 2, 0,
				m_ui->labelRemoteVideo->height(), m_ui->widgetRemoteVideo->height());
		} else {
			m_ui->labelRemoteVideo->setGeometry(0, (m_ui->widgetRemoteVideo->height() - m_ui->labelRemoteVideo->width()) / 2, 
				m_ui->widgetRemoteVideo->width(), m_ui->labelRemoteVideo->width());
		}
		rotationExchanged();
		break;
	case RemoteVideoSetting::RotationStraight:
		m_rotation += 2;
		if (m_rotation > 3)
			m_rotation %= 4;
		m_sipService->setVideoRotation(m_rotation);
		break;
	default:
		break;
	}
}

void RemoteVideoDialog::slotProportionChanged(RemoteVideoSetting::ProportionStyle style)
{
	switch(style)
	{
	case RemoteVideoSetting::ProportionNormal:
		m_proportionW = 2;
		m_proportionH = 3;
		m_proportion = 2;
		break;
	case  RemoteVideoSetting::Proportion1:
		m_proportionW = 16;
		m_proportionH = 9;
		m_proportion = 2;
		break;
	case RemoteVideoSetting::Proportion2:
		m_proportionW = 4;
		m_proportionH = 3;
		m_proportion = 2;
		break;
	case RemoteVideoSetting::ProportionCovered:
		m_proportionW = 1;
		m_proportionH = 1;
		m_proportion = 1;
		break;
	default:
		break;
	}

	proportionChanged(m_proportionW, m_proportionH, m_proportion);
}

void RemoteVideoDialog::proportionChanged(int proportionWidth, int proportionHeight, int proportion)
{
	int videoWidth, videoHeight, videoTop, videoLeft;
	if (m_proportion == 1) {
		videoTop = 0;
		videoLeft = 0;
		videoWidth = m_ui->widgetRemoteVideo->width();
		videoHeight = m_ui->widgetRemoteVideo->height();
	} else {
		if (m_ui->widgetRemoteVideo->width() > m_ui->widgetRemoteVideo->height()) {
			videoHeight = m_ui->widgetRemoteVideo->height();
			videoWidth = videoHeight * m_proportionW / m_proportionH;
			if (videoWidth > m_ui->widgetRemoteVideo->width())
				videoWidth = m_ui->widgetRemoteVideo->width();
		} else {
			videoWidth = m_ui->widgetRemoteVideo->width();
			videoHeight = videoWidth * m_proportionH / m_proportionW;
			if (videoHeight > m_ui->widgetRemoteVideo->height())
				videoHeight = m_ui->widgetRemoteVideo->height();
		} 

		videoTop = (m_ui->widgetRemoteVideo->height() - videoHeight) / m_proportion;
		videoLeft = (m_ui->widgetRemoteVideo->width() - videoWidth) / m_proportion;
	}

	m_ui->labelRemoteVideo->setGeometry(videoLeft, videoTop, videoWidth, videoHeight);
}

void RemoteVideoDialog::rotationExchanged()
{
	if (m_proportion != 1) {
		int proprotion;
		proprotion = m_proportionW;
		m_proportionW = m_proportionH;
		m_proportionH = proprotion;
	}
}

void RemoteVideoDialog::detectNetworkResult(quint32 byteRecv)
{
	if (byteRecv <= 0) {
		notByteRecv();
	} else if (byteRecv <= 20000) {
		byteRecvOne();
	} else if (byteRecv <= 30000) {
		byteRecvTwo();
	} else if (byteRecv <= 40000) {
		byteRecvThree();
	} else if (byteRecv <= 50000) {
		byteRecvFour();
	} else {
		byteRecvNormal();
	}
}

void RemoteVideoDialog::notByteRecv()
{
	m_ui->labelNetwork1->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork2->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork3->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork4->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork5->setStyleSheet(stylesheetNetworkDisable);
}

void RemoteVideoDialog::byteRecvOne()
{
	m_ui->labelNetwork1->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork2->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork3->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork4->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork5->setStyleSheet(stylesheetNetworkDisable);
}

void RemoteVideoDialog::byteRecvTwo()
{
	m_ui->labelNetwork1->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork2->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork3->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork4->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork5->setStyleSheet(stylesheetNetworkDisable);
}

void RemoteVideoDialog::byteRecvThree()
{
	m_ui->labelNetwork1->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork2->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork3->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork4->setStyleSheet(stylesheetNetworkDisable);
	m_ui->labelNetwork5->setStyleSheet(stylesheetNetworkDisable);
}

void RemoteVideoDialog::byteRecvFour()
{
	m_ui->labelNetwork1->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork2->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork3->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork4->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork5->setStyleSheet(stylesheetNetworkDisable);
}

void RemoteVideoDialog::byteRecvNormal()
{
	m_ui->labelNetwork1->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork2->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork3->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork4->setStyleSheet(stylesheetNetworkEnable);
	m_ui->labelNetwork5->setStyleSheet(stylesheetNetworkEnable);
}

void RemoteVideoDialog::slotBrightnessChanged(int brightness)
{
	if (m_sipService) {
		m_sipService->setVideoBrightness(brightness);
	}
}

void RemoteVideoDialog::slotDuration()
{
	++m_duration;

	const QTime currentTime((m_duration / 3600) % 60, (m_duration / 60) % 60, m_duration % 60);
	m_ui->labelDuration->setText(currentTime.toString(QLatin1String("hh:mm:ss")));
}

void RemoteVideoDialog::slotNetworkDetect()
{
	quint32 byteTotalRecv = 0;
	//unsigned int byteRecv = 0;
	if (m_sipService) {
		m_sipService->getRtpInfo(byteTotalRecv);	
	}

	quint32 currentByteRecv = byteTotalRecv - m_rtpByteRecv;

	m_rtpByteRecv = byteTotalRecv;

	qDebug() << DEBUG_PREFIX << byteTotalRecv << currentByteRecv;

	detectNetworkResult(currentByteRecv);
}

int RemoteVideoDialog::countFlag(QPoint p, int row)
{
	if(p.y() < MARGIN)
		return 10 + row;
	else if(p.y() > this->height() - MARGIN)
		return 30 + row;
	else
		return 20 + row;
}

void RemoteVideoDialog::setCursorType(int flag)
{
	Qt::CursorShape cursor;
	switch(flag)
	{
	case 11:
	case 33:
		cursor=Qt::SizeFDiagCursor;break;
	case 13:
	case 31:
		cursor=Qt::SizeBDiagCursor;break;
	case 21:
	case 23:
		cursor=Qt::SizeHorCursor;break;
	case 12:
		cursor = Qt::ArrowCursor;break;
	case 32:
		cursor=Qt::SizeVerCursor;break;
	case 22:
		cursor=Qt::ArrowCursor;break;
	default:
		cursor = Qt::ArrowCursor;break;
	}
	setCursor(cursor);
}

int RemoteVideoDialog::countRow(QPoint p)
{
	return (p.x()<MARGIN)?1:(p.x()>(this->width()-MARGIN)?3:2);
}

void RemoteVideoDialog::showFull(bool fullScreen)
{
	if (fullScreen) {
//		setWindowFlags(Qt::Window);
		showMaximized();
	}
	else {
//		setWindowFlags(Qt::SubWindow);
		showNormal();
	}
	m_ui->labelRemoteVideo->setFullScreen(fullScreen);
	m_ui->pushButtonMaxmize->setVisible(!fullScreen);
	m_ui->pushButtonNormal->setVisible(fullScreen);
	m_ui->pushButtonMax->setVisible(!fullScreen);
	m_ui->pushButtonMin->setVisible(fullScreen);
}

} // namespace ns
