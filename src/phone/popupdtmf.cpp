#include "popupdtmf.h"
#include "phoneapplication.h"
#include "ui_popupdtmf.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PopupDTMF:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PopupDTMFOptions
{

};

PopupDTMF::PopupDTMF(QWidget* parent) :
    FloatPanel(parent),
    m_ui(new Ui_PopupDTMF),
    m_opt(new PopupDTMFOptions)
{
//#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
//#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	connect(m_ui->widgetDTMFPlate, SIGNAL(DTMFClicked(const QChar&)), this, SLOT(slotAppendDigit(const QChar&)));

	QRect rect = m_ui->widgetDTMFPlate->geometry();

	m_ui->widgetDTMFPlate->setDTMFSize(QSize(rect.width()/3, rect.height()/4));
}

PopupDTMF::~PopupDTMF()
{

}

void PopupDTMF::clear()
{
	m_ui->lineEditDTMF->clear();
}

void PopupDTMF::changeEvent(QEvent* event)
{
	FloatPanel::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void PopupDTMF::slotAppendDigit(const QChar& c)
{
	updateDisplay(c);
	Q_EMIT digit(c);
}

void PopupDTMF::updateDisplay(const QChar& c)
{
	QString text = m_ui->lineEditDTMF->text();
	text += c;
	m_ui->lineEditDTMF->setText(text);
}

} // namespace Qpid
