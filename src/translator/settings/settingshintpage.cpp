#include "settingshintpage.h"
#include "ui_settingshintpage.h"
#include "phoneapplication.h"
#include <QDebug>


namespace ns {
	
struct SettingsHintOptions
{
	static const QString close_hint_;
    static const QString call_hint_;
	static const QString message_hint_;
	
};
const QString SettingsHintOptions::close_hint_ = QLatin1String("close_hint");
const QString SettingsHintOptions::call_hint_ = QLatin1String("call_hint");
const QString SettingsHintOptions::message_hint_ = QLatin1String("message_hint");


SettingsHintPage::SettingsHintPage(QWidget* parent) :
    AbstractSettingsPage(parent),
    m_ui(new Ui_SettingsHintPage)
{
    m_ui->setupUi(this);
}

SettingsHintPage::~SettingsHintPage()
{
}

void SettingsHintPage::changeEvent(QEvent* event)
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

void SettingsHintPage::accept()
{
	typedef SettingsHintOptions Opt;
    Preference* pref = preference();
	if (pref) {
		PreferenceTransaction trans(pref);
		pref->putBool(Opt::close_hint_, m_ui->checkBoxCloseHint->isChecked());
		pref->putBool(Opt::call_hint_, m_ui->checkBoxCallPrompt->isChecked());
		pref->putBool(Opt::message_hint_, m_ui->checkBoxMessagePrompt->isChecked());
		trans.commit();
	}
	
}

void SettingsHintPage::on_checkBoxCloseHint_clicked(bool checked)
{
	m_ui->checkBoxMessagePrompt->setEnabled(!checked);
	m_ui->checkBoxCallPrompt->setEnabled(!checked);
}

void SettingsHintPage::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef SettingsHintOptions Opt;
    Preference* pref = preference();
	if(pref) {
		m_ui->checkBoxCloseHint->setChecked(pref->getBool(Opt::close_hint_, false));
		bool closeHint = pref->getBool(Opt::close_hint_, false);
		if(closeHint) {
			m_ui->checkBoxMessagePrompt->setEnabled(false);
			m_ui->checkBoxCallPrompt->setEnabled(false);
		}
		m_ui->checkBoxCallPrompt->setChecked(pref->getBool(Opt::call_hint_, false));
		m_ui->checkBoxMessagePrompt->setChecked(pref->getBool(Opt::message_hint_, false));
	}
}

} // namespace ns
