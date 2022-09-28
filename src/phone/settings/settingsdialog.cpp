#include "settingsdialog.h"
#include "phoneapplication.h"
#include "ui_settingsdialog.h"
#include "abstractsettingspage.h"
#include <QString>
#include <QMouseEvent>
#include <QDebug>

namespace Qpid {

SettingsDialog::SettingsDialog(QWidget* parent) :
		FramelessDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_SettingsDialog)
{
    m_ui->setupUi(this);
    buildIndexTree();
	setAttribute(Qt::WA_TranslucentBackground,false);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setPreference(qApp->newPreference());
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::setPreference(Preference* pref)
{
    PreferenceSupport::setPreference(pref);

    for (int i = 0; i < m_ui->pages->count(); ++i) {
        AbstractSettingsPage* page = qobject_cast<AbstractSettingsPage*>(m_ui->pages->widget(i));
        if (page) {
            page->setPreference(pref ? new Preference(pref) : 0);
        }
    }
}

void SettingsDialog::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
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

void SettingsDialog::buildIndexTree()
{
    QTreeWidgetItem* selectItem = 0;

    m_ui->indexTree->clear();

    for (int i = 0; i < m_ui->pages->count(); ++i) {
        QWidget* page = m_ui->pages->widget(i);
        Q_ASSERT(page);
        if (!page)
            continue;

        QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->indexTree);
        item->setText(0, page->windowTitle());
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

	switch(index) {
        case 0: 
			m_ui->labelPageHeader->setText(tr("General"));
        break;

	}
}

void SettingsDialog::showEvent(QShowEvent *event)
{
	setVisible(true);
}

} // namespace Qpid
