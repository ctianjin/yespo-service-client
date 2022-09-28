#include "settingsblacklistpage.h"
#include "ui_settingsblacklistpage.h"
#include "phoneapplication.h"
#include "recentlistitems_p.h"
#include "user/blacklist.h"
#include "userservice.h"
#include "photoservice.h"
#include "commandservice.h"
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <QDebug>

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "SettingsBlacklistPage:"
#endif // DEBUG_PREFIX
namespace ns {
	
struct SettingsBlacklistOptions
{	
};


SettingsBlacklistPage::SettingsBlacklistPage(QWidget* parent) :
    AbstractSettingsPage(parent),
    m_ui(new Ui_SettingsBlacklistPage),
	m_blackUserName()
{
    m_ui->setupUi(this);
	m_header = m_ui->treeWidgetBlacklist->header();
	if (m_header) {
		m_header->model()->setHeaderData(ItemBase::BlacklistLabelColumn, Qt::Horizontal, tr("Blacklist"), Qt::DisplayRole);
		m_header->model()->setHeaderData(ItemBase::BlacklistDelColumn, Qt::Horizontal, QString(), Qt::DisplayRole);
	}

	m_ui->treeWidgetBlacklist->setColumnCount(ItemBase::BlacklistMaxColumn);
	
	m_ui->treeWidgetBlacklist->setFocusPolicy(Qt::NoFocus);
	m_ui->treeWidgetBlacklist->setAttribute(Qt::WA_MacShowFocusRect, false);
	m_header->setResizeMode(ItemBase::BlacklistLabelColumn, QHeaderView::Stretch);
	m_header->setResizeMode(ItemBase::BlacklistDelColumn, QHeaderView::ResizeToContents);
}

SettingsBlacklistPage::~SettingsBlacklistPage()
{
	m_blacklistItems.clear();
}

UserService* SettingsBlacklistPage::userService() const 
{
	return m_userService;
}

void SettingsBlacklistPage::setUserService(UserService* userService)
{
	if(m_userService) {
	}

	m_userService = userService;

	if(m_userService) {

	}
}

PhotoService* SettingsBlacklistPage::photoService() const
{
	return m_photoService;
}

void SettingsBlacklistPage::setPhotoService(PhotoService* photoService)
{
	if (m_photoService) {
	}

	m_photoService = photoService;

	if (m_photoService) {
	}
}

CommandService* SettingsBlacklistPage::commandService() const
{
	return m_commandService;
}

void SettingsBlacklistPage::setCommandService(CommandService* commandService)
{
	if (m_commandService) {
		disconnect(m_commandService, SIGNAL(delblacklistFinished(const Command::DelBlacklistParam&, const Command::DelBlacklistResult&)),
			this, SLOT(slotDelBlacklistInfoFinished(const Command::DelBlacklistParam&, const Command::DelBlacklistResult&)));

		disconnect(m_commandService, SIGNAL(addblacklistFinished(const Command::AddBlacklistParam&, const Command::AddBlacklistResult&)),
			this, SLOT(slotAddBlacklistInfoFinished(const Command::AddBlacklistParam&, const Command::AddBlacklistResult&)));
	}

	m_commandService = commandService;

	if (m_commandService) {
		connect(m_commandService, SIGNAL(delblacklistFinished(const Command::DelBlacklistParam&, const Command::DelBlacklistResult&)),
			this, SLOT(slotDelBlacklistInfoFinished(const Command::DelBlacklistParam&, const Command::DelBlacklistResult&)));

		connect(m_commandService, SIGNAL(addblacklistFinished(const Command::AddBlacklistParam&, const Command::AddBlacklistResult&)),
			this, SLOT(slotAddBlacklistInfoFinished(const Command::AddBlacklistParam&, const Command::AddBlacklistResult&)));
	}
}

QString SettingsBlacklistPage::VESysId() const 
{
	QString VeSysId = m_ui->lineEditBlacklistFilter->text();
	VeSysId = VeSysId.trimmed();
	return VeSysId;
}

void SettingsBlacklistPage::retranslateTr()
{
	m_ui->treeWidgetBlacklist->header()->model()->setHeaderData(0, Qt::Horizontal, tr("Blacklist"), Qt::DisplayRole);
}

void SettingsBlacklistPage::changeEvent(QEvent* event)
{
    AbstractSettingsPage::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
			retranslateTr();
            break;
        default:
            break;
    }
}

void SettingsBlacklistPage::accept()
{
	typedef SettingsBlacklistOptions Opt;
    Preference* pref = preference();
	if (pref) {
// 		PreferenceTransaction trans(pref);
// 		
// 		trans.commit();
	}
}

