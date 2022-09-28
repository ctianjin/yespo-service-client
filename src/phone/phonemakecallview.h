#ifndef QPID_PHONEMAKECALLVIEW_H
#define QPID_PHONEMAKECALLVIEW_H

#include <QScopedPointer>
#include "abstractphoneview.h"
#include "preferencesupport.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class SipService;
class Ui_PhoneMakeCallView;
struct PhoneMakeCallViewOptions;
class InterpretationSelect;
//class Call;

class PhoneMakeCallView : public AbstractPhoneView, public PreferenceSupport
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)
//	Q_PROPERTY(Call* currentCall READ currentCall WRITE setCurrentCall)

public:
    explicit PhoneMakeCallView(QWidget* parent = 0);
    virtual ~PhoneMakeCallView();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

// 	Call* currentCall() const;
// 	void setCurrentCall(Call* call);

	void signIn();
	void signOut();

	void setPSTNSupported(const QStringList& supported);
	void setInterpretationVisible(bool visible);
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);
	virtual void keyPressEvent(QKeyEvent *event);

private Q_SLOTS:
	void on_pushButtonDelete_clicked();
	void on_pushButtonAdd_clicked();
	void on_pushButtonChange_clicked();
	void on_pushButtonRemove_clicked();
	void on_pushButtonMakeCall_clicked();

	void slotInterpreterAdded(const QString& source, const QString& target);
	void slotAppendDigit(const QChar& c);

private:
	void retranslateTr();

    QScopedPointer<Ui_PhoneMakeCallView> m_ui;
    QScopedPointer<PhoneMakeCallViewOptions> m_opt;
	QPointer<SipService> m_sipService;
//	QPointer<Call> m_currentCall;
//	Call* m_currentCall;

	InterpretationSelect* m_interpretationSelect;
	QString m_sourceLanguage;
	QString m_targetLanguage;

    Q_DISABLE_COPY(PhoneMakeCallView)
};

} // namespace Qpid

#endif // QPID_PHONEMAKECALLVIEW_H
