#ifndef QPID_CONTACTPROFILE_H
#define QPID_CONTACTPROFILE_H

#include <QWidget>
#include <QScopedPointer>
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class Ui_ContactProfile;
struct ContactProfileOptions;
class PopupDirectCall;

class ContactProfile : public QWidget, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit ContactProfile(QWidget* parent = 0);
    virtual ~ContactProfile();

	void setPhoto(const QString& photoFile);
	void setDisplayName(const QString& name);
	void setOnlineStatus(const QString& status);
	void setVoiceraID(const QString& ID);
	void setPhoneNumbers(const QString& number);
	void setInterpretation(const QString& sourceLanguage, const QString& targetLanguage);
	void setPSTNSupported(const QStringList& supported);
	void setSimpleShow(bool show);
	void setAddToContact(bool show);

Q_SIGNALS:
	void callNumber(const QString& number, bool interpreter);
	void editNumber(const QString& oldPhone, const QString& phoneCountry, const QString& phone);
	void removeNumber(const QString& oldPhone);
	void simpleCall();
	void addToContact();
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);

private Q_SLOTS:
	void on_pushButtonDirectCall_clicked();
	void on_pushButtonInterpreterCall_clicked();
	void on_pushButtonDirectCallSelection_clicked();
	void on_pushButtonInterpreterCallSelection_clicked();
	void on_pushButtonSimpleDirectCall_clicked();
	void on_pushButtonAddContact_clicked();

private:
	void retranslateTr();

    QScopedPointer<Ui_ContactProfile> m_ui;
    QScopedPointer<ContactProfileOptions> m_opt;

	PopupDirectCall* m_popupDirectCall;

    Q_DISABLE_COPY(ContactProfile)
};

} // namespace Qpid

#endif // QPID_CONTACTPROFILE_H
