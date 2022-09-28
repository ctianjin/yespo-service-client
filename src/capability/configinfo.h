#ifndef QPID_CONFIGINFO_H
#define QPID_CONFIGINFO_H

#include <QtCore/QObject>
#include <QtCore/QList>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

namespace ns {

class ConfigItem : public QObject
{
public:
	ConfigItem();
	ConfigItem(const ConfigItem& rhs);
	virtual ~ConfigItem();

	enum RegStatus {
		UNREGISTER = 1,
		REGISTERED = 2
	};

	enum CallStatus {
		IDLE = 0,
		RINGBACK = 1,
		ANSWERED = 2,
		INCOMMING = 3
	};

	ConfigItem& operator=(const ConfigItem& rhs);

	int accountid() const;
	void setAccountid(int accountid);
	int callid() const;
	void setCallid(int callid);
	QString callerid() const;
	QString calleeid() const;
	void setConfigId(const QString& configId);
	QString password() const;
	void setPassword(const QString& password);
	int autoAnswer() const;
	void setAutoAnswer(int autoAnswer);
	QString sourceLang() const;
	void setSourceLang(const QString& srcLang);
	QString targetLang() const;
	void setTargetLang(const QString& tarLang);
	int absoluteTime() const;
	void setAbsoluteTime(int absoluteTime);
	int autoCall() const;
	void setAutoCall(int autoCall);

	QWidget* widget() const;
	void setWidget(QWidget* widget);

	RegStatus regStatus() const;
	void setRegStatus(RegStatus status);
	int callDuration() const;
	void setCallDuration(int duration);
	CallStatus callStatus() const;
	void setCallStatus(CallStatus status); 
	int idleDuration() const;
	void setIdleDuration(int duration);

private:
	int m_accountid;
	int m_callid;
	QString m_callerid;
	QString m_calleeid;
	QString m_password;
	int m_autoAnswer;
	QString m_sourceLang;
	QString m_targetLang;
	int m_absoluteTime;
	int m_autocall;

	QWidget* m_widget;

	RegStatus m_regStatus;
	int m_duration;
	CallStatus m_status;
	int m_idleDuration;
};

class ConfigInfo : public QObject
{
    Q_OBJECT

public:
    ConfigInfo();
    virtual ~ConfigInfo();

	enum ConfigOwner {
		Caller = 1,
		Translator = 2,
		Callee = 3
	};

    bool reload(const QString& fileName);
	QString fileName() const;

	ConfigOwner owner() const;
	QString server() const;
	bool isEmpty() const;
	QList<ConfigItem>& items();

public Q_SLOTS:
    


Q_SIGNALS:
    

protected:
    

private Q_SLOTS:
    

private:
	bool import(QIODevice* device);
	ConfigOwner m_owner;
	QString m_server;
	QList<ConfigItem> m_listitem;
	QString m_fileName;
    

    Q_DISABLE_COPY(ConfigInfo)
};

} // namespace Qpid

#endif // QPID_CONFIGINFO_H
