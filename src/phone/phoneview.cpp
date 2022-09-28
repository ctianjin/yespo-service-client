#include <QDebug>
#include "phoneview.h"
#include "phoneapplication.h"
#include "ui_phoneview.h"
#include "userservice.h"
#include "sipservice.h"
#include "commandservice.h"
#include "mediaservice.h"
#include "callinfo.h"

#ifndef QPID_DEBUG_PREFIX
#  define QPID_DEBUG_PREFIX "PhoneView:"
#endif // QPID_DEBUG_PREFIX

namespace Qpid {

struct PhoneViewOptions
{

};

const QString PhoneView::m_selectedStyle = QLatin1String("background: url(:/images/ui/arrow_up.png) center top;background-repeat: none;");
const QString PhoneView::m_unSelectedStyle = QLatin1String("background: transparent;");

PhoneView::PhoneView(QWidget* parent) :
    AbstractPhoneView(parent),
    m_ui(new Ui_PhoneView),
    m_opt(new PhoneViewOptions),
	m_widgetBeforeCall(0)
{
#ifdef QPID_PHONE_ENABLE_TRANSLUCENT
    setAttribute(Qt::WA_TranslucentBackground);
#endif // QPID_PHONE_ENABLE_TRANSLUCENT

    m_ui->setupUi(this);

	connect(m_ui->pageHome, SIGNAL(callClicked()), this, SLOT(slotHomeCallClicked()));
}

PhoneView::~PhoneView()
{

}

UserService* PhoneView::userService() const
{
	return m_userService;
}

void PhoneView::setUserService(UserService* userService)
{
	if (m_userService) {
	}

	m_userService = userService;
	m_ui->pageCalling->setUserService(userService);
	m_ui->pageAddContact->setUserService(userService);

	if (m_userService) {
	}
}

SipService* PhoneView::sipService() const
{
	return m_sipService;
}

void PhoneView::setSipService(SipService* sipService)
{
	if (m_sipService) {}

	m_sipService = sipService;
	m_ui->pageNeedInterpreter->setSipService(m_sipService);
	m_ui->pageMakeCall->setSipService(m_sipService);
	m_ui->pageAddContact->setSipService(m_sipService);
	m_ui->pageContact->setSipService(m_sipService);
	m_ui->pageCalling->setSipService(m_sipService);
	m_ui->pageHistory->setSipService(m_sipService);

	if (m_sipService) {}
}

CommandService* PhoneView::commandService() const
{
	return m_commandService;
}

void PhoneView::setCommandService(CommandService* commandService)
{
	if (m_commandService) {}

	m_commandService = commandService;
	m_ui->pageAddContact->setCommandService(m_commandService);
	m_ui->pageContact->setCommandService(m_commandService);
	m_ui->pageHistory->setCommandService(m_commandService);

	if (m_commandService) {}
}

MediaService* PhoneView::mediaService() const
{
	return m_mediaService;
}

void PhoneView::setMediaService(MediaService* mediaService)
{
	if (m_mediaService) {
	}

	m_mediaService = mediaService;
	m_ui->pageAddContact->setMediaService(m_mediaService);

	if (m_mediaService) {
	}
}

Call* PhoneView::currentCall() const
{
	return m_currentCall;
}

void PhoneView::setCurrentCall(Call* call)
{
	m_currentCall = call;
//	m_ui->pageMakeCall->setCurrentCall(call);
}

void PhoneView::signIn()
{
	m_ui->pageHome->signIn();
	m_ui->pageMakeCall->signIn();
	m_ui->pageAddContact->signIn();
	m_ui->pageNeedInterpreter->signIn();
	m_ui->pageContact->signIn();
	m_ui->pageHistory->signIn();
	m_ui->pageCalling->signIn();

	showHomeWindow();

	AbstractPhoneView::signIn();
}

void PhoneView::signOut()
{
	m_ui->pageCalling->signOut();
	m_ui->pageHome->signOut();
	m_ui->pageMakeCall->signOut();
	m_ui->pageAddContact->signOut();
	m_ui->pageNeedInterpreter->signOut();
	m_ui->pageContact->signOut();
	m_ui->pageHistory->signOut();

	showHomeWindow();

	m_widgetBeforeCall = 0;

	AbstractPhoneView::signOut();
}

void PhoneView::showHomeWindow()
{
	on_toolButtonHome_clicked();
}

void PhoneView::showNeedInterpreterWindow()
{
	on_pushButtonNeedInterpreter_clicked();
}

void PhoneView::showCallingWindow()
{
	m_widgetBeforeCall = m_ui->stackedWidget->currentWidget();
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageCalling);
}

