#ifndef QPID_PHONEHISTORYVIEW_H
#define QPID_PHONEHISTORYVIEW_H

#include <QScopedPointer>
#include "abstractphoneview.h"
#include "preferencesupport.h"
#include "user/contact.h"
#include "user/phonehistory.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class SipService;
class CommandService;
class Ui_PhoneHistoryView;
struct PhoneHistoryViewOptions;

class PhoneHistoryView : public AbstractPhoneView, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)

public:
    explicit PhoneHistoryView(QWidget* parent = 0);
    virtual ~PhoneHistoryView();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	void signIn();
	void signOut();

	void showHistory(const User::Contact& contact, 
		const User::PhoneHistory& history);
	void updateContact(const User::Contact& contact);
	void setPSTNSupported(const QStringList& supported);

	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);

private Q_SLOTS:
	void slotCallNumber(const QString& number, bool interpreter);
	void slotEditNumber(const QString& oldPhone, const QString& phoneCountry, const QString& phone);
	void slotRemoveNumber(const QString& oldPhone);
	void slotRedial();
	void slotAddToContact();

	void on_pushButtonRedialHistory_clicked();
	void on_pushButtonDeleteHistory_clicked();

private:
	void retranslateTr();

    QScopedPointer<Ui_PhoneHistoryView> m_ui;
    QScopedPointer<PhoneHistoryViewOptions> m_opt;
	QPointer<SipService> m_sipService;
	QPointer<CommandService> m_commandService;

	User::Contact m_contact;
	User::PhoneHistory m_history;

    Q_DISABLE_COPY(PhoneHistoryView)
};

} // namespace Qpid

#endif // QPID_PHONEHISTORYVIEW_H
