#include <QtGui/QPixmap>
#include "sippreferencedialog.h"
#include "ui_sippreferencedialog.h"
#include "sipservice.h"

namespace ns {

struct SettingsSipOptions
{
	static const QString transport_;	
};

const QString SettingsSipOptions::transport_ = QLatin1String("transport");

SipPreferenceDialog::SipPreferenceDialog(QWidget* parent) :
    FramelessDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_SipPreferenceDialog),
	m_transport(-1)
{
    m_ui->setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground);	
}

SipPreferenceDialog::~SipPreferenceDialog()
{
}

SipService* SipPreferenceDialog::sipService() const 
{
	return m_sipService;
}

void SipPreferenceDialog::setSipService(SipService* sipService)
{
	if (m_sipService) {

	}

	m_sipService = sipService;

	if (m_sipService) {

	}
}

void SipPreferenceDialog::accept()
{
	typedef SettingsSipOptions Opt;
	Preference* pref = preference();
	if (pref) {
		QString oldTransport = pref->getString(Opt::transport_, QLatin1String("udp"));

		PreferenceTransaction trans(pref);
		QString transport = QLatin1String("udp");
		if (m_ui->radioUdp->isChecked()) {
			transport = QLatin1String("udp");
			m_transport = 1;
		} else if(m_ui->radioTcp->isChecked()) {
			transport = QLatin1String("tcp");
			m_transport = 2;
		}
		pref->putString(Opt::transport_, transport);

		trans.commit();

		if (oldTransport != transport) {
			Q_EMIT transportChanged(m_transport);
		}
	}

	QDialog::accept();
}

void SipPreferenceDialog::changeEvent(QEvent* event)
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

void SipPreferenceDialog::applyPreference(const QSet<QString>& paths, bool force)
{
	typedef SettingsSipOptions Opt;
	Preference* pref = preference();
	if(pref) {

		QString transport = pref->getString(Opt::transport_, QLatin1String("udp"));
		if (transport == QLatin1String("udp")) {
			m_ui->radioUdp->setChecked(true);
			m_transport = 1;
		} else if(transport == QLatin1String("tcp")) {
			m_ui->radioTcp->setChecked(true);
			m_transport = 2;
		} 
	}
}

} // namespace ns
