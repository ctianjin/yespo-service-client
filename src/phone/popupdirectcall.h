#ifndef QPID_POPUPDIRECTCALL_H
#define QPID_POPUPDIRECTCALL_H

#include <QWidget>
#include <QScopedPointer>
#include <QStringList>
#include "preferencesupport.h"
#include "floatpanel.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

namespace Qpid {

class Ui_PopupDirectCall;
struct PopupDirectCallOptions;

class PopupDirectCall : public FloatPanel, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit PopupDirectCall(QWidget* parent = 0);
    virtual ~PopupDirectCall();

	void addNumber(const QString& number);
	void clearNumber();
	void showVoicera(bool show);
	void showInterpreterOption(bool show);
	void setInterpretation(const QString& sourceLanguage, const QString& targetLanguage);
	void setPSTNSupported(const QStringList& supported);

Q_SIGNALS:
	void callNumber(const QString& number, bool interpreter);
	void editNumber(const QString& oldPhone, const QString& phoneCountry, const QString& phone);
	void removeNumber(const QString& oldPhone);
	
public Q_SLOTS:
   

protected:
	virtual void changeEvent(QEvent * event);

private Q_SLOTS:
	void on_pushButtonEdit_1_clicked();
	void on_pushButtonEdit_2_clicked();
	void on_pushButtonEdit_3_clicked();
	void on_pushButtonEdit_4_clicked();
	void on_pushButtonEdit_5_clicked();
	void on_pushButtonAddNumber_clicked();
	void on_pushButtonOK_clicked();
	void on_pushButtonCancel_clicked();
	void on_pushButtonRemove_clicked();
	void on_textBrowserFreeLink_anchorClicked(const QUrl & link);

	void slotClickToCall();

private:
	void autoResize();
	void updateDisplay();
	void retranslateTr();

    QScopedPointer<Ui_PopupDirectCall> m_ui;
    QScopedPointer<PopupDirectCallOptions> m_opt;

	QStringList m_listNumbers;
	QString m_editNumber;
	QString m_sourceLanguage;
	QString m_targetLanguage;

    Q_DISABLE_COPY(PopupDirectCall)
};

} // namespace Qpid

#endif // QPID_POPUPDIRECTCALL_H
