#ifndef QPID_PHONECONTACTVIEW_H
#define QPID_PHONECONTACTVIEW_H

#include <QScopedPointer>
#include "abstractphoneview.h"
#include "preferencesupport.h"
#include "user/contact.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class SipService;
class CommandService;
class Ui_PhoneContactView;
struct PhoneContactViewOptions;
class InterpretationSelect;

class PhoneContactView : public AbstractPhoneView, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)

public:
    explicit PhoneContactView(QWidget* parent = 0);
    virtual ~PhoneContactView();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	void signIn();
	void signOut();

	void showContact(const User::Contact& contact, 
		const QString& nativeLanguage, 
		const QString& nickname, 
		const QString& nationality, 
		const QString& major);
	void updateContact(const User::Contact& contact);

	void setPSTNSupported(const QStringList& supported);
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);

private Q_SLOTS:
	void on_pushButtonLanguageEdit_clicked();
	void on_pushButtonDeleteContact_clicked();

	void slotCallNumber(const QString& number, bool interpreter);
	void slotEditNumber(const QString& oldPhone, const QString& phoneCountry, const QString& phone);
	void slotRemoveNumber(const QString& oldPhone);
	void slotInterpreterAdded(const QString& source, const QString& target);

private:
	void retranslateTr();

    QScopedPointer<Ui_PhoneContactView> m_ui;
    QScopedPointer<PhoneContactViewOptions> m_opt;
	QPointer<SipService> m_sipService;
	QPointer<CommandService> m_commandService;

	User::Contact m_contact;
	QString m_nativeLanguage;
	QString m_nickname;
	QString m_nationality;
	QString m_major;
	InterpretationSelect *m_interpretationSelect;

    Q_DISABLE_COPY(PhoneContactView)
};

} // namespace Qpid

#endif // QPID_PHONECONTACTVIEW_H
