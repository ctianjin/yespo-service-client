#ifndef QPID_PHONEINTERPRETERVIEW_H
#define QPID_PHONEINTERPRETERVIEW_H

#include <QScopedPointer>
#include "abstractphoneview.h"
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class SipService;
class Ui_PhoneInterpreterView;
struct PhoneInterpreterViewOptions;

class PhoneInterpreterView : public AbstractPhoneView, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)

public:
    explicit PhoneInterpreterView(QWidget* parent = 0);
    virtual ~PhoneInterpreterView();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	void signIn();
	void signOut();

	void initialSourceLanguage();
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent* event);
	virtual void keyPressEvent(QKeyEvent *event);

private Q_SLOTS:
	void on_comboBoxSourceLanguage_currentIndexChanged(int index);
	void on_pushButtonCall_clicked();

private:
	void retranslateTr();

    QScopedPointer<Ui_PhoneInterpreterView> m_ui;
    QScopedPointer<PhoneInterpreterViewOptions> m_opt;
	QPointer<SipService> m_sipService;

    Q_DISABLE_COPY(PhoneInterpreterView)
};

} // namespace Qpid

#endif // QPID_PHONEINTERPRETERVIEW_H
