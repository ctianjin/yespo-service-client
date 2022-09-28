#include <QtGui/QPixmap>
#include "closemethoddialog.h"
#include "phoneapplication.h"
#include "ui_closemethoddialog.h"

namespace ns {

struct CloseMethodDialogOptions
{
	static const QString remember_choose_;
	static const QString close_window_;	
};

const QString CloseMethodDialogOptions::remember_choose_ = QLatin1String("remember_choose");
const QString CloseMethodDialogOptions::close_window_ = QLatin1String("close_window");

CloseMethodDialog::CloseMethodDialog(QWidget* parent) :
    FramelessDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_CloseMethodDialog),
	m_closwWindow(false)
{
    m_ui->setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground);	
}

CloseMethodDialog::~CloseMethodDialog()
{
}

void CloseMethodDialog::setPreference(Preference* pref)
{
	PreferenceSupport::setPreference(pref);
}

void CloseMethodDialog::accept()
{
	typedef CloseMethodDialogOptions Opt;
	Preference* pref = preference();
	if (pref) {
		PreferenceTransaction trans(pref);
		pref->putBool(Opt::remember_choose_, m_ui->checkBoxRemember->isChecked());

		int closeWindow = m_ui->radioButtonTray->isChecked()?0:1;
		pref->putInt(Opt::close_window_, closeWindow);

		if (closeWindow == 0)
			Q_EMIT showTrayIcon(true);

		trans.commit();
		m_closwWindow = closeWindow;
	}

	QDialog::accept();
}

bool CloseMethodDialog::closeWindow()
{
	return m_closwWindow;
}

void CloseMethodDialog::applyPreference(const QSet<QString>& paths, bool force)
{
	typedef CloseMethodDialogOptions Opt;
	Preference* pref = preference();
	if (pref) { 
		int closeWindow = pref->getInt(Opt::close_window_, 0);
		if (closeWindow == 0) {
			m_ui->radioButtonTray->setChecked(true);
		} else {
			m_ui->radioButtonQuit->setChecked(true);
		}
		m_ui->checkBoxRemember->setChecked(pref->getBool(Opt::remember_choose_, false));
	}
}

void CloseMethodDialog::changeEvent(QEvent* event)
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

void CloseMethodDialog::closeEvent(QCloseEvent* event)
{
	QDialog::closeEvent(event);
	reject();
}

} // namespace ns