void PhoneView::closeCallingWindow()
{
	if (m_ui->stackedWidget->currentWidget() == m_ui->pageCalling) {
		if (m_widgetBeforeCall)
			m_ui->stackedWidget->setCurrentWidget(m_widgetBeforeCall);
		else
			m_ui->stackedWidget->setCurrentWidget(m_ui->pageHome);
	}
	m_widgetBeforeCall = NULL;
}

void PhoneView::showContactWindow(const User::Contact& contact, 
					   const QString& nativeLanguage, 
					   const QString& nickname, 
					   const QString& nationality, 
					   const QString& major)
{
	if (qApp->call()->state() != Call::CallClosed) {
		Q_EMIT callingMinimize(true);
	}
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageContact);
	m_ui->pageContact->showContact(contact, nativeLanguage, nickname, nationality, major);
}

void PhoneView::showHistoryWindow(const User::Contact& contact, 
					const User::PhoneHistory& history)
{
	if (qApp->call()->state() != Call::CallClosed) {
		Q_EMIT callingMinimize(true);
	}
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageHistory);
	m_ui->pageHistory->showHistory(contact, history);
}

void PhoneView::updateContactWindow(const User::Contact& contact)
{
	m_ui->pageContact->updateContact(contact);
	m_ui->pageHistory->updateContact(contact);
}

void PhoneView::getContactDetail(User::Contact& contact)
{
	m_ui->pageAddContact->getContactDetail(contact);
}

void PhoneView::callOut()
{
	m_ui->pageCalling->callOut();
}

void PhoneView::callIncomming()
{
	m_ui->pageCalling->callIn();
}

void PhoneView::callConnected(const QString& statusText)
{
	m_ui->pageCalling->callConnected(statusText);
}

void PhoneView::callClosed()
{
	m_ui->pageCalling->callClosed();
}

void PhoneView::callConfirmed()
{
	m_ui->pageCalling->callConfirmed();
}

void PhoneView::setUserPhoto(const QString& photo)
{
	m_ui->pageCalling->setUserPhoto(photo);
}

void PhoneView::setPSTNSupported(const QStringList& supported)
{
	m_ui->pageMakeCall->setPSTNSupported(supported);
	m_ui->pageAddContact->setPSTNSupported(supported);
	m_ui->pageContact->setPSTNSupported(supported);
	m_ui->pageHistory->setPSTNSupported(supported);
}

void PhoneView::changeEvent(QEvent* event)
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

void PhoneView::slotHomeCallClicked()
{
	on_pushButtonNeedInterpreter_clicked();
}

void PhoneView::on_toolButtonHome_clicked()
{
	if (qApp->call()->state() != Call::CallClosed) {
		Q_EMIT callingMinimize(true);
	}
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageHome);
}

void PhoneView::on_toolButtonCall_clicked()
{
	if (qApp->call()->state() != Call::CallClosed) {
		Q_EMIT callingMinimize(true);
	}
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageMakeCall);
}

void PhoneView::on_toolButtonAddContact_clicked()
{
	if (qApp->call()->state() != Call::CallClosed) {
		Q_EMIT callingMinimize(true);
	}
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageAddContact);
	m_ui->pageAddContact->resetShow();
}

void PhoneView::on_pushButtonNeedInterpreter_clicked()
{
	if (qApp->call()->state() != Call::CallClosed) {
		Q_EMIT callingMinimize(true);
	}
	m_ui->pageNeedInterpreter->initialSourceLanguage();
	m_ui->stackedWidget->setCurrentWidget(m_ui->pageNeedInterpreter);
}

void PhoneView::on_stackedWidget_currentChanged(int index)
{
	if (index != m_ui->stackedWidget->indexOf(m_ui->pageMakeCall))
		m_ui->pageMakeCall->setInterpretationVisible(false);

	QWidget *w = m_ui->stackedWidget->widget(index);

	m_ui->toolButtonHome->setChecked(w==m_ui->pageHome);
	m_ui->toolButtonCall->setChecked(w==m_ui->pageMakeCall);
	m_ui->toolButtonAddContact->setChecked(w==m_ui->pageAddContact);
	m_ui->pushButtonNeedInterpreter->setChecked(w==m_ui->pageNeedInterpreter);

	m_ui->labelHomeSelected->setStyleSheet((w==m_ui->pageHome)?m_selectedStyle:m_unSelectedStyle);
	m_ui->labelMakeCallSelected->setStyleSheet((w==m_ui->pageMakeCall)?m_selectedStyle:m_unSelectedStyle);
	m_ui->labelAddContactSelected->setStyleSheet((w==m_ui->pageAddContact)?m_selectedStyle:m_unSelectedStyle);
	m_ui->labelNeedInterpretedSelected->setStyleSheet((w==m_ui->pageNeedInterpreter)?m_selectedStyle:m_unSelectedStyle);
}

void PhoneView::retranslateTr()
{

}

} // namespace Qpid
