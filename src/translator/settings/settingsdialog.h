#ifndef QPID_SETTINGSDIALOG_H
#define QPID_SETTINGSDIALOG_H

#include "framelessdialog.h"
#include <QScopedPointer>
#include "preferencesupport.h"
#include "command.h"

namespace ns {

class Ui_SettingsDialog;
class SettingsGeneralPage;
class SettingsLanguagePage;
class SettingsHintPage;
class SettingsCallinPage;
class SettingsBlacklistPage;
class SettingsUpdatePage;
class UserService;
class PhotoService;
class CommandService;

class SettingsDialog : public FramelessDialog, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(PhotoService* photoService READ photoService WRITE setPhotoService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)

public:
    explicit SettingsDialog(QWidget* parent = 0);
    virtual ~SettingsDialog();

	UserService* userService() const;
	void setUserService(UserService* userService);

	PhotoService* photoService() const;
	void setPhotoService(PhotoService* photoService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

    virtual void setPreference(Preference* pref);
    virtual void accept();

	SettingsGeneralPage* generalPage() const;
	SettingsLanguagePage* languagePage() const;
	SettingsHintPage* hintPage() const;
	SettingsCallinPage* callinPage() const;
	SettingsBlacklistPage* blacklistPage() const;
	SettingsUpdatePage* updatePage() const;

protected:
    virtual void changeEvent(QEvent* event);
	virtual void showEvent(QShowEvent *event);
//	virtual void resizeEvent(QResizeEvent* event);

private Q_SLOTS:
    void on_indexTree_itemSelectionChanged();
    void on_pages_currentChanged(int index);
	void buildIndexTree();
//	void slotDelBlacklistInfoFinished(const Command::DelBlacklistParam& param, const Command::DelBlacklistResult& result);

private:
	void retranslateTr();

	QPoint offset; 
    QScopedPointer<Ui_SettingsDialog> m_ui;
	QPointer<UserService> m_userService;
	QPointer<PhotoService> m_photoService;
	QPointer<CommandService> m_commandService;

    Q_DISABLE_COPY(SettingsDialog)
};

} // namespace ns

#endif // QPID_SETTINGSDIALOG_H
