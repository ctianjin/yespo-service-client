#ifndef QPID_POPVOLUME_H
#define QPID_POPVOLUME_H

#include <QWidget>
#include <QScopedPointer>
#include "preferencesupport.h"
#include "floatpanel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class Ui_PopupVolume;
struct PopupVolumeOptions;

class PopupVolume : public FloatPanel, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit PopupVolume(QWidget* parent = 0);
    virtual ~PopupVolume();

	void setVolume(int volume);

Q_SIGNALS:
	void volumeChanged(int volume);
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);

private Q_SLOTS:
	void on_verticalSlider_valueChanged();
	

private:

    QScopedPointer<Ui_PopupVolume> m_ui;
    QScopedPointer<PopupVolumeOptions> m_opt;

    Q_DISABLE_COPY(PopupVolume)
};

} // namespace Qpid

#endif // QPID_POPVOLUME_H
