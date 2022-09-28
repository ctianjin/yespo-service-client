#include "settingsdialog.h"
#include "phoneapplication.h"
#include "ui_settingsdialog.h"
#include "abstractsettingspage.h"
#include <QString>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QBitmap>
#include <QTimer>
#include "settingsgeneralpage.h"
#include "settingslanguagepage.h"
#include "settingshintpage.h"
#include "settingscallinpage.h"
#include "settingsblacklistpage.h"
#include "settingsupdatepage.h"
#include "userservice.h"
#include "photoservice.h"
#include "commandservice.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "SettingsDialog:"
#endif // DEBUG_PREFIX

namespace ns {

SettingsDialog::SettingsDialog(QWidget* parent) :
	FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_SettingsDialog)
{
    m_ui->setupUi(this);

// 	BoxShadow *boxShadow = new BoxShadow();
// 	boxShadow->setBlurRadius(35.0);
// 	boxShadow->setDistance(12.0);
// 	boxShadow->setColor(QColor(0, 0, 0, 70));
// 	setGraphicsEffect(boxShadow);

	buildIndexTree();

    setPreference(qApp->newPreference());
}

SettingsDialog::~SettingsDialog()
{
}

UserService* SettingsDialog::userService() const
{
	return m_userService;
}

void SettingsDialog::setUserService(UserService* userService) 
{
	if (m_userService) {
	}

	m_userService = userService;
	blacklistPage()->setUserService(m_userService);

	if (m_userService) {
	}
}

PhotoService* SettingsDialog::photoService() const
{
	return m_photoService;
}

void SettingsDialog::setPhotoService(PhotoService* photoService)
{
	if (m_photoService) {
	}

	m_photoService = photoService;
	blacklistPage()->setPhotoService(m_photoService);

	if (m_photoService) {
	}
}

CommandService* SettingsDialog::commandService() const
{
	return m_commandService;
}

void SettingsDialog::setCommandService(CommandService* commandService)
{
	if (m_commandService) {
	}

	m_commandService = commandService;
	blacklistPage()->setCommandService(m_commandService);

	if (m_commandService) {
 	}
}

void SettingsDialog::setPreference(Preference* pref)
{
    PreferenceSupport::setPreference(pref);

	blacklistPage()->setPreference(pref ? new Preference(pref, QLatin1String("ui.settings.blacklist")) : 0);
	generalPage()->setPreference(pref ? new Preference(pref, QLatin1String("ui.settings.general")) : 0);
	callinPage()->setPreference(pref ? new Preference(pref, QLatin1String("ui.settings.callin")) : 0);
	hintPage()->setPreference(pref ? new Preference(pref, QLatin1String("ui.settings.hint")) : 0);
	languagePage()->setPreference(pref ? new Preference(pref, QLatin1String("ui.settings.languages")) : 0);
	updatePage()->setPreference(pref ? new Preference(pref, QLatin1String("ui.settings.updatemethod")) : 0);
}

void SettingsDialog::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
			retranslateTr();
            break;
        default:
            break;
    }
}

void SettingsDialog::accept()
{
    for (int i = 0; i < m_ui->pages->count(); ++i) {
        AbstractSettingsPage* page = qobject_cast<AbstractSettingsPage*>(m_ui->pages->widget(i));
        if (page) {
            page->accept();
        }
    }

    QDialog::accept();
}

SettingsGeneralPage* SettingsDialog::generalPage() const
{
	return qobject_cast<SettingsGeneralPage*>(m_ui->pages->widget(0));
}

SettingsLanguagePage* SettingsDialog::languagePage() const
{
	return qobject_cast<SettingsLanguagePage*>(m_ui->pages->widget(1));
}

SettingsHintPage* SettingsDialog::hintPage() const
{
	return qobject_cast<SettingsHintPage*>(m_ui->pages->widget(2));
}

SettingsCallinPage* SettingsDialog::callinPage() const
{
	return qobject_cast<SettingsCallinPage*>(m_ui->pages->widget(3));
}

SettingsBlacklistPage* SettingsDialog::blacklistPage() const
{
	return qobject_cast<SettingsBlacklistPage*>(m_ui->pages->widget(4));
}

SettingsUpdatePage* SettingsDialog::updatePage() const
{
	return qobject_cast<SettingsUpdatePage*>(m_ui->pages->widget(5));
}

void SettingsDialog::retranslateTr()
{
	QTimer::singleShot(2000, this, SLOT(buildIndexTree()));
}

void SettingsDialog::buildIndexTree()
{
    QTreeWidgetItem* selectItem = 0;

    m_ui->indexTree->clear();

    for (int i = 0; i < m_ui->pages->count(); ++i) {
        QWidget* page = m_ui->pages->widget(i);
        if (!page)
            continue;
        QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->indexTree);
        item->setText(0, page->windowTitle());
		item->setData(0, Qt::DecorationRole, page->windowIcon());
        item->setData(0, Qt::UserRole, i);

        if (!selectItem) {
            selectItem = item;
        }
    }

    if (selectItem) {
        m_ui->indexTree->setCurrentItem(selectItem, 0,
            QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);
        on_pages_currentChanged(m_ui->pages->currentIndex());
    }
}



void SettingsDialog::on_indexTree_itemSelectionChanged()
{
    QTreeWidgetItem* item = m_ui->indexTree->selectedItems().value(0);
    if (item) {
        int index = item->data(0, Qt::UserRole).toInt();
        m_ui->pages->setCurrentIndex(index);
    }
}

void SettingsDialog::on_pages_currentChanged(int index)
{
    QString title;

    QWidget* page = m_ui->pages->widget(index);
    if (page) {
        title = page->windowTitle();
    }
}

void SettingsDialog::showEvent(QShowEvent *event)
{
	setVisible(true);

	if (blacklistPage()) {
		blacklistPage()->showBlacklistRecord();
	}
}

} // namespace ns