void SettingsBlacklistPage::on_treeWidgetBlacklist_itemClicked(QTreeWidgetItem * item, int column)
{
	if (item && column == ItemBase::BlacklistDelColumn)	{
		
		BlacklistItem* blacklistItem = static_cast<BlacklistItem*>(item);
		if (m_commandService) {
			Command::DelBlacklistParam param;
			param.setVESysId(blacklistItem->blacklist().veSysId());
			m_commandService->delblacklist(param);
		}
	}
}

void SettingsBlacklistPage::on_pushButtonAddBlacklist_clicked()
{
	if (VESysId().isEmpty()) {
		return;
	}

	if (m_commandService) {
		Command::AddBlacklistParam param;
		param.setVESysId(VESysId());
		m_commandService->addblacklist(param);
	}
}

void SettingsBlacklistPage::showBlacklistRecord()
{
	m_ui->treeWidgetBlacklist->clear();

	if (m_userService) {
		User::BlacklistList blist = m_userService->findBlacklistRecord();
		foreach(const User::Blacklist& bl, blist) {
			BlacklistItem* newItem = new BlacklistItem(m_ui->treeWidgetBlacklist, bl);
			if (newItem) {
				newItem->setPhotoService(m_photoService);
				m_blacklistItems.insert(bl.veSysId(), newItem);
			}
		}
		QString prefix;
		int count = m_blacklistItems.count();
		prefix = tr("Blacklist");
		QString blacklistCountStr = tr("(%1/100)").arg(count);
		prefix.append(blacklistCountStr);
		m_header->model()->setHeaderData(ItemBase::BlacklistLabelColumn, Qt::Horizontal, prefix, Qt::DisplayRole);
		m_header->model()->setHeaderData(ItemBase::BlacklistDelColumn, Qt::Horizontal, QString(), Qt::DisplayRole);
	}
}

void SettingsBlacklistPage::slotDelBlacklistInfoFinished(const Command::DelBlacklistParam& param, const Command::DelBlacklistResult& result)
{
	if (result.common().error().isEmpty()) {
		if (m_userService) {
			m_userService->removeRecentById(param.VESysId());
		}

		BlacklistItem* delItem = m_blacklistItems.value(param.VESysId());
		if (delItem) {
			m_blacklistItems.remove(param.VESysId());
			delItem->deleteLater();
			delItem = NULL;
		}
		QString prefix;
		int count = m_blacklistItems.count();
		prefix = tr("Blacklist");
		QString blacklistCountStr = tr("(%1/100)").arg(count);
		prefix.append(blacklistCountStr);
		m_header->model()->setHeaderData(ItemBase::BlacklistLabelColumn, Qt::Horizontal, prefix, Qt::DisplayRole);
		m_header->model()->setHeaderData(ItemBase::BlacklistDelColumn, Qt::Horizontal, QString(), Qt::DisplayRole);
	} else {
		qWarning() << DEBUG_PREFIX << "Delete Blacklist Failed!";
	}
}

void SettingsBlacklistPage::slotAddBlacklistInfoFinished(const Command::AddBlacklistParam& param, const Command::AddBlacklistResult& result)
{
	if(result.common().error().isEmpty()) {
		if(m_userService) {
			qDebug() <<  result.id() << result.blacklistId() << result.displayName() << result.smallPhoto() << result.VESysId() << result.parterSysId();
			User::Blacklist blist;
			blist.setUniqueid(result.id());
			blist.setBlacklistId(result.blacklistId());
			blist.setDisplayName(result.displayName());
			blist.setSmallPhoto(result.smallPhoto());
			blist.setVeSysId(result.VESysId());
			blist.setPartnerSysId(result.parterSysId());
			m_userService->addBlacklistRecord(blist);

			BlacklistItem* newItem = new BlacklistItem(m_ui->treeWidgetBlacklist, blist);
			if (newItem) {
				newItem->setPhotoService(m_photoService);
				m_blacklistItems.insert(blist.veSysId(), newItem);
			}

			QString prefix;
			int count = m_blacklistItems.count();
			prefix = tr("Blacklist");
			QString blacklistCountStr = tr("(%1/100)").arg(count);
			prefix.append(blacklistCountStr);
			m_header->model()->setHeaderData(ItemBase::BlacklistLabelColumn, Qt::Horizontal, prefix, Qt::DisplayRole);
			m_header->model()->setHeaderData(ItemBase::BlacklistDelColumn, Qt::Horizontal, QString(), Qt::DisplayRole);

			if (m_photoService) {
				if (!m_photoService->isReady(result.smallPhoto())) {
					if (!m_photoService->isDownloading(result.smallPhoto())) {
						m_photoService->downloadPhoto(result.smallPhoto());
					}
				}
			}
		}
	} else {
		qWarning() << DEBUG_PREFIX << "Add Blacklist Failed!";
	}
	m_ui->lineEditBlacklistFilter->clear();
}

void SettingsBlacklistPage::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef SettingsBlacklistOptions Opt;
    Preference* pref = preference();
	QStringList path = pref->subPaths();
	if(pref) {
		
	}
}

} // namespace ns
