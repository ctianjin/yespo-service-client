#ifndef QPID_SETTINGSBLACKLISTPAGE_H
#define QPID_SETTINGSBLACKLISTPAGE_H

#include <QtCore/QScopedPointer>
#include "abstractsettingspage.h"
#include "command.h"

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
class QHeaderView;
QT_END_NAMESPACE

namespace ns {

class Ui_SettingsBlacklistPage;
class UserService;
class PhotoService;
class CommandService;
class BlacklistItem;
class SettingsBlacklistPage : public AbstractSettingsPage
{
    Q_OBJECT
	Q_PROPERTY(UserService* userService READ userService WRITE setUserService)
	Q_PROPERTY(PhotoService* photoService READ photoService WRITE setPhotoService)
	Q_PROPERTY(CommandService* commandService READ commandService WRITE setCommandService)

public:
    explicit SettingsBlacklistPage(QWidget* parent = 0);
    virtual ~SettingsBlacklistPage();

	UserService* userService() const;
	void setUserService(UserService* userService);

	PhotoService* photoService() const;
	void setPhotoService(PhotoService* photoService);

	CommandService* commandService() const;
	void setCommandService(CommandService* commandService);

Q_SIGNALS:

public Q_SLOTS:
	void showBlacklistRecord();
    virtual void accept();

private Q_SLOTS:
	void on_treeWidgetBlacklist_itemClicked(QTreeWidgetItem * item, int column);
	void on_pushButtonAddBlacklist_clicked();
	void slotAddBlacklistInfoFinished(const Command::AddBlacklistParam& param, const Command::AddBlacklistResult& result);
	void slotDelBlacklistInfoFinished(const Command::DelBlacklistParam& param, const Command::DelBlacklistResult& result);

protected:
    virtual void changeEvent(QEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);

private:
	QString VESysId() const;
	void retranslateTr();
	QHash<QString, BlacklistItem*>m_blacklistItems;

    QScopedPointer<Ui_SettingsBlacklistPage> m_ui;
	QPointer<UserService> m_userService;
	QPointer<PhotoService> m_photoService;
	QPointer<CommandService> m_commandService;

	QHeaderView* m_header;
	QString m_blackUserName;

    Q_DISABLE_COPY(SettingsBlacklistPage)
};

} // namespace ns

#endif // QPID_SETTINGSBLACKLISTPAGE_H
