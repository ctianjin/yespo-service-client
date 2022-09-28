#include "settingsgeneralpage.h"
#include "ui_settingsgeneralpage.h"
#include "phoneapplication.h"
#include "product.h"
#include <QDebug>
#include <QDir>
#include <QSettings>

namespace ns {
	
struct SettingsGeneralOptions
{
	static const QString auto_run_;
	static const QString save_password_;
	static const QString show_login_;
	static const QString always_top_;
	static const QString float_side_;
	static const QString show_trayicon_;
	static const QString close_window_;	
};
const QString SettingsGeneralOptions::auto_run_ = QLatin1String("auto_run");
const QString SettingsGeneralOptions::save_password_ = QLatin1String("remember");
const QString SettingsGeneralOptions::show_login_ = QLatin1String("show_login");
const QString SettingsGeneralOptions::always_top_ = QLatin1String("always_top");
const QString SettingsGeneralOptions::float_side_ = QLatin1String("float_side");
const QString SettingsGeneralOptions::show_trayicon_ = QLatin1String("show_trayicon");
const QString SettingsGeneralOptions::close_window_ = QLatin1String("close_window");

SettingsGeneralPage::SettingsGeneralPage(QWidget* parent) :
    AbstractSettingsPage(parent),
    m_ui(new Ui_SettingsGeneralPage)
{
    m_ui->setupUi(this);

	m_ui->checkBoxAlwaysLoginWindow->hide();
	m_ui->checkBoxAutoHide->hide();
	m_ui->checkBoxAlwaysOnTop->hide();
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

		QString app = qApp->applicationFilePath();
		app = app.replace(QLatin1String("/"), QLatin1String("\\")); //Need
// 		qDebug() << qApp->productApplication() << qApp->applicationName() << qApp->applicationFilePath() << app;
// 		QString appName = qApp->productApplication();
// 		QString fileName = qApp->applicationDirPath() + QDir::separator() + QLatin1String(BUILDSYS_APPLICATION_TRANSLATOR) + QLatin1String(".exe");
		PreferenceTransaction trans(pref);
		bool autorun = m_ui->checkBoxAutoRun->isChecked();
#ifdef Q_WS_WIN
		QSettings bootUpSettings(QLatin1String("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 
			QSettings::NativeFormat);
		if (autorun) {
			bootUpSettings.setValue(QLatin1String(BUILDSYS_APPLICATION_TRANSLATOR), app);
		} else {
			bootUpSettings.remove(QLatin1String(BUILDSYS_APPLICATION_TRANSLATOR));
		}
	#endif
		pref->putBool(Opt::auto_run_, autorun);

		pref->putBool(Opt::save_password_, m_ui->checkBoxSavePassword->isChecked());
		pref->putBool(Opt::show_login_, m_ui->checkBoxAlwaysLoginWindow->isChecked());

		bool onTop = m_ui->checkBoxAlwaysOnTop->isChecked();
		pref->putBool(Opt::always_top_, onTop);
		pref->putBool(Opt::float_side_, m_ui->checkBoxAutoHide->isChecked());

		bool trayIcon = m_ui->checkBoxTrayIcon->isChecked();
		pref->putBool(Opt::show_trayicon_, trayIcon);
		int closeWindow = m_ui->radioButtonMinTray->isChecked()?0:1;
		pref->putInt(Opt::close_window_, closeWindow);

		if (closeWindow==0) {
			trayIcon = true;
			pref->putBool(Opt::show_trayicon_, trayIcon);
		}

		trans.commit();

		Q_EMIT alwaysOnTop(onTop);
		Q_EMIT showTrayIcon(trayIcon);
	}
	
}

void SettingsGeneralPage::applyPreference(const QSet<QString>& paths, bool force)
{
    typedef SettingsGeneralOptions Opt;
    Preference* pref = preference();
	if(pref) {

		m_ui->checkBoxAutoRun->setChecked(pref->getBool(Opt::auto_run_, false));
		m_ui->checkBoxSavePassword->setChecked(pref->getBool(Opt::save_password_, false));
		m_ui->checkBoxAlwaysLoginWindow->setChecked(pref->getBool(Opt::show_login_, false));
		m_ui->checkBoxAlwaysOnTop->setChecked(pref->getBool(Opt::always_top_, false));
		m_ui->checkBoxAutoHide->setChecked(pref->getBool(Opt::float_side_, false));
		m_ui->checkBoxTrayIcon->setChecked(pref->getBool(Opt::show_trayicon_, false));
		int closeWindow = pref->getInt(Opt::close_window_, 0);
		m_ui->radioButtonMinTray->setChecked(closeWindow==0);
		m_ui->radioButtonExit->setChecked(closeWindow==1);

		if (closeWindow == 0) {
			m_ui->checkBoxTrayIcon->setChecked(true);
			m_ui->checkBoxTrayIcon->setEnabled(false);
		} else {
			m_ui->checkBoxTrayIcon->setEnabled(true);
		}

	}
}

void SettingsGeneralPage::on_radioButtonMinTray_clicked(bool checked)
{
	m_ui->checkBoxTrayIcon->setChecked(true);
	m_ui->checkBoxTrayIcon->setEnabled(false);
}

void SettingsGeneralPage::on_radioButtonExit_clicked(bool checked)
{
	m_ui->checkBoxTrayIcon->setEnabled(true);
}

} // namespace ns
