#include "settingslanguagepage.h"
#include "ui_settingslanguagepage.h"
#include "phoneapplication.h"
#include <QDebug>


namespace ns {
	
struct SettingsLanguageOptions
{
	static const QString language_;	
};

const QString SettingsLanguageOptions::language_ = QLatin1String("language");

SettingsLanguagePage::SettingsLanguagePage(QWidget* parent) :
    AbstractSettingsPage(parent),
    m_ui(new Ui_SettingsLanguagePage)
{
    m_ui->setupUi(this);
	m_ui->radioButtonThai->hide();
}

SettingsLanguagePage::~SettingsLanguagePage()
{
}

void SettingsLanguagePage::changeEvent(QEvent* event)
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

void SettingsLanguagePage::accept()
{
	typedef SettingsLanguageOptions Opt;
    Preference* pref = preference();
	if (pref) {
		QString oldLanguage = pref->getString(Opt::language_, QLatin1String("zh_CN"));

		PreferenceTransaction trans(pref);
		QString language = QLatin1String("zh_CN");
		if (m_ui->radioButtonChinese->isChecked()) {
			language = QLatin1String("zh_CN");
		} else if(m_ui->radioButtonEnglish->isChecked()) {
			language = QLatin1String("en_US");
		} else if(m_ui->radioButtonThai->isChecked()) {
			language = QLatin1String("TH");
		}
		pref->putString(Opt::language_, language);

		trans.commit();

		if (oldLanguage != language) {
			qApp->changeLanguage(language);
		}
	}
	
}

void SettingsLanguagePage::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef SettingsLanguageOptions Opt;
    Preference* pref = preference();
	if(pref) {

		QString language = pref->getString(Opt::language_, QLatin1String("zh_CN"));
		if (language == QLatin1String("zh_CN")) {
			m_ui->radioButtonChinese->setChecked(true);
		} else if(language == QLatin1String("en_US")) {
			m_ui->radioButtonEnglish->setChecked(true);
		} else if(language == QLatin1String("TH")) {
			m_ui->radioButtonThai->setChecked(true);
		}

	}
}

} // namespace ns
