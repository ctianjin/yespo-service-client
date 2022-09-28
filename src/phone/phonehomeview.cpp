#include "phonehomeview.h"
#include "phoneapplication.h"
#include "ui_phonehomeview.h"


#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneHomeView:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PhoneHomeViewOptions
{

};

PhoneHomeView::PhoneHomeView(QWidget* parent) :
    AbstractPhoneView(parent),
    m_ui(new Ui_PhoneHomeView),
    m_opt(new PhoneHomeViewOptions)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	connect(m_ui->pushButtonCall, SIGNAL(clicked()), this, SIGNAL(callClicked()));
}

PhoneHomeView::~PhoneHomeView()
{

}

void PhoneHomeView::signIn()
{
	AbstractPhoneView::signIn();
}

void PhoneHomeView::signOut()
{
	AbstractPhoneView::signOut();
}

void PhoneHomeView::changeEvent(QEvent* event)
{
	AbstractPhoneView::changeEvent(event);

	switch (event->type()) {
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);
			retranslateTr();
			break;
		default:
			break;
	}
}

void PhoneHomeView::retranslateTr()
{

}

} // namespace Qpid
