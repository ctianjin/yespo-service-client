#include "settingsgeneralpage.h"
#include "ui_settingsgeneralpage.h"
#include "phoneapplication.h"
#include <QDebug>


namespace Qpid {
	
struct SettingsGeneralOptions
{
	static const QString host_;

    static const QString auto_login_;
	static const QString window_hide_;
	
};
const QString SettingsGeneralOptions::host_ = QLatin1String("application.host");

const QString SettingsGeneralOptions::window_hide_ = QLatin1String("application.window_hide");
const QString SettingsGeneralOptions::auto_login_ = QLatin1String("ui.login.auto_login");


SettingsGeneralPage::SettingsGeneralPage(QWidget* parent) :
    AbstractSettingsPage(parent),
    m_ui(new Ui_SettingsGeneralPage)
{
    m_ui->setupUi(this);

	connect(m_ui->hideButton, SIGNAL(toggled(bool)), this, SIGNAL(modified()));
	connect(m_ui->exitButton, SIGNAL(toggled(bool)), this, SIGNAL(modified()));

	m_ui->groupBox->hide();
}

SettingsGeneralPage::~SettingsGeneralPage()
{
}

void SettingsGeneralPage::changeEvent(QEvent* event)
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

void SettingsGeneralPage::accept()
{
	typedef SettingsGeneralOptions Opt;
    Preference* pref = preference();
	if (pref) {
		PreferenceTransaction trans(pref);
		pref->putInt(Opt::window_hide_, m_ui->hideButton->isChecked()?1:0);
		pref->putString(Opt::host_, m_ui->lineEdit->text().trimmed());
		trans.commit();
	}
	
}

void SettingsGeneralPage::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef SettingsGeneralOptions Opt;
    Preference* pref = preference();
	QStringList path = pref->subPaths();
	if(pref) {
		m_ui->lineEdit->setText(pref->getString(Opt::host_));
		int hide = pref->getInt(Opt::window_hide_, 0);
		m_ui->hideButton->setChecked(hide==1);
		m_ui->exitButton->setChecked(hide==0);
	}
}

} // namespace Qpid
