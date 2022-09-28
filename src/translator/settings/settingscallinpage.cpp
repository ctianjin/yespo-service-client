#include "settingscallinpage.h"
#include "ui_settingscallinpage.h"
#include "phoneapplication.h"
#include <QDebug>


namespace ns {
	
struct SettingsCallinOptions
{
	static const QString close_callin_;
    static const QString callin_prompt_;	
};
const QString SettingsCallinOptions::close_callin_ = QLatin1String("close_callin");
const QString SettingsCallinOptions::callin_prompt_ = QLatin1String("callin_prompt");

SettingsCallinPage::SettingsCallinPage(QWidget* parent) :
    AbstractSettingsPage(parent),
    m_ui(new Ui_SettingsCallinPage)
{
    m_ui->setupUi(this);
}

SettingsCallinPage::~SettingsCallinPage()
{
}

void SettingsCallinPage::changeEvent(QEvent* event)
{
    AbstractSettingsPage::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void SettingsCallinPage::accept()
{
	typedef SettingsCallinOptions Opt;
    Preference* pref = preference();
	if (pref) {
		PreferenceTransaction trans(pref);
		bool isCheck = m_ui->checkBoxCallinPrompt->isChecked();
		pref->putBool(Opt::close_callin_, m_ui->checkBoxCallinPrompt->isChecked());
		int callinPrompt = 0;
		if (m_ui->radioButtonSlayTrayIcon->isChecked()) {
			callinPrompt = 0;
		} else if(m_ui->radioButtonDirectTrayIcon->isChecked()) {
			callinPrompt = 1;
		} else if(m_ui->radioButtonPopupCallin->isChecked()) {
			callinPrompt = 2;
		}
		pref->putInt(Opt::callin_prompt_, callinPrompt);
		
		trans.commit();
	}
	
}

void SettingsCallinPage::on_checkBoxCallinPrompt_clicked(bool checked)
{
	m_ui->radioButtonSlayTrayIcon->setEnabled(!checked);
	m_ui->radioButtonDirectTrayIcon->setEnabled(!checked);
	m_ui->radioButtonPopupCallin->setEnabled(!checked);
}

void SettingsCallinPage::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef SettingsCallinOptions Opt;
    Preference* pref = preference();
	QStringList path = pref->subPaths();
	if(pref) {
		m_ui->checkBoxCallinPrompt->setChecked(pref->getBool(Opt::close_callin_, false));
		bool closeCallin = pref->getBool(Opt::close_callin_, false);
		if(closeCallin) {
			m_ui->radioButtonSlayTrayIcon->setEnabled(false);
			m_ui->radioButtonDirectTrayIcon->setEnabled(false);
			m_ui->radioButtonPopupCallin->setEnabled(false);
		}
		int callinPrompt = pref->getInt(Opt::callin_prompt_, 0);
		if (callinPrompt==0) {
			m_ui->radioButtonSlayTrayIcon->setChecked(true);
		} else if(callinPrompt==1) {
			m_ui->radioButtonDirectTrayIcon->setChecked(true);
		} else if(callinPrompt==2) {
			m_ui->radioButtonPopupCallin->setChecked(true);
		}
	}
}

} // namespace ns
