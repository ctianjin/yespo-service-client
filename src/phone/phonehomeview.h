#ifndef QPID_PHONEHOMEVIEW_H
#define QPID_PHONEHOMEVIEW_H

#include <QScopedPointer>
#include "abstractphoneview.h"
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class Ui_PhoneHomeView;
struct PhoneHomeViewOptions;

class PhoneHomeView : public AbstractPhoneView, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit PhoneHomeView(QWidget* parent = 0);
    virtual ~PhoneHomeView();

	void signIn();
	void signOut();

Q_SIGNALS:
	void callClicked();
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent* event);

private Q_SLOTS:

private:
	void retranslateTr();

    QScopedPointer<Ui_PhoneHomeView> m_ui;
    QScopedPointer<PhoneHomeViewOptions> m_opt;

    Q_DISABLE_COPY(PhoneHomeView)
};

} // namespace Qpid

#endif // QPID_PHONEHOMEVIEW_H
