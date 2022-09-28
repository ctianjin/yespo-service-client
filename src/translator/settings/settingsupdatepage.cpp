#include "settingsupdatepage.h"
#include "ui_settingsupdatepage.h"
#include "phoneapplication.h"
#include <QDebug>


namespace ns {
	
struct SettingsUpdateOptions
{
	static const QString update_method_;	
};
const QString SettingsUpdateOptions::update_method_ = QLatin1String("update_method");

SettingsUpdatePage::SettingsUpdatePage(QWidget* parent) :
    AbstractSettingsPage(parent),
    m_ui(new Ui_SettingsUpdatePage)
{
    m_ui->setupUi(this);
}

SettingsUpdatePage::~SettingsUpdatePage()
{
}

void SettingsUpdatePage::changeEvent(QEvent* event)
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

void SettingsUpdatePage::accept()
{
	typedef SettingsUpdateOptions Opt;
    Preference* pref = preference();
	if (pref) {
		PreferenceTransaction trans(pref);
		int updateMethod = 0;
		if (m_ui->radioButtonAutomate->isChecked()) {
			updateMethod = 0;
		} else if(m_ui->radioButtonNoticeMe->isChecked()) {
			updateMethod = 1;
		}
		pref->putInt(Opt::update_method_, updateMethod);
		trans.commit();
	}
	
}

void SettingsUpdatePage::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef SettingsUpdateOptions Opt;
    Preference* pref = preference();
	QStringList path = pref->subPaths();
	if(pref) {
		int updateMethod = pref->getInt(Opt::update_method_, 0);
		if (updateMethod==0) {
			m_ui->radioButtonAutomate->setChecked(true);
		} else if (updateMethod==1) {
			m_ui->radioButtonNoticeMe->setChecked(true);
		}
	}
}

} // namespace ns
