#ifndef QPID_PHONEADDCONTACTVIEW_H
#define QPID_PHONEADDCONTACTVIEW_H

#include <QScopedPointer>
#include <QHash>
#include "abstractphoneview.h"
#include "preferencesupport.h"
#include "command.h"
#include "user/contact.h"

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE

namespace Qpid {

class UserService;
class SipService;
class CommandService;
class MediaService;
class Ui_PhoneAddContactView;
struct PhoneAddContactViewOptions;
class SearchContactItem;
class MessagePanel;

class PhoneAddContactView : public AbstractPhoneView, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)
	Q_PROPERTY(MediaService* mediaService READ mediaService WRITE setMediaService)

public:
    explicit PhoneAddContactView(QWidget* parent = 0);
    virtual ~PhoneAddContactView();

	UserService* userService() const;
	void setUserService(UserService* userService);

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

	MediaService* mediaService() const;
	void setMediaService(MediaService* mediaService);

	void signIn();
	void signOut();

	void setPSTNSupported(const QStringList& supported);
	void getContactDetail(User::Contact& contact);
	void resetShow();
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);

private Q_SLOTS:
	void slotPhoneGetUserInfoFinished(const Command::PhoneGetUserInfoParam& param, const Command::PhoneGetUserInfoResult& result);
	void slotContactPhotoDownloadFailed(const QString& link, const QString& reason);
	void slotContactPhotoDownloaded(const QString& photoLink, const QString& fileName);

	void on_pushButtonSearch_clicked();
	void on_pushButtonAdd_clicked();
	void on_pushButtonReturn_clicked();
	void on_pushButtonSaveNumber_clicked();
	void on_pushButtonCancel_clicked();
	void on_treeWidgetResult_itemClicked(QTreeWidgetItem * item, int column);
	void on_textBrowserAddNumber_anchorClicked(const QUrl & link);

private:
	void resetSearchedContact();
	void showDetails(SearchContactItem* contactItem, bool alreadyExist);
	void retranslateTr();

    QScopedPointer<Ui_PhoneAddContactView> m_ui;
    QScopedPointer<PhoneAddContactViewOptions> m_opt;
	QPointer<UserService> m_userService;
	QPointer<SipService> m_sipService;
	QPointer<CommandService> m_commandService;
	QPointer<MediaService> m_mediaService;
	QHash<QString, SearchContactItem*> m_contacts;
	MessagePanel* m_messagePanel;

    Q_DISABLE_COPY(PhoneAddContactView)
};

} // namespace Qpid

#endif // QPID_PHONEADDCONTACTVIEW_H
