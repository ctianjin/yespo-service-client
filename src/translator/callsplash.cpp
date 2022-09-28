#include <Qdebug>
#include <QRect>
#include <QDesktopWidget>
#include <QPalette>
#include <QColor>
#include <QResizeEvent>
#include <QPainter>
#include <QPixmap>
#include <QBitmap>
#include <QFontMetricsF>
#include <QPropertyAnimation>
#include "phoneapplication.h"
#include "callsplash.h"
#include "ui_callsplash.h"

namespace ns {

CallSplash::CallSplash(QWidget* parent) :
    QWidget(parent, Qt::SplashScreen | Qt::WindowStaysOnTopHint),
    m_ui(new Ui_CallSplash),
	m_initRect(0),
	m_local(false)
{
    m_ui->setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground);
   	
	adjustSize();

	QRect rc = m_ui->widgetOuter->geometry();
	m_initRect = new QRect(rc.topLeft(), rc.bottomRight());

	m_slideShow = new QPropertyAnimation(this, "geometry");
	m_slideShow->setDuration(2000);

	hide();
}

CallSplash::~CallSplash()
{
	if (m_initRect) {
		delete m_initRect;
		m_initRect = NULL;
	}
}

void CallSplash::setCallId(int callId)
{
	m_callId = callId;
}

void CallSplash::setLocalCountryLang(const QString& localCountry, const QString& localLanguage)
{
	m_local = true;
	m_ui->labelChannel->setText(tr("Channel:Local"));
	m_localCountry = localCountry;
	m_locallanguage = localLanguage;

	m_ui->labelSource->setText(m_localCountry);
	m_ui->labelTarget->setText(m_locallanguage);
//	m_ui->labelCountryLang->setText(tr("country and language"));
//	m_ui->labelCountryLangValue->setText(tr("%1/%2").arg(m_localCountry).arg(m_locallanguage));
//	m_ui->widgetLocal->show();
	m_ui->widgetInner->setStyleSheet(QLatin1String("#widgetInner {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #7abfc0, stop: 0.2 #e3f2f5);}"));
}

void CallSplash::setLanguage(const QString& srcLang, const QString& targetLang)
{
	m_local = false;
	m_ui->labelChannel->setText(tr("Channel:Translation"));
	m_sourceLang = srcLang;
	m_targetLang = targetLang;

	m_ui->labelSource->setText(qApp->languageFull(m_sourceLang));
	m_ui->labelTarget->setText(qApp->languageFull(m_targetLang));

//	m_ui->widgetTranslation->setLanguage(srcLang, targetLang);
//	m_ui->widgetTranslation->show();
	m_ui->widgetInner->setStyleSheet(QLatin1String("#widgetInner {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #9BD0F0, stop: 0.2 #EFF8FF);}"));
}

void CallSplash::setServiceMode(const QString& mode)
{
	m_serviceMode = mode;

//	m_ui->widgetTranslation->setServiceMode(mode);
}

void CallSplash::setRemoteContact(const QString& contact)
{
	m_remoteContact = contact;
}

void CallSplash::setCallerUser(const QString& callerId, const QString& photo, const QString& displayName, const QString& mobile)
{
	if (m_remoteContact.contains(callerId)) {
		m_ui->labelDisplayName->setText(displayName);
		m_ui->labelCallerInfo->setText(tr("Mobile: %1").arg(mobile));
		m_displayName = displayName;
		m_mobile = mobile;
		m_callerPhoto = photo;
	}
}

void CallSplash::setPhotoPicture(const QString& photoLink, const QString& photoFile)
{
	if (photoLink == m_callerPhoto) {
		QPixmap pic(photoFile);
		m_ui->labelPhoto->setPixmap(pic.scaled(m_ui->labelPhoto->size()));
	}
}

void CallSplash::slowShow(bool slide)
{
	m_ui->pushButtonAnswer->setEnabled(true);
	m_ui->pushButtonReject->setEnabled(true);

	QRect screen = QApplication::desktop()->availableGeometry();
	if (slide) {
		setGeometry(screen.right()-m_initRect->width(), screen.bottom(), m_initRect->width(), m_initRect->height());
		show();
		m_slideShow->setStartValue(QRect(screen.right()-m_initRect->width(), screen.bottom(), m_initRect->width(), m_initRect->height()));
		m_slideShow->setEndValue(QRect(screen.right()-m_initRect->width(), screen.bottom()-m_initRect->height(), m_initRect->width(), m_initRect->height()));
		m_slideShow->start();
	} else {
		setGeometry(screen.right()-m_initRect->width(), screen.bottom()-m_initRect->height(), m_initRect->width(), m_initRect->height());
		show();
	}
}

void CallSplash::slowHide()
{
	if (m_slideShow && m_slideShow->state()!=QPropertyAnimation::Stopped) {
		m_slideShow->stop();
	}
	hide();
}

int CallSplash::callId() const 
{
	return m_callId;
}

void CallSplash::changeEvent(QEvent* event)
{
    QWidget::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
			retranslateTr();
            break;
        default:
            break;
    }
}

void CallSplash::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);

	const QSize size = event->size();
	QPixmap pixmap(size);
	QPainter painter(&pixmap);
	painter.fillRect(pixmap.rect(), Qt::white);
	painter.setBrush(Qt::black);
	painter.drawRoundedRect(0, 0, size.width(), size.height(), 3, 3);
	setMask(pixmap.createMaskFromColor(Qt::white));
}

void CallSplash::on_pushButtonClose_clicked()
{
	slowHide();
}

void CallSplash::on_pushButtonReject_clicked()
{
	m_ui->pushButtonAnswer->setEnabled(false);
	m_ui->pushButtonReject->setEnabled(false);
	Q_EMIT decline();

	slowHide();
}

void CallSplash::on_pushButtonAnswer_clicked()
{
	m_ui->pushButtonAnswer->setEnabled(false);
	m_ui->pushButtonReject->setEnabled(false);
	Q_EMIT answer();

	slowHide();
}

void CallSplash::retranslateTr()
{
	m_ui->labelDisplayName->setText(m_displayName);
	m_ui->labelCallerInfo->setText(tr("Mobile: %1").arg(m_mobile));
	if (m_local) {
		m_ui->labelSource->setText(m_localCountry);
		m_ui->labelTarget->setText(m_locallanguage);
	} else {
		m_ui->labelSource->setText(qApp->languageFull(m_sourceLang));
		m_ui->labelTarget->setText(qApp->languageFull(m_targetLang));
	}

//	m_ui->labelCountryLang->setText(tr("country and language"));

//	m_ui->labelCountryLangValue->setText(tr("%1/%2").arg(m_localCountry).arg(m_locallanguage));
}

} // namespace ns
