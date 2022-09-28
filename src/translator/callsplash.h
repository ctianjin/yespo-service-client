#ifndef CALLSPLASH_H
#define CALLSPLASH_H

#include <QScopedPointer>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QRect;
class QPropertyAnimation;
QT_END_NAMESPACE

namespace ns {

class Ui_CallSplash;

class CallSplash : public QWidget
{
    Q_OBJECT

public:
    explicit CallSplash(QWidget* parent = 0);
    virtual ~CallSplash();

	void setCallId(int callId);
	void setLocalCountryLang(const QString& localCountry, const QString& localLanguage);
	void setLanguage(const QString& srcLang, const QString& targetLang);
	void setServiceMode(const QString& mode);
	void setRemoteContact(const QString& contact);
	void setCallerUser(const QString& callerId, const QString& photo, const QString& displayName, const QString& mobile);
	void setPhotoPicture(const QString& photoLink, const QString& photoFile);

	void slowShow(bool slide);
	void slowHide();

	int callId() const;

public Q_SLOTS:

Q_SIGNALS:
	void decline();
	void answer();

protected:
	virtual void changeEvent(QEvent* event);
	virtual void resizeEvent(QResizeEvent* event);

private Q_SLOTS:
	void on_pushButtonClose_clicked();
	void on_pushButtonReject_clicked();
	void on_pushButtonAnswer_clicked();

private:
	void retranslateTr();

	QRect* m_initRect;
	QPropertyAnimation* m_slideShow;

	int m_callId;
	bool m_local;
	QString m_sourceLang;
	QString m_targetLang;
	QString m_serviceMode;
	QString m_remoteContact;
	QString m_callerPhoto;
	QString m_displayName;
	QString m_mobile;
	QString m_localCountry;
	QString m_locallanguage;

    QScopedPointer<Ui_CallSplash> m_ui;

    Q_DISABLE_COPY(CallSplash)
};

} // namespace ns

#endif // CALLSPLASH_H
