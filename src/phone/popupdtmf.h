#ifndef QPID_POPUPDTMF_H
#define QPID_POPUPDTMF_H

#include <QWidget>
#include <QScopedPointer>
#include "preferencesupport.h"
#include "floatpanel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class Ui_PopupDTMF;
struct PopupDTMFOptions;

class PopupDTMF : public FloatPanel, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit PopupDTMF(QWidget* parent = 0);
    virtual ~PopupDTMF();

	void clear();

Q_SIGNALS:
	void digit(const QChar& c);
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);

private Q_SLOTS:
	void slotAppendDigit(const QChar& c);

private:
	void updateDisplay(const QChar& c);
    QScopedPointer<Ui_PopupDTMF> m_ui;
    QScopedPointer<PopupDTMFOptions> m_opt;

    Q_DISABLE_COPY(PopupDTMF)
};

} // namespace Qpid

#endif // QPID_POPUPDTMF_H
