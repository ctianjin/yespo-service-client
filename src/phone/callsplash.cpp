#include <Qdebug>
#include <QTimer>
#include <QRect>
#include <QDesktopWidget>
#include <QPalette>
#include <QColor>
#include <QResizeEvent>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QFontMetricsF>
#include "callsplash.h"
#include "ui_callsplash.h"
#include "sipservice.h"

namespace Qpid {

const int CallSplash::stepUnit = 15;

CallSplash::CallSplash(QWidget* parent) :
    QWidget(parent, Qt::SplashScreen | Qt::WindowStaysOnTopHint),
    m_ui(new Ui_CallSplash),
	m_initRect(0),
	m_slowTimer(0)
{
    m_ui->setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowOpacity(0.800000);
   	
	adjustSize();

	m_showHeight = 0;
	QRect rc = m_ui->widgetContainer->geometry();
	m_initRect = new QRect(rc.topLeft(), rc.bottomRight());

	m_slowTimer = new QTimer(this);
	m_slowTimer->setInterval(100);
	connect(m_slowTimer, SIGNAL(timeout()), this, SLOT(slotSlowShow()));

	hide();
}

CallSplash::~CallSplash()
{
	if (m_initRect) {
		delete m_initRect;
		m_initRect = NULL;
	}
}

SipService* CallSplash::sipService() const
{
	return m_sipService;
}

void CallSplash::setSipService(SipService* sipService)
{
	if (m_sipService) {
	}

	m_sipService = sipService;

	if (m_sipService) {
	}
}

void CallSplash::setCallDetails(int callID,
	const QString& callerName, 
	const QString& srcLang, 
	const QString& dstLang,
	const QString& serviceMode, 
	const QString& photo)
{
	m_callID = callID;
	QFontMetricsF ff(m_ui->labelCallerName->font());
	QString elidedText = ff.elidedText(callerName, Qt::ElideRight, m_ui->labelCallerName->geometry().width());
	m_ui->labelCallerName->setText(elidedText);
	m_ui->labelPhoto->setPixmap(photo);
// 	m_ui->labelSourceLang->setText(srcLang);
// 	m_ui->labelTargetLang->setText(dstLang);
// 	m_ui->labelServiceMode->setText(serviceMode);
}

void CallSplash::slowShow()
{
	m_showHeight = 0;
	if (m_slowTimer)
		m_slowTimer->start();
}

void CallSplash::slowHide()
{
	if (m_slowTimer)
		m_slowTimer->stop();
	hide();
}

void CallSplash::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void CallSplash::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

// 	const QSize size = event->size();
// 	QPixmap pixmap(size);
// 	QPainter painter(&pixmap);
// 	painter.fillRect(pixmap.rect(), Qt::white);
// 	painter.setBrush(Qt::black);
// 	painter.drawRoundedRect(0, 0, size.width()-2, size.height()-1, 3, 3);
// 	setMask(pixmap.createMaskFromColor(Qt::white));
}

void CallSplash::hideEvent(QHideEvent * event)
{
	m_ui->labelCallerName->setText(QString(QLatin1String("")));
}

void CallSplash::slotSlowShow()
{
	int remainHeight = m_initRect->height()-m_showHeight;
	if (remainHeight > stepUnit)
		m_showHeight += stepUnit;
	else
		m_showHeight += remainHeight;
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect screen = desktopWidget->availableGeometry();
	setGeometry(screen.right()-m_initRect->width(), screen.bottom()-m_showHeight, m_initRect->width(), m_initRect->height());
	if (m_showHeight == m_initRect->height()) {
		m_slowTimer->stop();
	}
	if (m_showHeight == stepUnit)
		show();
}

void CallSplash::on_pushButtonReject_clicked()
{
	if (m_sipService)
		m_sipService->hangupCall(m_callID);

	Q_EMIT decline();

	slowHide();
}

void CallSplash::on_pushButtonAnswer_clicked()
{
	if (m_sipService)
		m_sipService->answerCall(m_callID);

	Q_EMIT answer();

	slowHide();
}

} // namespace Qpid
