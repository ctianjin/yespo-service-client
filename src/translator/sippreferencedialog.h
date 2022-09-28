#ifndef SIPPREFERENCEDIALOG_H
#define SIPPREFERENCEDIALOG_H

#include <QScopedPointer>
#include <QString>
#include <QIcon>
#include "preferencesupport.h"
#include "framelessdialog.h"

namespace ns {

class SipService;
class Ui_SipPreferenceDialog;

class SipPreferenceDialog : public FramelessDialog, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)

public:
    explicit SipPreferenceDialog(QWidget* parent = 0);
    virtual ~SipPreferenceDialog();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	virtual void accept();

Q_SIGNALS:
	void transportChanged(int transport);
	
protected:
    virtual void changeEvent(QEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);

private:
	int m_transport;

	QPointer<SipService> m_sipService;
    QScopedPointer<Ui_SipPreferenceDialog> m_ui;

    Q_DISABLE_COPY(SipPreferenceDialog) 
};

} // namespace ns

#endif // SIPPREFERENCEDIALOG_H
