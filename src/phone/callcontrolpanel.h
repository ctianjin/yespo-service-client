#ifndef QPID_CALLCONTROLPANEL_H
#define QPID_CALLCONTROLPANEL_H

#include <QWidget>
#include <QScopedPointer>
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class Ui_CallControlPanel;
struct CallControlPanelOptions;
class PopupDTMF;
class PopupVolume;

class CallControlPanel : public QWidget, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit CallControlPanel(QWidget* parent = 0);
    virtual ~CallControlPanel();

	void callIn();
	void callOut();
	void callConnected();
	void callClosed();

	void setCallVolume(int volume);
	void setCallQuality(float rxLossPercent);

Q_SIGNALS:
	void mute(bool checked);
	void speaker(bool checked);
	void digit(const QChar& c);
	void hangup();
	void answer();
	void reject();
	void volume(int vol);

public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent* event);
	

private Q_SLOTS:
	void slotDTMFClicked();

private:
    QScopedPointer<Ui_CallControlPanel> m_ui;
    QScopedPointer<CallControlPanelOptions> m_opt;
	PopupDTMF* m_DTMFPanel;
	PopupVolume* m_volumePanel;
	QString m_currentQualityImage;

    Q_DISABLE_COPY(CallControlPanel)
};

} // namespace Qpid

#endif // QPID_CALLCONTROLPANEL_H
