#include <QtCore/QSharedData>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include "command.h"

namespace ns {
namespace Command {

// CommonResult
class CommonResultPrivate : public QSharedData
{
public:
	CommonResultPrivate();
	CommonResultPrivate(const CommonResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;
	QString m_errorNum;
	QString m_errorTips;
};

CommonResultPrivate::CommonResultPrivate() : 
	QSharedData(),
	m_error(),
	m_errorMsg(),
	m_errorNum(),
	m_errorTips()
{
}

CommonResultPrivate::CommonResultPrivate(const CommonResultPrivate& rhs) : 
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg),
	m_errorNum(rhs.m_errorNum),
	m_errorTips(rhs.m_errorTips)
{
}

CommonResult::CommonResult() :
	d(new CommonResultPrivate)
{
}

CommonResult::CommonResult(const CommonResult& rhs) : 
	d(rhs.d)
{
}

CommonResult::~CommonResult()
{
}

CommonResult& CommonResult::operator=(const CommonResult& rhs)
{
	d = rhs.d;
	return *this;
}

QString CommonResult::error() const
{
	if (d->m_error == QLatin1String("0"))
		return QString();
	else
		return d->m_error;
}

void CommonResult::setError(const QString& err)
{
	d->m_error = err;
}

QString CommonResult::errorMsg() const
{
	return d->m_errorMsg;
}

void CommonResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

QString CommonResult::errorNum() const
{
	return d->m_errorNum;
}

void CommonResult::setErrorNum(const QString& errNum)
{
	d->m_errorNum = errorNum();
}

QString CommonResult::errorTips() const
{
	return d->m_errorTips;
}

void CommonResult::setErrorTips(const QString& tips)
{
	d->m_errorTips = tips;
}

// LoadConfigParam
class LoadConfigParamPrivate : public QSharedData
{
public:
	LoadConfigParamPrivate();
	LoadConfigParamPrivate(const LoadConfigParamPrivate& rhs);

	QString m_lastTime;
	int m_reload;
};

LoadConfigParamPrivate::LoadConfigParamPrivate() :
	QSharedData(),
	m_lastTime(),
	m_reload(0)
{
}

LoadConfigParamPrivate::LoadConfigParamPrivate(const LoadConfigParamPrivate& rhs) :
	QSharedData(rhs),
	m_lastTime(rhs.m_lastTime),
	m_reload(rhs.m_reload)
{
}

LoadConfigParam::LoadConfigParam() :
	d(new LoadConfigParamPrivate)
{
}

LoadConfigParam::LoadConfigParam(const LoadConfigParam& rhs) :
	d(rhs.d)
{
}

LoadConfigParam::~LoadConfigParam()
{
}

LoadConfigParam& LoadConfigParam::operator=(const LoadConfigParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString LoadConfigParam::lastTime() const
{
	return d->m_lastTime;
}

void LoadConfigParam::setLastTime(const QString& lastTime)
{
	d->m_lastTime = lastTime;
}

int LoadConfigParam::reloadConfig() const
{
	return d->m_reload;
}

void LoadConfigParam::setReloadConfig(int reload)
{
	d->m_reload = reload;
}

bool LoadConfigParam::operator==(const LoadConfigParam& rhs) const
{
	return d->m_lastTime == rhs.d->m_lastTime
		&& d->m_reload == rhs.d->m_reload;
}

bool LoadConfigParam::operator!=(const LoadConfigParam& rhs) const
{
	return !operator==(rhs);
}


// LoadConfigResult
class LoadConfigResultPrivate : public QSharedData
{
public:
	LoadConfigResultPrivate();
	LoadConfigResultPrivate(const LoadConfigResultPrivate& rhs);

	CommonResult m_common;
	QVariant m_config;
	QVariant m_client;
};

LoadConfigResultPrivate::LoadConfigResultPrivate() :
	QSharedData(),
	m_config(),
	m_client()
{
}

LoadConfigResultPrivate::LoadConfigResultPrivate(const LoadConfigResultPrivate& rhs) :
	QSharedData(rhs),
	m_config(rhs.m_config),
	m_client(rhs.m_client)
{
}

LoadConfigResult::LoadConfigResult() :
	d(new LoadConfigResultPrivate)
{
}

LoadConfigResult::LoadConfigResult(const LoadConfigResult& rhs) :
	d(rhs.d)
{
}

LoadConfigResult::~LoadConfigResult()
{
}

LoadConfigResult& LoadConfigResult::operator=(const LoadConfigResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult LoadConfigResult::common() const
{
	return d->m_common;
}

void LoadConfigResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QVariant LoadConfigResult::config() const
{
	return d->m_config;
}

void LoadConfigResult::setConfig(const QVariant& result)
{
	d->m_config = result;
}

QVariant LoadConfigResult::client() const
{
	return d->m_client;
}

void LoadConfigResult::setClient(const QVariant& result)
{
	d->m_client = result;
}

// LoginParam
class LoginParamPrivate : public QSharedData
{
public:
    LoginParamPrivate();
    LoginParamPrivate(const LoginParamPrivate& rhs);

    QString m_userName;
    QString m_password;
};

LoginParamPrivate::LoginParamPrivate() :
    QSharedData(),
    m_userName(),
    m_password()
{
}

LoginParamPrivate::LoginParamPrivate(const LoginParamPrivate& rhs) :
    QSharedData(rhs),
    m_userName(rhs.m_userName),
    m_password(rhs.m_password)
{
}

LoginParam::LoginParam() :
    d(new LoginParamPrivate)
{
}

LoginParam::LoginParam(const LoginParam& rhs) :
    d(rhs.d)
{
}

LoginParam::~LoginParam()
{
}

LoginParam& LoginParam::operator=(const LoginParam& rhs)
{
    d = rhs.d;
    return *this;
}

QString LoginParam::userName() const
{
    return d->m_userName;
}

void LoginParam::setUserName(const QString& userName)
{
    d->m_userName = userName;
}

QString LoginParam::password() const
{
    return d->m_password;
}

void LoginParam::setPassword(const QString& password)
{
    d->m_password = password;
}

bool LoginParam::operator==(const LoginParam& rhs) const
{
    return d->m_userName == rhs.d->m_userName
        && d->m_password == rhs.d->m_password;
}

bool LoginParam::operator!=(const LoginParam& rhs) const
{
    return !operator==(rhs);
}

// LoginResult
class LoginResultPrivate : public QSharedData
{
public:
    LoginResultPrivate();
    LoginResultPrivate(const LoginResultPrivate& rhs);

	CommonResult m_common;
	QString m_systemId;
    QString m_callerId;
	QString m_loginId;
	QString m_secret;
	QString m_displayName;
	QStringList m_skill;
	QStringList m_local;
	QStringList m_sipServerList;
	QString m_statusServiceHost;
	QString m_sipServiceHost;
	QString m_imServiceHost;
	QString m_smallPhoto;
	QString m_largePhoto;
	QString m_token;
};

LoginResultPrivate::LoginResultPrivate() :
    QSharedData(),
	m_common(),
	m_systemId(0),
	m_callerId(),
    m_loginId(),
	m_secret(),
	m_displayName(),
	m_skill(),
	m_local(),
	m_sipServerList(),
	m_statusServiceHost(),
	m_sipServiceHost(),
	m_imServiceHost(),
	m_smallPhoto(),
	m_largePhoto(),
	m_token()
{
}

LoginResultPrivate::LoginResultPrivate(const LoginResultPrivate& rhs) :
    QSharedData(rhs),
	m_common(rhs.m_common),
	m_systemId(rhs.m_systemId),
    m_callerId(rhs.m_callerId),
    m_loginId(rhs.m_loginId),
    m_secret(rhs.m_secret),
    m_displayName(rhs.m_displayName),
	m_skill(rhs.m_skill),
	m_local(rhs.m_local),
	m_sipServerList(rhs.m_sipServerList),
	m_statusServiceHost(rhs.m_statusServiceHost),
	m_sipServiceHost(rhs.m_sipServiceHost),
	m_imServiceHost(rhs.m_imServiceHost),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto),
	m_token(rhs.m_token)
{
}

LoginResult::LoginResult() :
    d(new LoginResultPrivate)
{
}

LoginResult::LoginResult(const LoginResult& rhs) :
    d(rhs.d)
{
}

LoginResult::~LoginResult()
{
}

LoginResult& LoginResult::operator=(const LoginResult& rhs)
{
    d = rhs.d;
    return *this;
}

CommonResult LoginResult::common() const
{
	return d->m_common;
}

void LoginResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QString LoginResult::systemId() const
{
	return d->m_systemId;
}

void LoginResult::setSystemId(const QString& Id)
{
	d->m_systemId = Id;
}

QString LoginResult::callerId() const
{
	return d->m_callerId;
}

void LoginResult::setCallerId(const QString& Id)
{
	d->m_callerId = Id;
}

QString LoginResult::loginId() const
{
	return d->m_loginId;
}

void LoginResult::setLoginId(const QString& Id)
{
	d->m_loginId = Id;
}

QString LoginResult::secret() const
{
	return d->m_secret;
}

void LoginResult::setSecret(const QString& secret)
{
	d->m_secret = secret;
}

QString LoginResult::displayName() const
{
	return d->m_displayName;
}

void LoginResult::setDisplayName(const QString& name)
{
	d->m_displayName = name;
}

QStringList LoginResult::skill() const
{
	return d->m_skill;
}

void LoginResult::setSkill(const QStringList& skill)
{
	d->m_skill = skill;
}

QStringList LoginResult::local() const 
{
	return d->m_local;
}

void LoginResult::setLocal(const QStringList& local)
{
	d->m_local = local;
}

QStringList LoginResult::sipServerList() const
{
	return d->m_sipServerList;
}

void LoginResult::setSipServerList(const QStringList& list)
{
	d->m_sipServerList = list;
}

QString LoginResult::statusServiceHost() const 
{
	return d->m_statusServiceHost;
}

void LoginResult::setStatusServiceHost(const QString& statusServiceHost) 
{
	d->m_statusServiceHost = statusServiceHost;
}

QString LoginResult::sipServiceHost() const
{
	return d->m_sipServiceHost;
}

void LoginResult::setSipServiceHost(const QString& sipServiceHost)
{
	d->m_sipServiceHost = sipServiceHost;
}

QString LoginResult::imServiceHost() const 
{
	return d->m_imServiceHost;
}

void LoginResult::setImServiceHost(const QString& imServiceHost)
{
	d->m_imServiceHost = imServiceHost;
}

QString LoginResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void LoginResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString LoginResult::largePhoto() const
{
	return d->m_largePhoto;
}

void LoginResult::setLargePhoto(const QString& photo)
{
	d->m_largePhoto = photo;
}

QString LoginResult::token() const 
{
	return d->m_token;
}

void LoginResult::setToken(const QString& token)
{
	d->m_token = token;
}

//SetClientInfoParam
class SetClientInfoParamPrivate : public QSharedData
{
public:
	SetClientInfoParamPrivate();
	SetClientInfoParamPrivate(const SetClientInfoParamPrivate& rhs);

	QString m_lastDomainCall;
};

SetClientInfoParamPrivate::SetClientInfoParamPrivate() :
	QSharedData(),
	m_lastDomainCall()
{

}

SetClientInfoParamPrivate::SetClientInfoParamPrivate(const SetClientInfoParamPrivate& rhs) : 
	QSharedData(rhs),
	m_lastDomainCall(rhs.m_lastDomainCall)
{

}

SetClientInfoParam::SetClientInfoParam() :
	d(new SetClientInfoParamPrivate)
{
}

SetClientInfoParam::SetClientInfoParam(const SetClientInfoParam& rhs) :
	d(rhs.d)
{
}

SetClientInfoParam::~SetClientInfoParam()
{
}

SetClientInfoParam& SetClientInfoParam::operator=(const SetClientInfoParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString SetClientInfoParam::lastDomainCall() const
{
	return d->m_lastDomainCall;
}

void SetClientInfoParam::setLastDomainCall(const QString& domainCall)
{
	d->m_lastDomainCall = domainCall;
}

bool SetClientInfoParam::operator==(const SetClientInfoParam& rhs) const
{
	return d->m_lastDomainCall == rhs.d->m_lastDomainCall;
}

bool SetClientInfoParam::operator!=(const SetClientInfoParam& rhs) const
{
	return !operator==(rhs);
}

//SetClientInfoResult
class SetClientInfoResultPrivate : public QSharedData
{
public:
	SetClientInfoResultPrivate();
	SetClientInfoResultPrivate(const SetClientInfoResultPrivate& rhs);

	CommonResult m_common;
};

SetClientInfoResultPrivate::SetClientInfoResultPrivate() :
	QSharedData()
{

}

SetClientInfoResultPrivate::SetClientInfoResultPrivate(const SetClientInfoResultPrivate& rhs) :
	QSharedData(rhs),
	m_common(rhs.m_common)
{

}

SetClientInfoResult::SetClientInfoResult() : 
	d(new SetClientInfoResultPrivate)
{

}

SetClientInfoResult::SetClientInfoResult(const SetClientInfoResult& rhs) : 
	d(rhs.d)
{

}

SetClientInfoResult::~SetClientInfoResult()
{

}

SetClientInfoResult& SetClientInfoResult::operator=(const SetClientInfoResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult SetClientInfoResult::common() const
{
	return d->m_common;
}

void SetClientInfoResult::setCommon(const CommonResult& result) 
{
	d->m_common = result;
}

//GetServerParam
class GetServerParamPrivate : public QSharedData
{
public:
	GetServerParamPrivate();
	GetServerParamPrivate(const GetServerParamPrivate& rhs);

};

GetServerParamPrivate::GetServerParamPrivate() :
	QSharedData()
{

}


GetServerParamPrivate::GetServerParamPrivate(const GetServerParamPrivate& rhs) :
	QSharedData(rhs)
{

}

GetServerParam::GetServerParam() :
	d(new GetServerParamPrivate)
{

}

GetServerParam::GetServerParam(const GetServerParam& rhs) : 
	d(rhs.d)
{

}

GetServerParam::~GetServerParam()
{

}

GetServerParam& GetServerParam::operator=(const GetServerParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool GetServerParam::operator==(const GetServerParam& rhs) const
{
	return false;
}

bool GetServerParam::operator!=(const GetServerParam& rhs) const
{
	return !operator==(rhs);
}

//GetServerResult
class GetServerResultPrivate : public QSharedData
{
public:
	GetServerResultPrivate();
	GetServerResultPrivate(const GetServerResultPrivate& rhs);

	CommonResult m_common;
	QStringList m_sipServerList;
	QString m_domainStatus;
	QString m_domainCall;
	QString m_domainIM;
};

GetServerResultPrivate::GetServerResultPrivate() : 
	QSharedData(),
	m_common(),
	m_sipServerList(),
	m_domainStatus(),
	m_domainCall(),
	m_domainIM()
{

}

GetServerResultPrivate::GetServerResultPrivate(const GetServerResultPrivate& rhs) : 
	QSharedData(rhs),
	m_common(rhs.m_common),
	m_sipServerList(rhs.m_sipServerList),
	m_domainStatus(rhs.m_domainStatus),
	m_domainCall(rhs.m_domainCall),
	m_domainIM(rhs.m_domainIM)
{

}

GetServerResult::GetServerResult() : 
	d(new GetServerResultPrivate)
{

}

GetServerResult::GetServerResult(const GetServerResult& rhs) : 
	d(rhs.d)
{

}

GetServerResult::~GetServerResult()
{

}

GetServerResult& GetServerResult::operator=(const GetServerResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult GetServerResult::common() const
{
	return d->m_common;
}

void GetServerResult::setCommon(const CommonResult& result) 
{
	d->m_common = result;
}

QStringList GetServerResult::sipServerList() const 
{
	return d->m_sipServerList;
}

void GetServerResult::setSipServerList(const QStringList& list)
{
	d->m_sipServerList = list;
}

QString GetServerResult::domainStatus() const
{
	return d->m_domainStatus;
}

void GetServerResult::setDomainStatus(const QString& status)
{
	d->m_domainStatus = status;
}

QString GetServerResult::domainCall() const
{
	return d->m_domainCall;
}

void GetServerResult::setDomainCall(const QString& call)
{
	d->m_domainCall = call;
}

QString GetServerResult::domainIM() const
{
	return d->m_domainIM;
}

void GetServerResult::setDomainIM(const QString& IM)
{
	d->m_domainIM = IM;
}

//TodayStatParam
class TodayStatParamPrivate : public QSharedData
{
public:
	TodayStatParamPrivate();
	TodayStatParamPrivate(const TodayStatParamPrivate& rhs);

};

TodayStatParamPrivate::TodayStatParamPrivate() :
QSharedData()
{

}


TodayStatParamPrivate::TodayStatParamPrivate(const TodayStatParamPrivate& rhs) :
QSharedData(rhs)
{

}

TodayStatParam::TodayStatParam() :
	d(new TodayStatParamPrivate)
{

}

TodayStatParam::TodayStatParam(const TodayStatParam& rhs) : 
	d(rhs.d)
{

}

TodayStatParam::~TodayStatParam()
{

}

TodayStatParam& TodayStatParam::operator=(const TodayStatParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool TodayStatParam::operator==(const TodayStatParam& rhs) const
{
	return false;
}

bool TodayStatParam::operator!=(const TodayStatParam& rhs) const
{
	return !operator==(rhs);
}

//TodayStatResult
class TodayStatResultPrivate : public QSharedData
{
public:
	TodayStatResultPrivate();
	TodayStatResultPrivate(const TodayStatResultPrivate& rhs);

	CommonResult m_common;
	QString m_answer;
	QString m_success;
	QString m_noAnswer;
};

TodayStatResultPrivate::TodayStatResultPrivate() : 
	QSharedData(),
	m_common(),
	m_answer(),
	m_success(),
	m_noAnswer()
{

}

TodayStatResultPrivate::TodayStatResultPrivate(const TodayStatResultPrivate& rhs) : 
	QSharedData(rhs),
	m_common(rhs.m_common),
	m_answer(rhs.m_answer),
	m_success(rhs.m_success),
	m_noAnswer(rhs.m_noAnswer)
{

}

TodayStatResult::TodayStatResult() : 
	d(new TodayStatResultPrivate)
{

}

TodayStatResult::TodayStatResult(const TodayStatResult& rhs) : 
	d(rhs.d)
{

}

TodayStatResult::~TodayStatResult()
{

}

TodayStatResult& TodayStatResult::operator=(const TodayStatResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult TodayStatResult::common() const
{
	return d->m_common;
}

void TodayStatResult::setCommon(const CommonResult& result) 
{
	d->m_common = result;
}

QString TodayStatResult::answer() const
{
	return d->m_answer;
}

void TodayStatResult::setAnswer(const QString& value)
{
	d->m_answer = value;
}

QString TodayStatResult::success() const
{
	return d->m_success;
}

void TodayStatResult::setSuccess(const QString& value)
{
	d->m_success = value;
}

QString TodayStatResult::noanswer() const
{
	return d->m_noAnswer;
}

void TodayStatResult::setNoAnswer(const QString& value)
{
	d->m_noAnswer = value;
}

//GetNoReportParam
class GetNoReportParamPrivate : public QSharedData
{
public:
	GetNoReportParamPrivate();
	GetNoReportParamPrivate(const GetNoReportParamPrivate& rhs);

};

GetNoReportParamPrivate::GetNoReportParamPrivate() :
	QSharedData()
{

}


GetNoReportParamPrivate::GetNoReportParamPrivate(const GetNoReportParamPrivate& rhs) :
	QSharedData(rhs)
{

}

GetNoReportParam::GetNoReportParam() :
	d(new GetNoReportParamPrivate)
{

}

GetNoReportParam::GetNoReportParam(const GetNoReportParam& rhs) : 
	d(rhs.d)
{

}

GetNoReportParam::~GetNoReportParam()
{

}

GetNoReportParam& GetNoReportParam::operator=(const GetNoReportParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool GetNoReportParam::operator==(const GetNoReportParam& rhs) const
{
	return false;
}

bool GetNoReportParam::operator!=(const GetNoReportParam& rhs) const
{
	return !operator==(rhs);
}

//GetNoReportResult
class GetNoReportResultPrivate : public QSharedData
{
public:
	GetNoReportResultPrivate();
	GetNoReportResultPrivate(const GetNoReportResultPrivate& rhs);

	CommonResult m_common;
	QString m_num;
};

GetNoReportResultPrivate::GetNoReportResultPrivate() : 
	QSharedData(),
	m_common(),
	m_num()
{

}

GetNoReportResultPrivate::GetNoReportResultPrivate(const GetNoReportResultPrivate& rhs) : 
	QSharedData(rhs),
	m_common(rhs.m_common),
	m_num(rhs.m_num)
{

}

GetNoReportResult::GetNoReportResult() : 
	d(new GetNoReportResultPrivate)
{

}

GetNoReportResult::GetNoReportResult(const GetNoReportResult& rhs) : 
	d(rhs.d)
{

}

GetNoReportResult::~GetNoReportResult()
{

}

GetNoReportResult& GetNoReportResult::operator=(const GetNoReportResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult GetNoReportResult::common() const
{
	return d->m_common;
}

void GetNoReportResult::setCommon(const CommonResult& result) 
{
	d->m_common = result;
}

QString GetNoReportResult::num() const 
{
	return d->m_num;
}

void GetNoReportResult::setNum(const QString& num)
{
	d->m_num = num;
}

// GetCDRListParam
class GetCDRListParamPrivate : public QSharedData
{
public:
	GetCDRListParamPrivate();
	GetCDRListParamPrivate(const GetCDRListParamPrivate& rhs);

	QString m_id;
	QString m_type;
};

GetCDRListParamPrivate::GetCDRListParamPrivate() : 
	QSharedData(),
	m_id(),
	m_type()
{

}

GetCDRListParamPrivate::GetCDRListParamPrivate(const GetCDRListParamPrivate& rhs) :
	QSharedData(rhs),
	m_id(rhs.m_id),
	m_type(rhs.m_type)
{

}

GetCDRListParam::GetCDRListParam() : 
	d(new GetCDRListParamPrivate)
{

}

GetCDRListParam::GetCDRListParam(const GetCDRListParam& rhs) : 
	d(rhs.d)
{

}

GetCDRListParam::~GetCDRListParam()
{

}

GetCDRListParam& GetCDRListParam::operator=(const GetCDRListParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString GetCDRListParam::Id() const
{
	return d->m_id;
}

void GetCDRListParam::setId(const QString& Id)
{
	d->m_id = Id;
}

QString GetCDRListParam::type() const
{
	return d->m_type;
}

void GetCDRListParam::setType(const QString& type)
{
	d->m_type = type;
}

bool GetCDRListParam::operator==(const GetCDRListParam& rhs) const
{
	return d->m_id == rhs.d->m_id
		&& d->m_type == rhs.d->m_type;
}

bool GetCDRListParam::operator!=(const GetCDRListParam& rhs) const
{
	return !operator==(rhs);
}

// GetCDRListResult
class GetCDRListResultPrivate : public QSharedData
{
public:
	GetCDRListResultPrivate();
	GetCDRListResultPrivate(const GetCDRListResultPrivate& rhs);

	CommonResult m_common;
	QString m_id;
	QString m_VESysId;
	QString m_meetCDRId;
	QString m_smallPhoto;
	QString m_displayName;
	QString m_startCallTime;
	QString m_callTime;
	QString m_isAnswered;
	QString m_callMode;
	QString m_isSuccess;
};

GetCDRListResultPrivate::GetCDRListResultPrivate() : 
	QSharedData(),
	m_common(),
	m_id(),
	m_VESysId(),
	m_meetCDRId(),
	m_smallPhoto(),
	m_displayName(),
	m_startCallTime(),
	m_callTime(),
	m_isAnswered(),
	m_callMode(),
	m_isSuccess()
{

}

GetCDRListResultPrivate::GetCDRListResultPrivate(const GetCDRListResultPrivate& rhs) : 
	QSharedData(rhs),
	m_common(rhs.m_common),
	m_id(rhs.m_id),
	m_VESysId(rhs.m_VESysId),
	m_meetCDRId(rhs.m_meetCDRId),
	m_smallPhoto(rhs.m_smallPhoto),
	m_displayName(rhs.m_displayName),
	m_startCallTime(rhs.m_startCallTime),
	m_callTime(rhs.m_callTime),
	m_isAnswered(rhs.m_isAnswered),
	m_callMode(rhs.m_callMode),
	m_isSuccess(rhs.m_isSuccess)
{

}

GetCDRListResult::GetCDRListResult() : 
	d(new GetCDRListResultPrivate)
{

}

GetCDRListResult::GetCDRListResult(const GetCDRListResult& rhs) : 
	d(rhs.d)
{

}

GetCDRListResult::~GetCDRListResult()
{

}

GetCDRListResult& GetCDRListResult::operator=(const GetCDRListResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult GetCDRListResult::common() const
{
	return d->m_common;
}

void GetCDRListResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QString GetCDRListResult::Id() const
{
	return d->m_id;
}

void GetCDRListResult::setId(const QString& Id)
{
	d->m_id = Id;
}

QString GetCDRListResult::VESysId() const
{
	return d->m_VESysId;
}

void GetCDRListResult::setVESysId(const QString& Id)
{
	d->m_VESysId = Id;
}

QString GetCDRListResult::meetCDRId() const
{
	return d->m_meetCDRId;
}

void GetCDRListResult::setMeetCDRId(const QString& Id)
{
	d->m_meetCDRId = Id;
}

QString GetCDRListResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void GetCDRListResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString GetCDRListResult::displayName() const
{
	return d->m_displayName;
}

void GetCDRListResult::setDisplayName(const QString& name)
{
	d->m_displayName = name;
}

QString GetCDRListResult::startCallTime() const
{
	return d->m_startCallTime;
}

void GetCDRListResult::setStartCallTime(const QString& callTime)
{
	d->m_startCallTime = callTime;
}

QString GetCDRListResult::callTime() const
{
	return d->m_callTime;
}

void GetCDRListResult::setCallTime(const QString& callTime)
{
	d->m_callTime = callTime;
}

QString GetCDRListResult::isAnswered() const
{
	return d->m_isAnswered;
}

void GetCDRListResult::setIsAnswered(const QString& answered)
{
	d->m_isAnswered = answered;
}

QString GetCDRListResult::callMode() const
{
	return d->m_callMode;
}

void GetCDRListResult::setCallMode(const QString& mode)
{
	d->m_callMode = mode;
}

QString GetCDRListResult::isSuccess() const
{
	return d->m_isSuccess;
}

void GetCDRListResult::setIsSuccess(const QString& success)
{
	d->m_isSuccess = success;
}

// GetCDRInfoParam
class GetCDRInfoParamPrivate : public QSharedData
{
public:
	GetCDRInfoParamPrivate();
	GetCDRInfoParamPrivate(const GetCDRInfoParamPrivate& rhs);

	QString m_meetCDRId;
};

GetCDRInfoParamPrivate::GetCDRInfoParamPrivate() : 
	QSharedData(),
	m_meetCDRId()
{

}

GetCDRInfoParamPrivate::GetCDRInfoParamPrivate(const GetCDRInfoParamPrivate& rhs) :
	QSharedData(rhs),
	m_meetCDRId(rhs.m_meetCDRId)
{

}

GetCDRInfoParam::GetCDRInfoParam() : 
	d(new GetCDRInfoParamPrivate)
{

}

GetCDRInfoParam::GetCDRInfoParam(const GetCDRInfoParam& rhs) : 
	d(rhs.d)
{

}

GetCDRInfoParam::~GetCDRInfoParam()
{

}

GetCDRInfoParam& GetCDRInfoParam::operator=(const GetCDRInfoParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString GetCDRInfoParam::meetCDRId() const
{
	return d->m_meetCDRId;
}

void GetCDRInfoParam::setMeetCDRId(const QString& Id)
{
	d->m_meetCDRId = Id;
}

bool GetCDRInfoParam::operator==(const GetCDRInfoParam& rhs) const
{
	return d->m_meetCDRId == rhs.d->m_meetCDRId;
}

bool GetCDRInfoParam::operator!=(const GetCDRInfoParam& rhs) const
{
	return !operator==(rhs);
}

// GetCDRInfoResult
class GetCDRInfoResultPrivate : public QSharedData
{
public:
	GetCDRInfoResultPrivate();
	GetCDRInfoResultPrivate(const GetCDRInfoResultPrivate& rhs);

	CommonResult m_common;
	QString m_CDRType;
	QString m_callerVESysId;
	QString m_callerId;
	QString m_callerVELoginId;
	QString m_callerDisplayName;
	QString m_callerMobile;
	QString m_callerSmallPhoto;
	QString m_callerLargePhoto;
	QString m_isPSTNCall;
	QString m_callCountry;
	QString m_calleeVESysId;
	QString m_calleeId;
	QString m_calleeVELoginId;
	QString m_calleeDisplayName;
	QString m_calleeMobile;
	QString m_calleeSmallPhoto;
	QString m_calleeLargePhoto;
	QString m_partnerVESysId;
	QString m_partnerId;
	QString m_partnerLoginId;
	QString m_partnerDisplayName;
	QString m_partnerSmallPhoto;
	QString m_partnerLargePhoto;
	QString m_isFree;
	QString m_partnerLevel;
	QString m_language1;
	QString m_language2;
	QString m_callTime;
	QString m_startCallTime;
};

GetCDRInfoResultPrivate::GetCDRInfoResultPrivate() : 
	QSharedData(),
	m_common(),
	m_CDRType(),
	m_callerVESysId(),
	m_callerId(),
	m_callerVELoginId(),
	m_callerDisplayName(),
	m_callerMobile(),
	m_callerSmallPhoto(),
	m_callerLargePhoto(),
	m_isPSTNCall(),
	m_callCountry(),
	m_calleeVESysId(),
	m_calleeId(),
	m_calleeVELoginId(),
	m_calleeDisplayName(),
	m_calleeMobile(),
	m_calleeSmallPhoto(),
	m_calleeLargePhoto(),
	m_partnerVESysId(),
	m_partnerId(),
	m_partnerLoginId(),
	m_partnerDisplayName(),
	m_partnerSmallPhoto(),
	m_partnerLargePhoto(),
	m_isFree(),
	m_partnerLevel(),
	m_language1(),
	m_language2(),
	m_callTime(),
	m_startCallTime()
{

}

GetCDRInfoResultPrivate::GetCDRInfoResultPrivate(const GetCDRInfoResultPrivate& rhs) : 
	QSharedData(rhs),
	m_common(rhs.m_common),
	m_CDRType(rhs.m_CDRType),
	m_callerVESysId(rhs.m_callerVESysId),
	m_callerId(rhs.m_callerId),
	m_callerVELoginId(rhs.m_callerVELoginId),
	m_callerDisplayName(rhs.m_callerDisplayName),
	m_callerMobile(rhs.m_callerMobile),
	m_callerSmallPhoto(rhs.m_callerSmallPhoto),
	m_callerLargePhoto(rhs.m_callerLargePhoto),
	m_isPSTNCall(rhs.m_isPSTNCall),
	m_callCountry(rhs.m_callCountry),
	m_calleeVESysId(rhs.m_calleeVESysId),
	m_calleeId(rhs.m_calleeId),
	m_calleeVELoginId(rhs.m_calleeVELoginId),
	m_calleeDisplayName(rhs.m_calleeDisplayName),
	m_calleeMobile(rhs.m_calleeMobile),
	m_calleeSmallPhoto(rhs.m_calleeSmallPhoto),
	m_calleeLargePhoto(rhs.m_calleeLargePhoto),
	m_partnerVESysId(rhs.m_partnerVESysId),
	m_partnerId(rhs.m_partnerId),
	m_partnerLoginId(rhs.m_partnerLoginId),
	m_partnerDisplayName(rhs.m_partnerDisplayName),
	m_partnerSmallPhoto(rhs.m_partnerSmallPhoto),
	m_partnerLargePhoto(rhs.m_partnerLargePhoto),
	m_isFree(rhs.m_isFree),
	m_partnerLevel(rhs.m_partnerLevel),
	m_language1(rhs.m_language1),
	m_language2(rhs.m_language2),
	m_callTime(rhs.m_callTime),
	m_startCallTime(rhs.m_startCallTime)
{

}

GetCDRInfoResult::GetCDRInfoResult() : 
	d(new GetCDRInfoResultPrivate)
{

}

GetCDRInfoResult::GetCDRInfoResult(const GetCDRInfoResult& rhs) : 
	d(rhs.d)
{

}

GetCDRInfoResult::~GetCDRInfoResult()
{

}

GetCDRInfoResult& GetCDRInfoResult::operator=(const GetCDRInfoResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult GetCDRInfoResult::common() const
{
	return d->m_common;
}

void GetCDRInfoResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QString GetCDRInfoResult::CDRType() const
{
	return d->m_CDRType;
}

void GetCDRInfoResult::setCDRType(const QString& type)
{
	d->m_CDRType = type;
}

QString GetCDRInfoResult::callerVESysId() const
{
	return d->m_callerVESysId;
}

void GetCDRInfoResult::setCallerVESysId(const QString& Id)
{
	d->m_callerVESysId = Id;
}

QString GetCDRInfoResult::callerId() const
{
	return d->m_callerId;
}

void GetCDRInfoResult::setCallerId(const QString& Id)
{
	d->m_callerId = Id;
}

QString GetCDRInfoResult::callerVELoginId() const
{
	return d->m_callerVELoginId;
}

void GetCDRInfoResult::setCallerVELoginId(const QString& Id)
{
	d->m_callerVELoginId = Id;
}

QString GetCDRInfoResult::callerDisplayName() const
{
	return d->m_callerDisplayName;
}

void GetCDRInfoResult::setCallerDisplayName(const QString& name)
{
	d->m_callerDisplayName = name;
}

QString GetCDRInfoResult::callerMobile() const
{
	return d->m_callerMobile;
}

void GetCDRInfoResult::setCallerMobile(const QString& mobile)
{
	d->m_callerMobile = mobile;
}

QString GetCDRInfoResult::callerSmallPhoto() const
{
	return d->m_callerSmallPhoto;
}

void GetCDRInfoResult::setCallerSmallPhoto(const QString& photo)
{
	d->m_callerSmallPhoto = photo;
}

QString GetCDRInfoResult::callerLargePhoto() const
{
	return d->m_callerLargePhoto;
}

void GetCDRInfoResult::setCallerLargePhoto(const QString& photo)
{
	d->m_callerLargePhoto = photo;
}

QString GetCDRInfoResult::isPSTNCall() const
{
	return d->m_isPSTNCall;
}

void GetCDRInfoResult::setIsPSTNCall(const QString& PSTNCall)
{
	d->m_isPSTNCall = PSTNCall;
}

QString GetCDRInfoResult::callCountry() const
{
	return d->m_callCountry;
}

void GetCDRInfoResult::setCallCountry(const QString& country)
{
	d->m_callCountry = country;
}

QString GetCDRInfoResult::calleeVESysId() const
{
	return d->m_calleeVESysId;
}

void GetCDRInfoResult::setCalleeVESysId(const QString& Id)
{
	d->m_calleeVESysId = Id;
}

QString GetCDRInfoResult::calleeId() const
{
	return d->m_calleeId;
}

void GetCDRInfoResult::setCalleeId(const QString& Id)
{
	d->m_calleeId = Id;
}

QString GetCDRInfoResult::calleeVELoginId() const
{
	return d->m_calleeVELoginId;
}

void GetCDRInfoResult::setCalleeVELoginId(const QString& Id)
{
	d->m_calleeVELoginId = Id;
}

QString GetCDRInfoResult::calleeDisplayName() const
{
	return d->m_calleeDisplayName;
}

void GetCDRInfoResult::setCalleeDisplayName(const QString& name)
{
	d->m_calleeDisplayName = name;
}

QString GetCDRInfoResult::calleeMobile() const
{
	return d->m_calleeMobile;
}

void GetCDRInfoResult::setCalleeMobile(const QString& mobile)
{
	d->m_calleeMobile = mobile;
}

QString GetCDRInfoResult::calleeSmallPhoto() const
{
	return d->m_calleeSmallPhoto;
}

void GetCDRInfoResult::setCalleeSmallPhoto(const QString& photo)
{
	d->m_calleeSmallPhoto = photo;
}

QString GetCDRInfoResult::calleeLargePhoto() const
{
	return d->m_calleeLargePhoto;
}

void GetCDRInfoResult::setCalleeLargePhoto(const QString& photo)
{
	d->m_calleeLargePhoto = photo;
}

QString GetCDRInfoResult::partnerVESysId() const
{
	return d->m_partnerVESysId;
}

void GetCDRInfoResult::setPartnerVESysId(const QString& Id)
{
	d->m_partnerVESysId = Id;
}

QString GetCDRInfoResult::partnerId() const
{
	return d->m_partnerId;
}

void GetCDRInfoResult::setPartnerId(const QString& Id)
{
	d->m_partnerId = Id;
}

QString GetCDRInfoResult::partnerLoginId() const
{
	return d->m_partnerLoginId;
}

void GetCDRInfoResult::setPartnerLoginId(const QString& Id)
{
	d->m_partnerLoginId = Id;
}

QString GetCDRInfoResult::partnerDisplayName() const
{
	return d->m_partnerDisplayName;
}

void GetCDRInfoResult::setPartnerDisplayName(const QString& name)
{
	d->m_partnerDisplayName = name;
}

QString GetCDRInfoResult::partnerSmallPhoto() const
{
	return d->m_partnerSmallPhoto;
}

void GetCDRInfoResult::setPartnerSmallPhoto(const QString& photo)
{
	d->m_partnerSmallPhoto = photo;
}

QString GetCDRInfoResult::partnerLargePhoto() const
{
	return d->m_partnerLargePhoto;
}

void GetCDRInfoResult::setPartnerLargePhoto(const QString& photo)
{
	d->m_partnerLargePhoto = photo;
}

QString GetCDRInfoResult::isFree() const
{
	return d->m_isFree;
}

void GetCDRInfoResult::setIsFree(const QString& free)
{
	d->m_isFree = free;
}

QString GetCDRInfoResult::partnerLevel() const
{
	return d->m_partnerLevel;
}

void GetCDRInfoResult::setPartnerLevel(const QString& level)
{
	d->m_partnerLevel = level;
}

QString GetCDRInfoResult::language1() const
{
	return d->m_language1;
}

void GetCDRInfoResult::setLanguage1(const QString& lang)
{
	d->m_language1 = lang;
}

QString GetCDRInfoResult::language2() const
{
	return d->m_language2;
}

void GetCDRInfoResult::setLanguage2(const QString& lang)
{
	d->m_language2 = lang;
}

QString GetCDRInfoResult::callTime() const
{
	return d->m_callTime;
}

void GetCDRInfoResult::setCallTime(const QString& time)
{
	d->m_callTime = time;
}

QString GetCDRInfoResult::startCallTime() const
{
	return d->m_startCallTime;
}

void GetCDRInfoResult::setStartCallTime(const QString& time)
{
	d->m_startCallTime = time;
}

// GetUserInfoParam
class GetUserInfoParamPrivate : public QSharedData
{
public:
	GetUserInfoParamPrivate();
	GetUserInfoParamPrivate(const GetUserInfoParamPrivate& rhs);
};

GetUserInfoParamPrivate::GetUserInfoParamPrivate() :
	QSharedData()
{
}

GetUserInfoParamPrivate::GetUserInfoParamPrivate(const GetUserInfoParamPrivate& rhs) :
	QSharedData(rhs)
{
}

GetUserInfoParam::GetUserInfoParam() :
	d(new GetUserInfoParamPrivate)
{
}

GetUserInfoParam::GetUserInfoParam(const GetUserInfoParam& rhs) :
	d(rhs.d)
{
}

GetUserInfoParam::~GetUserInfoParam()
{
}

GetUserInfoParam& GetUserInfoParam::operator=(const GetUserInfoParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool GetUserInfoParam::operator==(const GetUserInfoParam& rhs) const
{
	return true;
}

bool GetUserInfoParam::operator!=(const GetUserInfoParam& rhs) const
{
	return !operator==(rhs);
}

// GetUserInfoResult
class GetUserInfoResultPrivate : public QSharedData
{
public:
	GetUserInfoResultPrivate();
	GetUserInfoResultPrivate(const GetUserInfoResultPrivate& rhs);

	CommonResult m_common;
	QString m_systemId;
	QString m_callerId;
	QString m_loginId;
	QString m_secret;
	QString m_displayName;
	QString m_gender;
	QString m_birthday;
	QString m_nationality;
	QString m_education;
	QString m_major;
	QString m_introduction;
	QStringList m_skill;
	QString m_smallPhoto;
	QString m_largePhoto;
};

GetUserInfoResultPrivate::GetUserInfoResultPrivate() :
	QSharedData(),
	m_common(),
	m_systemId(0),
	m_callerId(),
	m_loginId(),
	m_secret(),
	m_displayName(),
	m_gender(),
	m_birthday(),
	m_nationality(),
	m_education(),
	m_major(),
	m_introduction(),
	m_skill(),
	m_smallPhoto(),
	m_largePhoto()
{
}

GetUserInfoResultPrivate::GetUserInfoResultPrivate(const GetUserInfoResultPrivate& rhs) :
	QSharedData(rhs),
	m_common(rhs.m_common),
	m_systemId(rhs.m_systemId),
	m_callerId(rhs.m_callerId),
	m_loginId(rhs.m_loginId),
	m_secret(rhs.m_secret),
	m_displayName(rhs.m_displayName),
	m_gender(rhs.m_gender),
	m_birthday(rhs.m_birthday),
	m_nationality(rhs.m_nationality),
	m_education(rhs.m_education),
	m_major(rhs.m_major),
	m_introduction(rhs.m_introduction),
	m_skill(rhs.m_skill),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto)
{
}

GetUserInfoResult::GetUserInfoResult() :
	d(new GetUserInfoResultPrivate)
{
}

GetUserInfoResult::GetUserInfoResult(const GetUserInfoResult& rhs) :
	d(rhs.d)
{
}

GetUserInfoResult::~GetUserInfoResult()
{
}

GetUserInfoResult& GetUserInfoResult::operator=(const GetUserInfoResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult GetUserInfoResult::common() const
{
	return d->m_common;
}

void GetUserInfoResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QString GetUserInfoResult::systemId() const
{
	return d->m_systemId;
}

void GetUserInfoResult::setSystemId(const QString& Id)
{
	d->m_systemId = Id;
}

QString GetUserInfoResult::callerId() const
{
	return d->m_callerId;
}

void GetUserInfoResult::setCallerId(const QString& Id)
{
	d->m_callerId = Id;
}

QString GetUserInfoResult::loginId() const
{
	return d->m_loginId;
}

void GetUserInfoResult::setLoginId(const QString& Id)
{
	d->m_loginId = Id;
}

QString GetUserInfoResult::secret() const
{
	return d->m_secret;
}

void GetUserInfoResult::setSecret(const QString& secret)
{
	d->m_secret = secret;
}

QString GetUserInfoResult::displayName() const
{
	return d->m_displayName;
}

void GetUserInfoResult::setDisplayName(const QString& name)
{
	d->m_displayName = name;
}

QString GetUserInfoResult::gender() const
{
	return d->m_gender;
}

void GetUserInfoResult::setGender(const QString& gender)
{
	d->m_gender = gender;
}

QString GetUserInfoResult::birthday() const
{
	return d->m_birthday;
}

void GetUserInfoResult::setBirthday(const QString& dateTime)
{
	d->m_birthday = dateTime;
}

QString GetUserInfoResult::nationality() const
{
	return d->m_nationality;
}

void GetUserInfoResult::setNationality(const QString& nation)
{
	d->m_nationality = nation;
}

QString GetUserInfoResult::education() const
{
	return d->m_education;
}

void GetUserInfoResult::setEducation(const QString& edu)
{
	d->m_education = edu;
}

QString GetUserInfoResult::major() const
{
	return d->m_major;
}

void GetUserInfoResult::setMajor(const QString& name)
{
	d->m_major = name;
}

QString GetUserInfoResult::introduction() const
{
	return d->m_introduction;
}

void GetUserInfoResult::setIntroduction(const QString& name)
{
	d->m_introduction = name;
}

QStringList GetUserInfoResult::skill() const
{
	return d->m_skill;
}

void GetUserInfoResult::setSkill(const QStringList& skill)
{
	d->m_skill = skill;
}

QString GetUserInfoResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void GetUserInfoResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString GetUserInfoResult::largePhoto() const
{
	return d->m_largePhoto;
}

void GetUserInfoResult::setLargePhoto(const QString& photo)
{
	d->m_largePhoto = photo;
}


// GetCallerUserParam
class GetCallerUserParamPrivate : public QSharedData
{
public:
	GetCallerUserParamPrivate();
	GetCallerUserParamPrivate(const GetCallerUserParamPrivate& rhs);

	QString m_callerId;
};

GetCallerUserParamPrivate::GetCallerUserParamPrivate() :
	QSharedData(),
	m_callerId()
{
}

GetCallerUserParamPrivate::GetCallerUserParamPrivate(const GetCallerUserParamPrivate& rhs) :
	QSharedData(rhs),
	m_callerId(rhs.m_callerId)
{
}

GetCallerUserParam::GetCallerUserParam() :
	d(new GetCallerUserParamPrivate)
{
}

GetCallerUserParam::GetCallerUserParam(const GetCallerUserParam& rhs) :
	d(rhs.d)
{
}

GetCallerUserParam::~GetCallerUserParam()
{
}

GetCallerUserParam& GetCallerUserParam::operator=(const GetCallerUserParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString GetCallerUserParam::callerId() const
{
	return d->m_callerId;
}

void GetCallerUserParam::setCallerId(const QString& Id)
{
	d->m_callerId = Id;
}

bool GetCallerUserParam::operator==(const GetCallerUserParam& rhs) const
{
	return d->m_callerId == rhs.d->m_callerId;
}

bool GetCallerUserParam::operator!=(const GetCallerUserParam& rhs) const
{
	return !operator==(rhs);
}

// GetCallerUserResult
class GetCallerUserResultPrivate : public QSharedData
{
public:
	GetCallerUserResultPrivate();
	GetCallerUserResultPrivate(const GetCallerUserResultPrivate& rhs);

	CommonResult m_common;
	QString m_smallPhoto;
	QString m_largePhoto;
	QString m_displayName;
	QString m_mobile;
};

GetCallerUserResultPrivate::GetCallerUserResultPrivate() :
	QSharedData(),
	m_common(),
	m_smallPhoto(),
	m_largePhoto(),
	m_displayName(),
	m_mobile()
{
}

GetCallerUserResultPrivate::GetCallerUserResultPrivate(const GetCallerUserResultPrivate& rhs) :
	QSharedData(rhs),
	m_common(rhs.m_common),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto),
	m_displayName(rhs.m_displayName),
	m_mobile(rhs.m_mobile)
{
}

GetCallerUserResult::GetCallerUserResult() :
	d(new GetCallerUserResultPrivate)
{
}

GetCallerUserResult::GetCallerUserResult(const GetCallerUserResult& rhs) :
	d(rhs.d)
{
}

GetCallerUserResult::~GetCallerUserResult()
{
}

GetCallerUserResult& GetCallerUserResult::operator=(const GetCallerUserResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult GetCallerUserResult::common() const
{
	return d->m_common;
}

void GetCallerUserResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QString GetCallerUserResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void GetCallerUserResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString GetCallerUserResult::largePhoto() const
{
	return d->m_largePhoto;
}

void GetCallerUserResult::setLargePhoto(const QString& photo)
{
	d->m_largePhoto = photo;
}

QString GetCallerUserResult::displayName() const
{
	return d->m_displayName;
}

void GetCallerUserResult::setDisplayName(const QString& name)
{
	d->m_displayName = name;
}

QString GetCallerUserResult::mobile() const
{
	return d->m_mobile;
}

void GetCallerUserResult::setMobile(const QString& mobile)
{
	d->m_mobile = mobile;
}

// SaveScreenShotParam
class SaveScreenShotParamPrivate : public QSharedData
{
public:
	SaveScreenShotParamPrivate();
	SaveScreenShotParamPrivate(const SaveScreenShotParamPrivate& rhs);

	QString m_meetCDRId;
	QString m_photo;
};

SaveScreenShotParamPrivate::SaveScreenShotParamPrivate() :
	QSharedData(),
	m_meetCDRId(),
	m_photo()
{
}

SaveScreenShotParamPrivate::SaveScreenShotParamPrivate(const SaveScreenShotParamPrivate& rhs) :
	QSharedData(rhs),
	m_meetCDRId(rhs.m_meetCDRId),
	m_photo(rhs.m_photo)
{
}

SaveScreenShotParam::SaveScreenShotParam() :
	d(new SaveScreenShotParamPrivate)
{
}

SaveScreenShotParam::SaveScreenShotParam(const SaveScreenShotParam& rhs) :
	d(rhs.d)
{
}

SaveScreenShotParam::~SaveScreenShotParam()
{
}

SaveScreenShotParam& SaveScreenShotParam::operator=(const SaveScreenShotParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString SaveScreenShotParam::meetCDRId() const
{
	return d->m_meetCDRId;
}

void SaveScreenShotParam::setMeetCDRId(const QString& Id)
{
	d->m_meetCDRId = Id;
}

QString SaveScreenShotParam::photo() const
{
	return d->m_photo;
}

void SaveScreenShotParam::setPhoto(const QString& photo)
{
	d->m_photo = photo;
}

bool SaveScreenShotParam::operator==(const SaveScreenShotParam& rhs) const
{
	return d->m_meetCDRId == rhs.d->m_meetCDRId
		&& d->m_photo == rhs.d->m_photo;
}

bool SaveScreenShotParam::operator!=(const SaveScreenShotParam& rhs) const
{
	return !operator==(rhs);
}

// SaveScreenShotResult
class SaveScreenShotResultPrivate : public QSharedData
{
public:
	SaveScreenShotResultPrivate();
	SaveScreenShotResultPrivate(const SaveScreenShotResultPrivate& rhs);

	CommonResult m_common;
};

SaveScreenShotResultPrivate::SaveScreenShotResultPrivate() :
	QSharedData()
{
}

SaveScreenShotResultPrivate::SaveScreenShotResultPrivate(const SaveScreenShotResultPrivate& rhs) :
	QSharedData(rhs)
{
}

SaveScreenShotResult::SaveScreenShotResult() :
	d(new SaveScreenShotResultPrivate)
{
}

SaveScreenShotResult::SaveScreenShotResult(const SaveScreenShotResult& rhs) :
	d(rhs.d)
{
}

SaveScreenShotResult::~SaveScreenShotResult()
{
}

SaveScreenShotResult& SaveScreenShotResult::operator=(const SaveScreenShotResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult SaveScreenShotResult::common() const
{
	return d->m_common;
}

void SaveScreenShotResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

// ForgetPasswordParam
class ForgetPasswordParamPrivate : public QSharedData
{
public:
	ForgetPasswordParamPrivate();
	ForgetPasswordParamPrivate(const ForgetPasswordParamPrivate& rhs);

	QString m_userName;
};

ForgetPasswordParamPrivate::ForgetPasswordParamPrivate() : 
	QSharedData(),
	m_userName()
{
}

ForgetPasswordParamPrivate::ForgetPasswordParamPrivate(const ForgetPasswordParamPrivate& rhs) : 
	QSharedData(rhs),
	m_userName(rhs.m_userName)
{
}

ForgetPasswordParam::ForgetPasswordParam() :
	d(new ForgetPasswordParamPrivate)
{
}

ForgetPasswordParam::ForgetPasswordParam(const ForgetPasswordParam& rhs) :
	d(rhs.d)
{
}

ForgetPasswordParam::~ForgetPasswordParam()
{
}

ForgetPasswordParam& ForgetPasswordParam::operator=(const ForgetPasswordParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString ForgetPasswordParam::userName() const
{
	return d->m_userName;
}

void ForgetPasswordParam::setUserName(const QString& userName)
{
	d->m_userName = userName;
}

bool ForgetPasswordParam::operator==(const ForgetPasswordParam& rhs) const
{
	return d->m_userName == rhs.d->m_userName;
}

bool ForgetPasswordParam::operator!=(const ForgetPasswordParam& rhs) const
{
	return !operator==(rhs);
}

// CreateAccountParam
class CreateAccountParamPrivate : public QSharedData
{
public:
	CreateAccountParamPrivate();
	CreateAccountParamPrivate(const CreateAccountParamPrivate& rhs);

};

CreateAccountParamPrivate::CreateAccountParamPrivate() : 
	QSharedData()
{
}

CreateAccountParamPrivate::CreateAccountParamPrivate(const CreateAccountParamPrivate& rhs) : 
	QSharedData(rhs)
{
}

CreateAccountParam::CreateAccountParam() :
	d(new CreateAccountParamPrivate)
{
}

CreateAccountParam::CreateAccountParam(const CreateAccountParam& rhs) :
	d(rhs.d)
{
}

CreateAccountParam::~CreateAccountParam()
{
}

CreateAccountParam& CreateAccountParam::operator=(const CreateAccountParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool CreateAccountParam::operator==(const CreateAccountParam& rhs) const
{
	return true;
}

bool CreateAccountParam::operator!=(const CreateAccountParam& rhs) const
{
	return !operator==(rhs);
}

// ChangeStatusParam
class ChangeStatusParamPrivate : public QSharedData
{
public:
	ChangeStatusParamPrivate();
	ChangeStatusParamPrivate(const ChangeStatusParamPrivate& rhs);

	QString m_status;
};

ChangeStatusParamPrivate::ChangeStatusParamPrivate() :
	QSharedData(),
	m_status()
{
}

ChangeStatusParamPrivate::ChangeStatusParamPrivate(const ChangeStatusParamPrivate& rhs) :
	QSharedData(rhs),
	m_status(rhs.m_status)
{
}

ChangeStatusParam::ChangeStatusParam() :
	d(new ChangeStatusParamPrivate)
{
}

ChangeStatusParam::ChangeStatusParam(const ChangeStatusParam& rhs) :
	d(rhs.d)
{
}

ChangeStatusParam::~ChangeStatusParam()
{
}

ChangeStatusParam& ChangeStatusParam::operator=(const ChangeStatusParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString ChangeStatusParam::onlineVisibleStatus() const
{
	return d->m_status;
}

void ChangeStatusParam::setOnlineVisibleStatus(const QString& status)
{
	d->m_status = status;
}

bool ChangeStatusParam::operator==(const ChangeStatusParam& rhs) const
{
	return d->m_status == rhs.d->m_status;
}

bool ChangeStatusParam::operator!=(const ChangeStatusParam& rhs) const
{
	return !operator==(rhs);
}

// ChangeStatusResult
class ChangeStatusResultPrivate : public QSharedData
{
public:
	ChangeStatusResultPrivate();
	ChangeStatusResultPrivate(const ChangeStatusResultPrivate& rhs);

	CommonResult m_common;
};

ChangeStatusResultPrivate::ChangeStatusResultPrivate() :
	QSharedData()
{
}

ChangeStatusResultPrivate::ChangeStatusResultPrivate(const ChangeStatusResultPrivate& rhs) :
	QSharedData(rhs)
{
}

ChangeStatusResult::ChangeStatusResult() :
	d(new ChangeStatusResultPrivate)
{
}

ChangeStatusResult::ChangeStatusResult(const ChangeStatusResult& rhs) :
	d(rhs.d)
{
}

ChangeStatusResult::~ChangeStatusResult()
{
}

ChangeStatusResult& ChangeStatusResult::operator=(const ChangeStatusResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult ChangeStatusResult::common() const
{
	return d->m_common;
}

void ChangeStatusResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

// GetBlacklistParam
class GetBlacklistParamPrivate : public QSharedData
{
public:
	GetBlacklistParamPrivate();
	GetBlacklistParamPrivate(const GetBlacklistParamPrivate& rhs);

	QString m_id;
};

GetBlacklistParamPrivate::GetBlacklistParamPrivate() :
	QSharedData(),
	m_id()
{
}

GetBlacklistParamPrivate::GetBlacklistParamPrivate(const GetBlacklistParamPrivate& rhs) :
	QSharedData(rhs),
	m_id(rhs.m_id)
{
}

GetBlacklistParam::GetBlacklistParam() :
	d(new GetBlacklistParamPrivate)
{
}

GetBlacklistParam::GetBlacklistParam(const GetBlacklistParam& rhs) :
	d(rhs.d)
{
}

GetBlacklistParam::~GetBlacklistParam()
{
}

GetBlacklistParam& GetBlacklistParam::operator=(const GetBlacklistParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString GetBlacklistParam::id() const
{
	return d->m_id;
}

void GetBlacklistParam::setId(const QString& id)
{
	d->m_id = id;
}

bool GetBlacklistParam::operator==(const GetBlacklistParam& rhs) const
{
	return d->m_id == rhs.d->m_id;
}

bool GetBlacklistParam::operator!=(const GetBlacklistParam& rhs) const
{
	return !operator==(rhs);
}

// GetBlacklistResult
class GetBlacklistResultPrivate : public QSharedData
{
public:
	GetBlacklistResultPrivate();
	GetBlacklistResultPrivate(const GetBlacklistResultPrivate& rhs);

	CommonResult m_common;
	QString m_id;
	QString m_blacklistId;
	QString m_partnerSysId;
	QString m_VESysId;
	QString m_displayName;
	QString m_smallPhoto;
};

GetBlacklistResultPrivate::GetBlacklistResultPrivate() :
	QSharedData(),
	m_id(),
	m_blacklistId(),
	m_partnerSysId(),
	m_VESysId(),
	m_displayName(),
	m_smallPhoto()
{
}

GetBlacklistResultPrivate::GetBlacklistResultPrivate(const GetBlacklistResultPrivate& rhs) :
	QSharedData(rhs),
	m_id(rhs.m_id),
	m_blacklistId(rhs.m_blacklistId),
	m_partnerSysId(rhs.m_partnerSysId),
	m_VESysId(rhs.m_VESysId),
	m_displayName(rhs.m_displayName),
	m_smallPhoto(rhs.m_smallPhoto)
{
}

GetBlacklistResult::GetBlacklistResult() :
	d(new GetBlacklistResultPrivate)
{
}

GetBlacklistResult::GetBlacklistResult(const GetBlacklistResult& rhs) :
	d(rhs.d)
{
}

GetBlacklistResult::~GetBlacklistResult()
{
}

GetBlacklistResult& GetBlacklistResult::operator=(const GetBlacklistResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult GetBlacklistResult::common() const
{
	return d->m_common;
}

void GetBlacklistResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QString GetBlacklistResult::id() const
{
	return d->m_id;
}

void GetBlacklistResult::setId(const QString& id)
{
	d->m_id = id;
}

QString GetBlacklistResult::blacklistId() const
{
	return d->m_blacklistId;
}

void GetBlacklistResult::setBlacklistId(const QString& id)
{
	d->m_blacklistId = id;
}

QString GetBlacklistResult::parterSysId() const
{
	return d->m_partnerSysId;
}

void GetBlacklistResult::setPartnerSysId(const QString& id)
{
	d->m_partnerSysId = id;
}

QString GetBlacklistResult::VESysId() const
{
	return d->m_VESysId;
}

void GetBlacklistResult::setVESysId(const QString& id)
{
	d->m_VESysId = id;
}

QString GetBlacklistResult::displayName() const
{
	return d->m_displayName;
}

void GetBlacklistResult::setDisplayName(const QString& name)
{
	d->m_displayName = name;
}

QString GetBlacklistResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void GetBlacklistResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

//AddBlacklistParam
class AddBlacklistParamPrivate : public QSharedData
{
public:
	AddBlacklistParamPrivate();
	AddBlacklistParamPrivate(const AddBlacklistParamPrivate& rhs);

	QString m_VESysId;
};

AddBlacklistParamPrivate::AddBlacklistParamPrivate() :
	QSharedData(),
	m_VESysId()
{
}

AddBlacklistParamPrivate::AddBlacklistParamPrivate(const AddBlacklistParamPrivate& rhs) :
	QSharedData(rhs),
	m_VESysId(rhs.m_VESysId)
{
}

AddBlacklistParam::AddBlacklistParam() :
	d(new AddBlacklistParamPrivate)
{
}

AddBlacklistParam::AddBlacklistParam(const AddBlacklistParam& rhs) :
	d(rhs.d)
{
}

AddBlacklistParam::~AddBlacklistParam()
{
}

AddBlacklistParam& AddBlacklistParam::operator=(const AddBlacklistParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString AddBlacklistParam::VESysId() const
{
	return d->m_VESysId;
}

void AddBlacklistParam::setVESysId(const QString& id)
{
	d->m_VESysId = id;
}

bool AddBlacklistParam::operator==(const AddBlacklistParam& rhs) const
{
	return d->m_VESysId == rhs.d->m_VESysId;
}

bool AddBlacklistParam::operator!=(const AddBlacklistParam& rhs) const
{
	return !operator==(rhs);
}

// AddBlacklistResult
class AddBlacklistResultPrivate : public QSharedData
{
public:
	AddBlacklistResultPrivate();
	AddBlacklistResultPrivate(const AddBlacklistResultPrivate& rhs);

	CommonResult m_common;
	QString m_id;
	QString m_blacklistId;
	QString m_partnerSysId;
	QString m_VESysId;
	QString m_displayName;
	QString m_smallPhoto;
};

AddBlacklistResultPrivate::AddBlacklistResultPrivate() :
	QSharedData(),
	m_id(),
	m_blacklistId(),
	m_partnerSysId(),
	m_VESysId(),
	m_displayName(),
	m_smallPhoto()
{
}

AddBlacklistResultPrivate::AddBlacklistResultPrivate(const AddBlacklistResultPrivate& rhs) :
	QSharedData(rhs),
	m_id(rhs.m_id),
	m_blacklistId(rhs.m_blacklistId),
	m_partnerSysId(rhs.m_partnerSysId),
	m_VESysId(rhs.m_VESysId),
	m_displayName(rhs.m_displayName),
	m_smallPhoto(rhs.m_smallPhoto)
{
}

AddBlacklistResult::AddBlacklistResult() :
	d(new AddBlacklistResultPrivate)
{
}

AddBlacklistResult::AddBlacklistResult(const AddBlacklistResult& rhs) :
	d(rhs.d)
{
}

AddBlacklistResult::~AddBlacklistResult()
{
}

AddBlacklistResult& AddBlacklistResult::operator=(const AddBlacklistResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult AddBlacklistResult::common() const
{
	return d->m_common;
}

void AddBlacklistResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QString AddBlacklistResult::id() const
{
	return d->m_id;
}

void AddBlacklistResult::setId(const QString& id)
{
	d->m_id = id;
}

QString AddBlacklistResult::blacklistId() const
{
	return d->m_blacklistId;
}

void AddBlacklistResult::setBlacklistId(const QString& id)
{
	d->m_blacklistId = id;
}

QString AddBlacklistResult::parterSysId() const
{
	return d->m_partnerSysId;
}

void AddBlacklistResult::setPartnerSysId(const QString& id)
{
	d->m_partnerSysId = id;
}

QString AddBlacklistResult::VESysId() const
{
	return d->m_VESysId;
}

void AddBlacklistResult::setVESysId(const QString& id)
{
	d->m_VESysId = id;
}

QString AddBlacklistResult::displayName() const
{
	return d->m_displayName;
}

void AddBlacklistResult::setDisplayName(const QString& name)
{
	d->m_displayName = name;
}

QString AddBlacklistResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void AddBlacklistResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

//DelBlacklistParam
class DelBlacklistParamPrivate : public QSharedData
{
public:
	DelBlacklistParamPrivate();
	DelBlacklistParamPrivate(const DelBlacklistParamPrivate& rhs);

	QString m_VESysId;
};

DelBlacklistParamPrivate::DelBlacklistParamPrivate() :
	QSharedData(),
	m_VESysId()
{
}

DelBlacklistParamPrivate::DelBlacklistParamPrivate(const DelBlacklistParamPrivate& rhs) :
	QSharedData(rhs),
	m_VESysId(rhs.m_VESysId)
{
}

DelBlacklistParam::DelBlacklistParam() :
	d(new DelBlacklistParamPrivate)
{
}

DelBlacklistParam::DelBlacklistParam(const DelBlacklistParam& rhs) :
	d(rhs.d)
{
}

DelBlacklistParam::~DelBlacklistParam()
{
}

DelBlacklistParam& DelBlacklistParam::operator=(const DelBlacklistParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString DelBlacklistParam::VESysId() const
{
	return d->m_VESysId;
}

void DelBlacklistParam::setVESysId(const QString& id)
{
	d->m_VESysId = id;
}

bool DelBlacklistParam::operator==(const DelBlacklistParam& rhs) const
{
	return d->m_VESysId == rhs.d->m_VESysId;
}

bool DelBlacklistParam::operator!=(const DelBlacklistParam& rhs) const
{
	return !operator==(rhs);
}

// DelBlacklistResult
class DelBlacklistResultPrivate : public QSharedData
{
public:
	DelBlacklistResultPrivate();
	DelBlacklistResultPrivate(const DelBlacklistResultPrivate& rhs);

	CommonResult m_common;
};

DelBlacklistResultPrivate::DelBlacklistResultPrivate() :
	QSharedData()
{
}

DelBlacklistResultPrivate::DelBlacklistResultPrivate(const DelBlacklistResultPrivate& rhs) :
	QSharedData(rhs)
{
}

DelBlacklistResult::DelBlacklistResult() :
	d(new DelBlacklistResultPrivate)
{
}

DelBlacklistResult::DelBlacklistResult(const DelBlacklistResult& rhs) :
	d(rhs.d)
{
}

DelBlacklistResult::~DelBlacklistResult()
{
}

DelBlacklistResult& DelBlacklistResult::operator=(const DelBlacklistResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult DelBlacklistResult::common() const
{
	return d->m_common;
}

void DelBlacklistResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

// GetMemberInfoParam
class GetMemberInfoParamPrivate : public QSharedData
{
public:
	GetMemberInfoParamPrivate();
	GetMemberInfoParamPrivate(const GetMemberInfoParamPrivate& rhs);

	QString m_VESysId;
};

GetMemberInfoParamPrivate::GetMemberInfoParamPrivate() :
	QSharedData(),
	m_VESysId()
{
}

GetMemberInfoParamPrivate::GetMemberInfoParamPrivate(const GetMemberInfoParamPrivate& rhs) :
	QSharedData(rhs),
	m_VESysId(rhs.m_VESysId)
{
}

GetMemberInfoParam::GetMemberInfoParam() :
	d(new GetMemberInfoParamPrivate)
{
}

GetMemberInfoParam::GetMemberInfoParam(const GetMemberInfoParam& rhs) :
	d(rhs.d)
{
}

GetMemberInfoParam::~GetMemberInfoParam()
{
}

GetMemberInfoParam& GetMemberInfoParam::operator=(const GetMemberInfoParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString GetMemberInfoParam::VESysId() const
{
	return d->m_VESysId;
}

void GetMemberInfoParam::setVESysId(const QString& id)
{
	d->m_VESysId = id;
}

bool GetMemberInfoParam::operator==(const GetMemberInfoParam& rhs) const
{
	return d->m_VESysId == rhs.d->m_VESysId;
}

bool GetMemberInfoParam::operator!=(const GetMemberInfoParam& rhs) const
{
	return !operator==(rhs);
}

// GetMemberInfoResult
class GetMemberInfoResultPrivate : public QSharedData
{
public:
	GetMemberInfoResultPrivate();
	GetMemberInfoResultPrivate(const GetMemberInfoResultPrivate& rhs);

	CommonResult m_common;
	QString m_VESysId;
	QString m_VELoginId;
	QString m_callerId;
	QString m_displayName;
	QString m_nativeLanguage;
	QString m_country;
	QString m_major;
	QString m_smallPhoto;
};

GetMemberInfoResultPrivate::GetMemberInfoResultPrivate() :
	QSharedData(),
	m_VESysId(),
	m_VELoginId(),
	m_callerId(),
	m_displayName(),
	m_nativeLanguage(),
	m_country(),
	m_major(),
	m_smallPhoto()
{
}

GetMemberInfoResultPrivate::GetMemberInfoResultPrivate(const GetMemberInfoResultPrivate& rhs) :
	QSharedData(rhs),
	m_VESysId(rhs.m_VESysId),
	m_VELoginId(rhs.m_VELoginId),
	m_callerId(rhs.m_callerId),
	m_displayName(rhs.m_displayName),
	m_nativeLanguage(rhs.m_nativeLanguage),
	m_country(rhs.m_country),
	m_major(rhs.m_major),
	m_smallPhoto(rhs.m_smallPhoto)
{
}

GetMemberInfoResult::GetMemberInfoResult() :
	d(new GetMemberInfoResultPrivate)
{
}

GetMemberInfoResult::GetMemberInfoResult(const GetMemberInfoResult& rhs) :
	d(rhs.d)
{
}

GetMemberInfoResult::~GetMemberInfoResult()
{
}

GetMemberInfoResult& GetMemberInfoResult::operator=(const GetMemberInfoResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult GetMemberInfoResult::common() const
{
	return d->m_common;
}

void GetMemberInfoResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QString GetMemberInfoResult::VESysId() const
{
	return d->m_VESysId;
}

void GetMemberInfoResult::setVESysId(const QString& id)
{
	d->m_VESysId = id;
}

QString GetMemberInfoResult::VELoginId() const
{
	return d->m_VELoginId;
}

void GetMemberInfoResult::setVELoginId(const QString& id)
{
	d->m_VELoginId = id;
}

QString GetMemberInfoResult::callerId() const
{
	return d->m_callerId;
}

void GetMemberInfoResult::setCallerId(const QString& id)
{
	d->m_callerId = id;
}

QString GetMemberInfoResult::displayName() const
{
	return d->m_displayName;
}

void GetMemberInfoResult::setDisplayName(const QString& name)
{
	d->m_displayName = name;
}

QString GetMemberInfoResult::nativeLanguage() const
{
	return d->m_nativeLanguage;
}

void GetMemberInfoResult::setNativeLanguage(const QString& lang)
{
	d->m_nativeLanguage = lang;
}

QString GetMemberInfoResult::country() const
{
	return d->m_country;
}

void GetMemberInfoResult::setCountry(const QString& country)
{
	d->m_country = country;
}

QString GetMemberInfoResult::major() const
{
	return d->m_major;
}

void GetMemberInfoResult::setMajor(const QString& major)
{
	d->m_major = major;
}

QString GetMemberInfoResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void GetMemberInfoResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

//GetUploadPhotoParam
class GetUploadPhotoParamPrivate : public QSharedData
{
public:
	GetUploadPhotoParamPrivate();
	GetUploadPhotoParamPrivate(const GetUploadPhotoParamPrivate& rhs);

	QString m_callerId;
	QString m_photoId;
};

GetUploadPhotoParamPrivate::GetUploadPhotoParamPrivate() :
	QSharedData(),
	m_callerId(),
	m_photoId()
{

}

GetUploadPhotoParamPrivate::GetUploadPhotoParamPrivate(const GetUploadPhotoParamPrivate& rhs) :
	QSharedData(rhs),
	m_callerId(rhs.m_callerId),
	m_photoId(rhs.m_photoId)
{

}

GetUploadPhotoParam::GetUploadPhotoParam() :
	d(new GetUploadPhotoParamPrivate)
{
}

GetUploadPhotoParam::GetUploadPhotoParam(const GetUploadPhotoParam& rhs) :
	d(rhs.d)
{
}

GetUploadPhotoParam::~GetUploadPhotoParam()
{
}

GetUploadPhotoParam& GetUploadPhotoParam::operator=(const GetUploadPhotoParam& rhs)
{
	d = rhs.d;
	return *this;
}

QString GetUploadPhotoParam::callerId() const
{
	return d->m_callerId;
}

void GetUploadPhotoParam::setCallerId(const QString& id)
{
	d->m_callerId = id;
}

QString GetUploadPhotoParam::photoId() const
{
	return d->m_photoId;
}

void GetUploadPhotoParam::setPhotoId(const QString& id)
{
	d->m_photoId = id;
}

bool GetUploadPhotoParam::operator==(const GetUploadPhotoParam& rhs) const
{
	return d->m_callerId == rhs.d->m_callerId;
}

bool GetUploadPhotoParam::operator!=(const GetUploadPhotoParam& rhs) const
{
	return !operator==(rhs);
}

//GetUploadPhotoResult
class GetUploadPhotoResultPrivate : public QSharedData 
{
public:
	GetUploadPhotoResultPrivate();
	GetUploadPhotoResultPrivate(const GetUploadPhotoResultPrivate& rhs);

	CommonResult m_common;
	QString m_callerId;
	QString m_photoId;
	QString m_expire;
	QString m_smallPhoto;
	QString m_largePhoto;
};

GetUploadPhotoResultPrivate::GetUploadPhotoResultPrivate() :
	QSharedData(),
	m_callerId(),
	m_photoId(),
	m_expire(),
	m_smallPhoto(),
	m_largePhoto()
{

}
	
GetUploadPhotoResultPrivate::GetUploadPhotoResultPrivate(const GetUploadPhotoResultPrivate& rhs) :
	QSharedData(rhs),
	m_callerId(rhs.m_callerId),
	m_photoId(rhs.m_photoId),
	m_expire(rhs.m_expire),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto)
{
	
}

GetUploadPhotoResult::GetUploadPhotoResult() :
	d(new GetUploadPhotoResultPrivate)
{

}

GetUploadPhotoResult::GetUploadPhotoResult(const GetUploadPhotoResult& rhs) :
	d(rhs.d)
{

}

GetUploadPhotoResult::~GetUploadPhotoResult()
{

}

GetUploadPhotoResult& GetUploadPhotoResult::operator=(const GetUploadPhotoResult& rhs)
{
	d = rhs.d;
	return *this;
}

CommonResult GetUploadPhotoResult::common() const 
{
	return d->m_common;
}

void GetUploadPhotoResult::setCommon(const CommonResult& result)
{
	d->m_common = result;
}

QString GetUploadPhotoResult::expire() const 
{
	return d->m_expire;
}

void GetUploadPhotoResult::setExpire(const QString& expire)
{
	d->m_expire = expire;
}

QString GetUploadPhotoResult::callerId() const
{
	return d->m_callerId;
}

void GetUploadPhotoResult::setCallerId(const QString& id)
{
	d->m_callerId = id;
}

QString GetUploadPhotoResult::photoId() const
{
	return d->m_photoId;
}

void GetUploadPhotoResult::setPhotoId(const QString& id)
{
	d->m_photoId = id;
}

QString GetUploadPhotoResult::smallPhoto() const 
{
	return d->m_smallPhoto;
}

void GetUploadPhotoResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString GetUploadPhotoResult::largePhoto() const 
{
	return d->m_largePhoto;
}

void GetUploadPhotoResult::setLargePhoto(const QString& photo)
{
	d->m_largePhoto = photo;
}

//VersionParam
class VersionParamPrivate : public QSharedData
{
public:
	VersionParamPrivate();
	VersionParamPrivate(const VersionParamPrivate& rhs);
};

VersionParamPrivate::VersionParamPrivate() :
	QSharedData()
{
}

VersionParamPrivate::VersionParamPrivate(const VersionParamPrivate& rhs) :
	QSharedData(rhs)
{
}

VersionParam::VersionParam() :
	d(new VersionParamPrivate)
{
}

VersionParam::VersionParam(const VersionParam& rhs) :
	d(rhs.d)
{
}

VersionParam::~VersionParam()
{
}

VersionParam& VersionParam::operator=(const VersionParam& rhs)
{
	d = rhs.d;
	return *this;
}

// VersionResult
class VersionResultPrivate : public QSharedData
{
public:
	VersionResultPrivate();
	VersionResultPrivate(const VersionResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;

	QString m_number;
	QString m_link;
};

VersionResultPrivate::VersionResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg(),
	m_number(),
	m_link()
{
}

VersionResultPrivate::VersionResultPrivate(const VersionResultPrivate& rhs) :
	QSharedData(rhs),
	m_number(rhs.m_number),
	m_link(rhs.m_link)
{
}

VersionResult::VersionResult() :
	d(new VersionResultPrivate)
{
}

VersionResult::VersionResult(const VersionResult& rhs) :
	d(rhs.d)
{
}

VersionResult::~VersionResult()
{
}

QString VersionResult::error() const
{
	return d->m_error;
}

void VersionResult::setError(const QString& err)
{
	d->m_error = err;
}

QString VersionResult::errorMsg() const
{
	return d->m_errorMsg;
}

void VersionResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

QString VersionResult::number() const
{
	return d->m_number;
}

void VersionResult::setNumber(const QString& number)
{
	d->m_number = number;
}

QString VersionResult::link() const
{
	return d->m_link;
}

void VersionResult::setLink(const QString& link)
{
	d->m_link = link;
}

VersionResult& VersionResult::operator=(const VersionResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneLoadConfigParam
class PhoneLoadConfigParamPrivate : public QSharedData
{
public:
	PhoneLoadConfigParamPrivate();
	PhoneLoadConfigParamPrivate(const PhoneLoadConfigParamPrivate& rhs);

	QString m_lastTime;
	int m_reloadConfig;
};

PhoneLoadConfigParamPrivate::PhoneLoadConfigParamPrivate() :
	QSharedData(),
	m_lastTime(),
	m_reloadConfig(0)
{
}

PhoneLoadConfigParamPrivate::PhoneLoadConfigParamPrivate(const PhoneLoadConfigParamPrivate& rhs) :
	QSharedData(rhs),
	m_lastTime(rhs.m_lastTime),
	m_reloadConfig(rhs.m_reloadConfig)
{
}

PhoneLoadConfigParam::PhoneLoadConfigParam() :
	d(new PhoneLoadConfigParamPrivate)
{
}

PhoneLoadConfigParam::PhoneLoadConfigParam(const PhoneLoadConfigParam& rhs) :
	d(rhs.d)
{
}

PhoneLoadConfigParam::~PhoneLoadConfigParam()
{
}

QString PhoneLoadConfigParam::lastTime() const
{
	return d->m_lastTime;
}

void PhoneLoadConfigParam::setLastTime(const QString& lastTime)
{
	d->m_lastTime = lastTime;
}

int PhoneLoadConfigParam::reloadConfig() const
{
	return d->m_reloadConfig;
}

void PhoneLoadConfigParam::setReloadConfig(int reload)
{
	d->m_reloadConfig = reload;
}

PhoneLoadConfigParam& PhoneLoadConfigParam::operator=(const PhoneLoadConfigParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneLoadConfigParam::operator==(const PhoneLoadConfigParam& rhs) const
{
	return d->m_lastTime == rhs.d->m_lastTime
		&& d->m_reloadConfig == rhs.d->m_reloadConfig;
}

bool PhoneLoadConfigParam::operator!=(const PhoneLoadConfigParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneLoadConfigResult
class PhoneLoadConfigResultPrivate : public QSharedData
{
public:
	PhoneLoadConfigResultPrivate();
	PhoneLoadConfigResultPrivate(const PhoneLoadConfigResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;

	QVariant m_serviceLanguage;
	QVariant m_majorArray;
	QVariant m_useApp;
	QVariant m_callMode;
	QString m_version;
	QString m_downloadUrl;
	QString m_forceUpdate;
	QString m_verifyCode;
	QString m_versionName;
	QStringList m_PSTNSupported;
	QString m_createAccountUrl;
	QString m_changePasswordUrl;
	QString m_editUserInfoUrl;
};

PhoneLoadConfigResultPrivate::PhoneLoadConfigResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg(),
	m_serviceLanguage(),
	m_majorArray(),
	m_useApp(),
	m_callMode(),
	m_version(),
	m_downloadUrl(),
	m_forceUpdate(),
	m_verifyCode(),
	m_versionName(),
	m_PSTNSupported(),
	m_createAccountUrl(),
	m_changePasswordUrl(),
	m_editUserInfoUrl()
{
}

PhoneLoadConfigResultPrivate::PhoneLoadConfigResultPrivate(const PhoneLoadConfigResultPrivate& rhs) :
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg),
	m_serviceLanguage(rhs.m_serviceLanguage),
	m_majorArray(rhs.m_majorArray),
	m_useApp(rhs.m_useApp),
	m_callMode(rhs.m_callMode),
	m_version(rhs.m_version),
	m_downloadUrl(rhs.m_downloadUrl),
	m_forceUpdate(rhs.m_forceUpdate),
	m_verifyCode(rhs.m_verifyCode),
	m_versionName(rhs.m_versionName),
	m_PSTNSupported(rhs.m_PSTNSupported),
	m_createAccountUrl(rhs.m_createAccountUrl),
	m_changePasswordUrl(rhs.m_changePasswordUrl),
	m_editUserInfoUrl(rhs.m_editUserInfoUrl)
{
}

PhoneLoadConfigResult::PhoneLoadConfigResult() : 
	d(new PhoneLoadConfigResultPrivate)
{
}

PhoneLoadConfigResult::PhoneLoadConfigResult(const PhoneLoadConfigResult& rhs) :
	d(rhs.d)
{
}

PhoneLoadConfigResult::~PhoneLoadConfigResult()
{
}

QString PhoneLoadConfigResult::error() const
{
	return d->m_error;
}

void PhoneLoadConfigResult::setError(const QString& err)
{
	d->m_error = err;
}

QString PhoneLoadConfigResult::errorMsg() const
{
	return d->m_errorMsg;
}

void PhoneLoadConfigResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

QVariant PhoneLoadConfigResult::serviceLanguage() const
{
	return d->m_serviceLanguage;
}

void PhoneLoadConfigResult::setServiceLanguage(const QVariant& lang)
{
	d->m_serviceLanguage = lang;
}

QVariant PhoneLoadConfigResult::majorArray() const
{
	return d->m_majorArray;
}

void PhoneLoadConfigResult::setMajorArray(const QVariant& array)
{
	d->m_majorArray = array;
}

QVariant PhoneLoadConfigResult::useApp() const
{
	return d->m_useApp;
}

void PhoneLoadConfigResult::setUseApp(const QVariant& useApp)
{
	d->m_useApp = useApp;
}

QVariant PhoneLoadConfigResult::callMode() const
{
	return d->m_callMode;
}

void PhoneLoadConfigResult::setCallMode(const QVariant& callMode)
{
	d->m_callMode = callMode;
}

QString PhoneLoadConfigResult::version() const
{
	return d->m_version;
}

void PhoneLoadConfigResult::setVersion(const QString& version)
{
	d->m_version = version;
}

QString PhoneLoadConfigResult::downloadUrl() const
{
	return d->m_downloadUrl;
}

void PhoneLoadConfigResult::setDownloadUrl(const QString& url)
{
	d->m_downloadUrl = url;
}

QString PhoneLoadConfigResult::forceUpdate() const
{
	return d->m_forceUpdate;
}

void PhoneLoadConfigResult::setForceUpdate(const QString& force)
{
	d->m_forceUpdate = force;
}

QString PhoneLoadConfigResult::verifyCode() const
{
	return d->m_verifyCode;
}

void PhoneLoadConfigResult::setVerifyCode(const QString& code)
{
	d->m_verifyCode = code;
}

QString PhoneLoadConfigResult::versionName() const
{
	return d->m_versionName;
}

void PhoneLoadConfigResult::setVersionName(const QString& name)
{
	d->m_versionName = name;
}

QStringList PhoneLoadConfigResult::PSTNSupported() const
{
	return d->m_PSTNSupported;
}

void PhoneLoadConfigResult::setPSTNSupported(const QStringList& supported)
{
	d->m_PSTNSupported = supported;
}

QString PhoneLoadConfigResult::createAccountUrl() const
{
	return d->m_createAccountUrl;
}

void PhoneLoadConfigResult::setCreateAccountUrl(const QString& url)
{
	d->m_createAccountUrl = url;
}

QString PhoneLoadConfigResult::changePasswordUrl() const
{
	return d->m_changePasswordUrl;
}

void PhoneLoadConfigResult::setChangePasswordUrl(const QString& url)
{
	d->m_changePasswordUrl = url;
}

QString PhoneLoadConfigResult::editUserInfoUrl() const
{
	return d->m_editUserInfoUrl;
}

void PhoneLoadConfigResult::setEditUserInfoUrl(const QString& url)
{
	d->m_editUserInfoUrl = url;
}

PhoneLoadConfigResult& PhoneLoadConfigResult::operator=(const PhoneLoadConfigResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneRegisterParam
class PhoneRegisterParamPrivate : public QSharedData
{
public:
	PhoneRegisterParamPrivate();
	PhoneRegisterParamPrivate(const PhoneRegisterParamPrivate& rhs);

	QString m_nativeLanguage;
	QString m_voiceraID;
	QString m_email;
	QString m_password;
	QString m_photo;
};

PhoneRegisterParamPrivate::PhoneRegisterParamPrivate() :
	QSharedData(),
	m_nativeLanguage(),
	m_voiceraID(),
	m_email(),
	m_password(),
	m_photo()
{
}

PhoneRegisterParamPrivate::PhoneRegisterParamPrivate(const PhoneRegisterParamPrivate& rhs) :
	QSharedData(rhs),
	m_nativeLanguage(rhs.m_nativeLanguage),
	m_voiceraID(rhs.m_voiceraID),
	m_email(rhs.m_email),
	m_password(rhs.m_password),
	m_photo(rhs.m_photo)
{
}

PhoneRegisterParam::PhoneRegisterParam() :
	d(new PhoneRegisterParamPrivate)
{
}

PhoneRegisterParam::PhoneRegisterParam(const PhoneRegisterParam& rhs) :
	d(rhs.d)
{
}

PhoneRegisterParam::~PhoneRegisterParam()
{
}

QString PhoneRegisterParam::nativeLanguage() const
{
	return d->m_nativeLanguage;
}

void PhoneRegisterParam::setNativeLanguage(const QString& language)
{
	d->m_nativeLanguage = language;
}

QString PhoneRegisterParam::voiceraID() const
{
	return d->m_voiceraID;
}

void PhoneRegisterParam::setVoiceraID(const QString& id)
{
	d->m_voiceraID = id;
}

QString PhoneRegisterParam::email() const
{
	return d->m_email;
}

void PhoneRegisterParam::setEmail(const QString& email)
{
	d->m_email = email;
}

QString PhoneRegisterParam::password() const
{
	return d->m_password;
}

void PhoneRegisterParam::setPassword(const QString& password)
{
	d->m_password = password;
}

QString PhoneRegisterParam::photo() const
{
	return d->m_photo;
}

void PhoneRegisterParam::setPhoto(const QString& photo)
{
	d->m_photo = photo;
}

PhoneRegisterParam& PhoneRegisterParam::operator=(const PhoneRegisterParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneRegisterParam::operator==(const PhoneRegisterParam& rhs) const
{
	return d->m_nativeLanguage == rhs.d->m_nativeLanguage
		&& d->m_voiceraID == rhs.d->m_voiceraID
		&& d->m_email == rhs.d->m_email
		&& d->m_password == rhs.d->m_password
		&& d->m_photo == rhs.d->m_photo;
}

bool PhoneRegisterParam::operator!=(const PhoneRegisterParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneRegisterResult
class PhoneRegisterResultPrivate : public QSharedData
{
public:
	PhoneRegisterResultPrivate();
	PhoneRegisterResultPrivate(const PhoneRegisterResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;
	QString m_SIPCallerID;
	QString m_nickname;
	QString m_voiceraID;
	QString m_nativeLanguage;
	QString m_smallPhoto;
	QString m_largePhoto;
};

PhoneRegisterResultPrivate::PhoneRegisterResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg(),
	m_SIPCallerID(),
	m_nickname(),
	m_voiceraID(),
	m_nativeLanguage(),
	m_smallPhoto(),
	m_largePhoto()
{
}

PhoneRegisterResultPrivate::PhoneRegisterResultPrivate(const PhoneRegisterResultPrivate& rhs) :
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg),
	m_SIPCallerID(rhs.m_SIPCallerID),
	m_nickname(rhs.m_nickname),
	m_voiceraID(rhs.m_voiceraID),
	m_nativeLanguage(rhs.m_nativeLanguage),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto)
{
}

PhoneRegisterResult::PhoneRegisterResult() :
	d(new PhoneRegisterResultPrivate)
{
}

PhoneRegisterResult::PhoneRegisterResult(const PhoneRegisterResult& rhs) :
	d(rhs.d)
{
}

PhoneRegisterResult::~PhoneRegisterResult()
{
}

QString PhoneRegisterResult::error() const
{
	return d->m_error;
}

void PhoneRegisterResult::setError(const QString& err)
{
	d->m_error = err;
}

QString PhoneRegisterResult::errorMsg() const
{
	return d->m_errorMsg;
}

void PhoneRegisterResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

QString PhoneRegisterResult::SIPCallerID() const
{
	return d->m_SIPCallerID;
}

void PhoneRegisterResult::setSIPCallerID(const QString& id)
{
	d->m_SIPCallerID = id;
}

QString PhoneRegisterResult::nickname() const
{
	return d->m_nickname;
}

void PhoneRegisterResult::setNickname(const QString& nickname)
{
	d->m_nickname = nickname;
}

QString PhoneRegisterResult::voiceraID() const
{
	return d->m_voiceraID;
}

void PhoneRegisterResult::setVoiceraID(const QString& id)
{
	d->m_voiceraID = id;
}

QString PhoneRegisterResult::nativeLanguage() const
{
	return d->m_nativeLanguage;
}

void PhoneRegisterResult::setNativeLanguage(const QString& language)
{
	d->m_nativeLanguage = language;
}

QString PhoneRegisterResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void PhoneRegisterResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString PhoneRegisterResult::largePhoto() const
{
	return d->m_largePhoto;
}

void PhoneRegisterResult::setLargePhoto(const QString& photo)
{
	d->m_largePhoto = photo;
}

PhoneRegisterResult& PhoneRegisterResult::operator=(const PhoneRegisterResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneLoginParam
class PhoneLoginParamPrivate : public QSharedData
{
public:
	PhoneLoginParamPrivate();
	PhoneLoginParamPrivate(const PhoneLoginParamPrivate& rhs);

	QString m_voiceraID;
	QString m_password;
};

PhoneLoginParamPrivate::PhoneLoginParamPrivate() :
	QSharedData(),
	m_voiceraID(),
	m_password()
{
}

PhoneLoginParamPrivate::PhoneLoginParamPrivate(const PhoneLoginParamPrivate& rhs) :
	QSharedData(rhs),
	m_voiceraID(rhs.m_voiceraID),
	m_password(rhs.m_password)
{
}

PhoneLoginParam::PhoneLoginParam() :
	d(new PhoneLoginParamPrivate)
{
}

PhoneLoginParam::PhoneLoginParam(const PhoneLoginParam& rhs) :
	d(rhs.d)
{
}

PhoneLoginParam::~PhoneLoginParam()
{
}

QString PhoneLoginParam::voiceraID() const
{
	return d->m_voiceraID;
}

void PhoneLoginParam::setVoiceraID(const QString& id)
{
	d->m_voiceraID = id;
}

QString PhoneLoginParam::password() const
{
	return d->m_password;
}

void PhoneLoginParam::setPassword(const QString& password)
{
	d->m_password = password;
}

PhoneLoginParam& PhoneLoginParam::operator=(const PhoneLoginParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneLoginParam::operator==(const PhoneLoginParam& rhs) const
{
	return d->m_voiceraID == rhs.d->m_voiceraID
		&& d->m_password == rhs.d->m_password;
}

bool PhoneLoginParam::operator!=(const PhoneLoginParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneLoginResult
class PhoneLoginResultPrivate : public QSharedData
{
public:
	PhoneLoginResultPrivate();
	PhoneLoginResultPrivate(const PhoneLoginResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;
	QString m_SIPCallerID;
	QString m_nativeLanguage;
	QString m_voiceraID;
	QString m_smallPhoto;
	QString m_largePhoto;
};

PhoneLoginResultPrivate::PhoneLoginResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg(),
	m_SIPCallerID(),
	m_nativeLanguage(),
	m_voiceraID(),
	m_smallPhoto(),
	m_largePhoto()
{
}

PhoneLoginResultPrivate::PhoneLoginResultPrivate(const PhoneLoginResultPrivate& rhs) :
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg),
	m_SIPCallerID(rhs.m_SIPCallerID),
	m_nativeLanguage(rhs.m_nativeLanguage),
	m_voiceraID(rhs.m_voiceraID),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto)
{
}

PhoneLoginResult::PhoneLoginResult() :
	d(new PhoneLoginResultPrivate)
{
}

PhoneLoginResult::PhoneLoginResult(const PhoneLoginResult& rhs) :
	d(rhs.d)
{
}

PhoneLoginResult::~PhoneLoginResult()
{
}

QString PhoneLoginResult::error() const
{
	return d->m_error;
}

void PhoneLoginResult::setError(const QString& err)
{
	d->m_error = err;
}

QString PhoneLoginResult::errorMsg() const
{
	return d->m_errorMsg;
}

void PhoneLoginResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

QString PhoneLoginResult::SIPCallerID() const
{
	return d->m_SIPCallerID;
}

void PhoneLoginResult::setSIPCallerID(const QString& id)
{
	d->m_SIPCallerID = id;
}

QString PhoneLoginResult::nativeLanguage() const
{
	return d->m_nativeLanguage;
}

void PhoneLoginResult::setNativeLanguage(const QString& language)
{
	d->m_nativeLanguage = language;
}

QString PhoneLoginResult::voiceraID() const
{
	return d->m_voiceraID;
}

void PhoneLoginResult::setVoiceraID(const QString& id)
{
	d->m_voiceraID = id;
}

QString PhoneLoginResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void PhoneLoginResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString PhoneLoginResult::largePhoto() const
{
	return d->m_largePhoto;
}

void PhoneLoginResult::setLargePhoto(const QString& photo)
{
	d->m_largePhoto = photo;
}

PhoneLoginResult& PhoneLoginResult::operator=(const PhoneLoginResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneForgetPasswordParam
class PhoneForgetPasswordParamPrivate : public QSharedData
{
public:
	PhoneForgetPasswordParamPrivate();
	PhoneForgetPasswordParamPrivate(const PhoneForgetPasswordParamPrivate& rhs);

	QString m_email;
};

PhoneForgetPasswordParamPrivate::PhoneForgetPasswordParamPrivate() :
	QSharedData(),
	m_email()
{
}

PhoneForgetPasswordParamPrivate::PhoneForgetPasswordParamPrivate(const PhoneForgetPasswordParamPrivate& rhs) :
	QSharedData(rhs),
	m_email(rhs.m_email)
{
}

PhoneForgetPasswordParam::PhoneForgetPasswordParam() :
	d(new PhoneForgetPasswordParamPrivate)
{
}

PhoneForgetPasswordParam::PhoneForgetPasswordParam(const PhoneForgetPasswordParam& rhs) :
	d(rhs.d)
{
}

PhoneForgetPasswordParam::~PhoneForgetPasswordParam()
{
}

QString PhoneForgetPasswordParam::email() const
{
	return d->m_email;
}

void PhoneForgetPasswordParam::setEmail(const QString& email)
{
	d->m_email = email;
}

PhoneForgetPasswordParam& PhoneForgetPasswordParam::operator=(const PhoneForgetPasswordParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneForgetPasswordParam::operator==(const PhoneForgetPasswordParam& rhs) const
{
	return d->m_email == rhs.d->m_email;
}

bool PhoneForgetPasswordParam::operator!=(const PhoneForgetPasswordParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneForgetPasswordResult
class PhoneForgetPasswordResultPrivate : public QSharedData
{
public:
	PhoneForgetPasswordResultPrivate();
	PhoneForgetPasswordResultPrivate(const PhoneForgetPasswordResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;
};

PhoneForgetPasswordResultPrivate::PhoneForgetPasswordResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg()
{
}

PhoneForgetPasswordResultPrivate::PhoneForgetPasswordResultPrivate(const PhoneForgetPasswordResultPrivate& rhs) :
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg)
{
}

PhoneForgetPasswordResult::PhoneForgetPasswordResult() :
	d(new PhoneForgetPasswordResultPrivate)
{
}

PhoneForgetPasswordResult::PhoneForgetPasswordResult(const PhoneForgetPasswordResult& rhs) :
	d(rhs.d)
{
}

PhoneForgetPasswordResult::~PhoneForgetPasswordResult()
{
}

QString PhoneForgetPasswordResult::error() const
{
	return d->m_error;
}

void PhoneForgetPasswordResult::setError(const QString& err)
{
	d->m_error = err;
}

QString PhoneForgetPasswordResult::errorMsg() const
{
	return d->m_errorMsg;
}

void PhoneForgetPasswordResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

PhoneForgetPasswordResult& PhoneForgetPasswordResult::operator=(const PhoneForgetPasswordResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneContactListParam
class PhoneContactListParamPrivate : public QSharedData
{
public:
	PhoneContactListParamPrivate();
	PhoneContactListParamPrivate(const PhoneContactListParamPrivate& rhs);

	QString m_searchType;
};

PhoneContactListParamPrivate::PhoneContactListParamPrivate() :
	QSharedData(),
	m_searchType()
{
}

PhoneContactListParamPrivate::PhoneContactListParamPrivate(const PhoneContactListParamPrivate& rhs) :
	QSharedData(rhs),
	m_searchType(rhs.m_searchType)
{
}

PhoneContactListParam::PhoneContactListParam() :
	d(new PhoneContactListParamPrivate)
{
}

PhoneContactListParam::PhoneContactListParam(const PhoneContactListParam& rhs) :
	d(rhs.d)
{
}

PhoneContactListParam::~PhoneContactListParam()
{
}

QString PhoneContactListParam::searchType() const
{
	return d->m_searchType;
}

void PhoneContactListParam::setSearchType(const QString& type)
{
	d->m_searchType = type;
}

PhoneContactListParam& PhoneContactListParam::operator=(const PhoneContactListParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneContactListParam::operator==(const PhoneContactListParam& rhs) const
{
	return d->m_searchType == rhs.d->m_searchType;
}

bool PhoneContactListParam::operator!=(const PhoneContactListParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneContactListResult
class PhoneContactListResultPrivate : public QSharedData
{
public:
	PhoneContactListResultPrivate();
	PhoneContactListResultPrivate(const PhoneContactListResultPrivate& rhs);

	QString m_ID;
	QString m_addBookID;
	QString m_contactName;
	QString m_smallPhoto;
	QString m_largePhoto;
	QVariant m_phone;
	QString m_voiceraID;
	QString m_SIPCallerID;
	QString m_sourceLanguage;
	QString m_targetLanguage;
	QString m_onlineStatus;
	QString m_systemUser;
};

PhoneContactListResultPrivate::PhoneContactListResultPrivate() :
	QSharedData(),
	m_ID(),
	m_addBookID(),
	m_contactName(),
	m_smallPhoto(),
	m_largePhoto(),
	m_phone(),
	m_voiceraID(),
	m_SIPCallerID(),
	m_sourceLanguage(),
	m_targetLanguage(),
	m_onlineStatus(),
	m_systemUser()
{
}

PhoneContactListResultPrivate::PhoneContactListResultPrivate(const PhoneContactListResultPrivate& rhs) :
	QSharedData(rhs),
	m_ID(rhs.m_ID),
	m_addBookID(rhs.m_addBookID),
	m_contactName(rhs.m_contactName),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto),
	m_phone(rhs.m_phone),
	m_voiceraID(rhs.m_voiceraID),
	m_SIPCallerID(rhs.m_SIPCallerID),
	m_sourceLanguage(rhs.m_sourceLanguage),
	m_targetLanguage(rhs.m_targetLanguage),
	m_onlineStatus(rhs.m_onlineStatus),
	m_systemUser(rhs.m_systemUser)
{
}

PhoneContactListResult::PhoneContactListResult() :
	d(new PhoneContactListResultPrivate)
{
}

PhoneContactListResult::PhoneContactListResult(const PhoneContactListResult& rhs) :
	d(rhs.d)
{
}

PhoneContactListResult::~PhoneContactListResult()
{
}

QString PhoneContactListResult::ID() const
{
	return d->m_ID;
}

void PhoneContactListResult::setID(const QString& id)
{
	d->m_ID = id;
}

QString PhoneContactListResult::addBookID() const
{
	return d->m_addBookID;
}

void PhoneContactListResult::setAddBookID(const QString& id)
{
	d->m_addBookID = id;
}

QString PhoneContactListResult::contactName() const
{
	return d->m_contactName;
}

void PhoneContactListResult::setContactName(const QString& name)
{
	d->m_contactName = name;
}

QString PhoneContactListResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void PhoneContactListResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString PhoneContactListResult::largePhoto() const
{
	return d->m_largePhoto;
}

void PhoneContactListResult::setLargePhoto(const QString& photo)
{
	d->m_largePhoto = photo;
}

QVariant PhoneContactListResult::phone() const
{
	return d->m_phone;
}

void PhoneContactListResult::setPhone(const QVariant& phone)
{
	d->m_phone = phone;
}

QString PhoneContactListResult::voiceraID() const
{
	return d->m_voiceraID;
}

void PhoneContactListResult::setVoiceraID(const QString& id)
{
	d->m_voiceraID = id;
}

QString PhoneContactListResult::SIPCallerID() const
{
	return d->m_SIPCallerID;
}

void PhoneContactListResult::setSIPCallerID(const QString& id)
{
	d->m_SIPCallerID = id;
}

QString PhoneContactListResult::sourceLanguage() const
{
	return d->m_sourceLanguage;
}

void PhoneContactListResult::setSourceLanguage(const QString& language)
{
	d->m_sourceLanguage = language;
}

QString PhoneContactListResult::targetLanguage() const
{
	return d->m_targetLanguage;
}

void PhoneContactListResult::setTargetLanguage(const QString& language)
{
	d->m_targetLanguage = language;
}

QString PhoneContactListResult::onlineStatus() const
{
	return d->m_onlineStatus;
}

void PhoneContactListResult::setOnlineStatus(const QString& status)
{
	d->m_onlineStatus = status;
}

QString PhoneContactListResult::systemUser() const
{
	return d->m_systemUser;
}

void PhoneContactListResult::setSystemUser(const QString& system)
{
	d->m_systemUser = system;
}

PhoneContactListResult& PhoneContactListResult::operator=(const PhoneContactListResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneAddContactParam
class PhoneAddContactParamPrivate : public QSharedData
{
public:
	PhoneAddContactParamPrivate();
	PhoneAddContactParamPrivate(const PhoneAddContactParamPrivate& rhs);

	QString m_voiceraID;
	QString m_name;
	QString m_phone;
	QString m_phoneCountry;
	QString m_sourceLanguage;
	QString m_targetLanguage;
};

PhoneAddContactParamPrivate::PhoneAddContactParamPrivate() :
	QSharedData(),
	m_voiceraID(),
	m_name(),
	m_phone(),
	m_phoneCountry(),
	m_sourceLanguage(),
	m_targetLanguage()
{
}

PhoneAddContactParamPrivate::PhoneAddContactParamPrivate(const PhoneAddContactParamPrivate& rhs) :
	QSharedData(rhs),
	m_voiceraID(rhs.m_voiceraID),
	m_name(rhs.m_name),
	m_phone(rhs.m_phone),
	m_phoneCountry(rhs.m_phoneCountry),
	m_sourceLanguage(rhs.m_sourceLanguage),
	m_targetLanguage(rhs.m_targetLanguage)
{
}

PhoneAddContactParam::PhoneAddContactParam() :
	d(new PhoneAddContactParamPrivate)
{
}

PhoneAddContactParam::PhoneAddContactParam(const PhoneAddContactParam& rhs) :
	d(rhs.d)
{
}

PhoneAddContactParam::~PhoneAddContactParam()
{
}

QString PhoneAddContactParam::voiceraID() const
{
	return d->m_voiceraID;
}

void PhoneAddContactParam::setVoiceraID(const QString& id)
{
	d->m_voiceraID = id;
}

QString PhoneAddContactParam::name() const
{
	return d->m_name;
}

void PhoneAddContactParam::setName(const QString& name)
{
	d->m_name = name;
}

QString PhoneAddContactParam::phone() const
{
	return d->m_phone;
}

void PhoneAddContactParam::setPhone(const QString& phone)
{
	d->m_phone = phone;
}

QString PhoneAddContactParam::phoneCountry() const
{
	return d->m_phoneCountry;
}

void PhoneAddContactParam::setPhoneCountry(const QString& country)
{
	d->m_phoneCountry = country;
}

QString PhoneAddContactParam::sourceLanguage() const
{
	return d->m_sourceLanguage;
}

void PhoneAddContactParam::setSourceLanguage(const QString& language)
{
	d->m_sourceLanguage = language;
}

QString PhoneAddContactParam::targetLanguage() const
{
	return d->m_targetLanguage;
}

void PhoneAddContactParam::setTargetLanguage(const QString& language)
{
	d->m_targetLanguage = language;
}

PhoneAddContactParam& PhoneAddContactParam::operator=(const PhoneAddContactParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneAddContactParam::operator==(const PhoneAddContactParam& rhs) const
{
	return d->m_voiceraID == rhs.d->m_voiceraID
		&& d->m_name == rhs.d->m_name
		&& d->m_phone == rhs.d->m_phone
		&& d->m_phoneCountry == rhs.d->m_phoneCountry
		&& d->m_sourceLanguage == rhs.d->m_sourceLanguage
		&& d->m_targetLanguage == rhs.d->m_targetLanguage;
}

bool PhoneAddContactParam::operator!=(const PhoneAddContactParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneAddContactResult
class PhoneAddContactResultPrivate : public QSharedData
{
public:
	PhoneAddContactResultPrivate();
	PhoneAddContactResultPrivate(const PhoneAddContactResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;
	QString m_addBookID;
	QString m_ID;
};

PhoneAddContactResultPrivate::PhoneAddContactResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg(),
	m_addBookID(),
	m_ID()
{
}

PhoneAddContactResultPrivate::PhoneAddContactResultPrivate(const PhoneAddContactResultPrivate& rhs) :
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg),
	m_addBookID(rhs.m_addBookID),
	m_ID(rhs.m_ID)
{
}

PhoneAddContactResult::PhoneAddContactResult() :
	d(new PhoneAddContactResultPrivate)
{
}

PhoneAddContactResult::PhoneAddContactResult(const PhoneAddContactResult& rhs) :
	d(rhs.d)
{
}

PhoneAddContactResult::~PhoneAddContactResult()
{
}

QString PhoneAddContactResult::error() const
{
	return d->m_error;
}

void PhoneAddContactResult::setError(const QString& err)
{
	d->m_error = err;
}

QString PhoneAddContactResult::errorMsg() const
{
	return d->m_errorMsg;
}

void PhoneAddContactResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

QString PhoneAddContactResult::addBookID() const
{
	return d->m_addBookID;
}

void PhoneAddContactResult::setAddBookID(const QString& id)
{
	d->m_addBookID = id;
}

QString PhoneAddContactResult::ID() const
{
	return d->m_ID;
}

void PhoneAddContactResult::setID(const QString& id)
{
	d->m_ID = id;
}

PhoneAddContactResult& PhoneAddContactResult::operator=(const PhoneAddContactResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneUpdateContactParam
class PhoneUpdateContactParamPrivate : public QSharedData
{
public:
	PhoneUpdateContactParamPrivate();
	PhoneUpdateContactParamPrivate(const PhoneUpdateContactParamPrivate& rhs);

	QString m_addBookID;
	QString m_updateType;
	QString m_voiceraID;
	QString m_name;
	QString m_phone;
	QString m_oldPhone;
	QString m_phoneCountry;
	QString m_deletePhone;
	QString m_sourceLanguage;
	QString m_targetLanguage;
};

PhoneUpdateContactParamPrivate::PhoneUpdateContactParamPrivate() :
	QSharedData(),
	m_addBookID(),
	m_updateType(),
	m_voiceraID(),
	m_name(),
	m_phone(),
	m_oldPhone(),
	m_phoneCountry(),
	m_deletePhone(),
	m_sourceLanguage(),
	m_targetLanguage()
{
}

PhoneUpdateContactParamPrivate::PhoneUpdateContactParamPrivate(const PhoneUpdateContactParamPrivate& rhs) :
	QSharedData(rhs),
	m_addBookID(rhs.m_addBookID),
	m_updateType(rhs.m_updateType),
	m_voiceraID(rhs.m_voiceraID),
	m_name(rhs.m_name),
	m_phone(rhs.m_phone),
	m_oldPhone(rhs.m_oldPhone),
	m_phoneCountry(rhs.m_phoneCountry),
	m_deletePhone(rhs.m_deletePhone),
	m_sourceLanguage(rhs.m_sourceLanguage),
	m_targetLanguage(rhs.m_targetLanguage)
{
}

PhoneUpdateContactParam::PhoneUpdateContactParam() :
	d(new PhoneUpdateContactParamPrivate)
{
}

PhoneUpdateContactParam::PhoneUpdateContactParam(const PhoneUpdateContactParam& rhs) :
	d(rhs.d)
{
}

PhoneUpdateContactParam::~PhoneUpdateContactParam()
{
}

QString PhoneUpdateContactParam::addBookID() const
{
	return d->m_addBookID;
}

void PhoneUpdateContactParam::setAddBookID(const QString& id)
{
	d->m_addBookID = id;
}

QString PhoneUpdateContactParam::updateType() const
{
	return d->m_updateType;
}

void PhoneUpdateContactParam::setUpdateType(const QString& type)
{
	d->m_updateType = type;
}

QString PhoneUpdateContactParam::voiceraID() const
{
	return d->m_voiceraID;
}

void PhoneUpdateContactParam::setVoiceraID(const QString& id)
{
	d->m_voiceraID = id;
}

QString PhoneUpdateContactParam::name() const
{
	return d->m_name;
}

void PhoneUpdateContactParam::setName(const QString& name)
{
	d->m_name = name;
}

QString PhoneUpdateContactParam::phone() const
{
	return d->m_phone;
}

void PhoneUpdateContactParam::setPhone(const QString& phone)
{
	d->m_phone = phone;
}

QString PhoneUpdateContactParam::oldPhone() const
{
	return d->m_oldPhone;
}

void PhoneUpdateContactParam::setOldPhone(const QString& phone)
{
	d->m_oldPhone = phone;
}

QString PhoneUpdateContactParam::phoneCountry() const
{
	return d->m_phoneCountry;
}

void PhoneUpdateContactParam::setPhoneCountry(const QString& country)
{
	d->m_phoneCountry = country;
}

QString PhoneUpdateContactParam::deletePhone() const
{
	return d->m_deletePhone;
}

void PhoneUpdateContactParam::setDeletePhone(const QString& phone)
{
	d->m_deletePhone = phone;
}

QString PhoneUpdateContactParam::sourceLanguage() const
{
	return d->m_sourceLanguage;
}

void PhoneUpdateContactParam::setSourceLanguage(const QString& language)
{
	d->m_sourceLanguage = language;
}

QString PhoneUpdateContactParam::targetLanguage() const
{
	return d->m_targetLanguage;
}

void PhoneUpdateContactParam::setTargetLanguage(const QString& language)
{
	d->m_targetLanguage = language;
}

PhoneUpdateContactParam& PhoneUpdateContactParam::operator=(const PhoneUpdateContactParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneUpdateContactParam::operator==(const PhoneUpdateContactParam& rhs) const
{
	return d->m_addBookID == rhs.d->m_addBookID
		&& d->m_updateType == rhs.d->m_updateType
		&& d->m_voiceraID == rhs.d->m_voiceraID
		&& d->m_name == rhs.d->m_name
		&& d->m_phone == rhs.d->m_phone
		&& d->m_oldPhone == rhs.d->m_oldPhone
		&& d->m_phoneCountry == rhs.d->m_phoneCountry
		&& d->m_sourceLanguage == rhs.d->m_sourceLanguage
		&& d->m_targetLanguage == rhs.d->m_targetLanguage;
}

bool PhoneUpdateContactParam::operator!=(const PhoneUpdateContactParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneUpdateContactResult
class PhoneUpdateContactResultPrivate : public QSharedData
{
public:
	PhoneUpdateContactResultPrivate();
	PhoneUpdateContactResultPrivate(const PhoneUpdateContactResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;
};

PhoneUpdateContactResultPrivate::PhoneUpdateContactResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg()
{
}

PhoneUpdateContactResultPrivate::PhoneUpdateContactResultPrivate(const PhoneUpdateContactResultPrivate& rhs) :
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg)
{
}

PhoneUpdateContactResult::PhoneUpdateContactResult() :
	d(new PhoneUpdateContactResultPrivate)
{
}

PhoneUpdateContactResult::PhoneUpdateContactResult(const PhoneUpdateContactResult& rhs) :
	d(rhs.d)
{
}

PhoneUpdateContactResult::~PhoneUpdateContactResult()
{
}

QString PhoneUpdateContactResult::error() const
{
	return d->m_error;
}

void PhoneUpdateContactResult::setError(const QString& err)
{
	d->m_error = err;
}

QString PhoneUpdateContactResult::errorMsg() const
{
	return d->m_errorMsg;
}

void PhoneUpdateContactResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

PhoneUpdateContactResult& PhoneUpdateContactResult::operator=(const PhoneUpdateContactResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneDeleteContactParam
class PhoneDeleteContactParamPrivate : public QSharedData
{
public:
	PhoneDeleteContactParamPrivate();
	PhoneDeleteContactParamPrivate(const PhoneDeleteContactParamPrivate& rhs);

	QString m_addBookID;
};

PhoneDeleteContactParamPrivate::PhoneDeleteContactParamPrivate() :
	QSharedData(),
	m_addBookID()
{
}

PhoneDeleteContactParamPrivate::PhoneDeleteContactParamPrivate(const PhoneDeleteContactParamPrivate& rhs) :
	QSharedData(rhs),
	m_addBookID(rhs.m_addBookID)
{
}

PhoneDeleteContactParam::PhoneDeleteContactParam() :
	d(new PhoneDeleteContactParamPrivate)
{
}

PhoneDeleteContactParam::PhoneDeleteContactParam(const PhoneDeleteContactParam& rhs) :
	d(rhs.d)
{
}

PhoneDeleteContactParam::~PhoneDeleteContactParam()
{
}

QString PhoneDeleteContactParam::addBookID() const
{
	return d->m_addBookID;
}

void PhoneDeleteContactParam::setAddBookID(const QString& id)
{
	d->m_addBookID = id;
}

PhoneDeleteContactParam& PhoneDeleteContactParam::operator=(const PhoneDeleteContactParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneDeleteContactParam::operator==(const PhoneDeleteContactParam& rhs) const
{
	return d->m_addBookID == rhs.d->m_addBookID;
}

bool PhoneDeleteContactParam::operator!=(const PhoneDeleteContactParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneDeleteContactResult
class PhoneDeleteContactResultPrivate : public QSharedData
{
public:
	PhoneDeleteContactResultPrivate();
	PhoneDeleteContactResultPrivate(const PhoneDeleteContactResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;
};

PhoneDeleteContactResultPrivate::PhoneDeleteContactResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg()
{
}

PhoneDeleteContactResultPrivate::PhoneDeleteContactResultPrivate(const PhoneDeleteContactResultPrivate& rhs) :
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg)
{
}

PhoneDeleteContactResult::PhoneDeleteContactResult() :
	d(new PhoneDeleteContactResultPrivate)
{
}

PhoneDeleteContactResult::PhoneDeleteContactResult(const PhoneDeleteContactResult& rhs) :
	d(rhs.d)
{
}

PhoneDeleteContactResult::~PhoneDeleteContactResult()
{
}

QString PhoneDeleteContactResult::error() const
{
	return d->m_error;
}

void PhoneDeleteContactResult::setError(const QString& err)
{
	d->m_error = err;
}

QString PhoneDeleteContactResult::errorMsg() const
{
	return d->m_errorMsg;
}

void PhoneDeleteContactResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

PhoneDeleteContactResult& PhoneDeleteContactResult::operator=(const PhoneDeleteContactResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneDetailContactParam
class PhoneDetailContactParamPrivate : public QSharedData
{
public:
	PhoneDetailContactParamPrivate();
	PhoneDetailContactParamPrivate(const PhoneDetailContactParamPrivate& rhs);

	QString m_addBookID;
};

PhoneDetailContactParamPrivate::PhoneDetailContactParamPrivate() :
	QSharedData(),
	m_addBookID()
{
}

PhoneDetailContactParamPrivate::PhoneDetailContactParamPrivate(const PhoneDetailContactParamPrivate& rhs) :
	QSharedData(rhs),
	m_addBookID(rhs.m_addBookID)
{
}

PhoneDetailContactParam::PhoneDetailContactParam() :
	d(new PhoneDetailContactParamPrivate)
{
}

PhoneDetailContactParam::PhoneDetailContactParam(const PhoneDetailContactParam& rhs) :
	d(rhs.d)
{
}

PhoneDetailContactParam::~PhoneDetailContactParam()
{
}

QString PhoneDetailContactParam::addBookID() const
{
	return d->m_addBookID;
}

void PhoneDetailContactParam::setAddBookID(const QString& id)
{
	d->m_addBookID = id;
}

PhoneDetailContactParam& PhoneDetailContactParam::operator=(const PhoneDetailContactParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneDetailContactParam::operator==(const PhoneDetailContactParam& rhs) const
{
	return d->m_addBookID == rhs.d->m_addBookID;
}

bool PhoneDetailContactParam::operator!=(const PhoneDetailContactParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneDetailContactResult
class PhoneDetailContactResultPrivate : public QSharedData
{
public:
	PhoneDetailContactResultPrivate();
	PhoneDetailContactResultPrivate(const PhoneDetailContactResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;
	QString m_nativeLanguage;
	QString m_nickname;
	QString m_country;
	QString m_major;
};

PhoneDetailContactResultPrivate::PhoneDetailContactResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg(),
	m_nativeLanguage(),
	m_nickname(),
	m_country(),
	m_major()
{
}

PhoneDetailContactResultPrivate::PhoneDetailContactResultPrivate(const PhoneDetailContactResultPrivate& rhs) :
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg),
	m_nativeLanguage(rhs.m_nativeLanguage),
	m_nickname(rhs.m_nickname),
	m_country(rhs.m_country),
	m_major(rhs.m_major)
{
}

PhoneDetailContactResult::PhoneDetailContactResult() :
	d(new PhoneDetailContactResultPrivate)
{
}

PhoneDetailContactResult::PhoneDetailContactResult(const PhoneDetailContactResult& rhs) :
	d(rhs.d)
{
}

PhoneDetailContactResult::~PhoneDetailContactResult()
{
}

QString PhoneDetailContactResult::error() const
{
	return d->m_error;
}

void PhoneDetailContactResult::setError(const QString& err)
{
	d->m_error = err;
}

QString PhoneDetailContactResult::errorMsg() const
{
	return d->m_errorMsg;
}

void PhoneDetailContactResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}

QString PhoneDetailContactResult::nativeLanguage() const
{
	return d->m_nativeLanguage;
}

void PhoneDetailContactResult::setNativeLanguage(const QString& language)
{
	d->m_nativeLanguage = language;
}

QString PhoneDetailContactResult::nickname() const
{
	return d->m_nickname;
}

void PhoneDetailContactResult::setNickname(const QString& nickname)
{
	d->m_nickname = nickname;
}

QString PhoneDetailContactResult::country() const
{
	return d->m_country;
}

void PhoneDetailContactResult::setCountry(const QString& country)
{
	d->m_country = country;
}

QString PhoneDetailContactResult::major() const
{
	return d->m_major;
}

void PhoneDetailContactResult::setMajor(const QString& major)
{
	d->m_major = major;
}

PhoneDetailContactResult& PhoneDetailContactResult::operator=(const PhoneDetailContactResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneGetHistoryCDRParam
class PhoneGetHistoryCDRParamPrivate : public QSharedData
{
public:
	PhoneGetHistoryCDRParamPrivate();
	PhoneGetHistoryCDRParamPrivate(const PhoneGetHistoryCDRParamPrivate& rhs);

	QString m_historyType;
};

PhoneGetHistoryCDRParamPrivate::PhoneGetHistoryCDRParamPrivate() :
	QSharedData(),
	m_historyType()
{
}

PhoneGetHistoryCDRParamPrivate::PhoneGetHistoryCDRParamPrivate(const PhoneGetHistoryCDRParamPrivate& rhs) :
	QSharedData(rhs),
	m_historyType(rhs.m_historyType)
{
}

PhoneGetHistoryCDRParam::PhoneGetHistoryCDRParam() :
	d(new PhoneGetHistoryCDRParamPrivate)
{
}

PhoneGetHistoryCDRParam::PhoneGetHistoryCDRParam(const PhoneGetHistoryCDRParam& rhs) :
	d(rhs.d)
{
}

PhoneGetHistoryCDRParam::~PhoneGetHistoryCDRParam()
{
}

QString PhoneGetHistoryCDRParam::historyType() const
{
	return d->m_historyType;
}

void PhoneGetHistoryCDRParam::setHistoryType(const QString& type)
{
	d->m_historyType = type;
}

PhoneGetHistoryCDRParam& PhoneGetHistoryCDRParam::operator=(const PhoneGetHistoryCDRParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneGetHistoryCDRParam::operator==(const PhoneGetHistoryCDRParam& rhs) const
{
	return d->m_historyType == rhs.d->m_historyType;
}

bool PhoneGetHistoryCDRParam::operator!=(const PhoneGetHistoryCDRParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneGetHistoryCDRResult
class PhoneGetHistoryCDRResultPrivate : public QSharedData
{
public:
	PhoneGetHistoryCDRResultPrivate();
	PhoneGetHistoryCDRResultPrivate(const PhoneGetHistoryCDRResultPrivate& rhs);

	QString m_ID;
	QString m_meetCDRID;
	QString m_calleeID;
	QString m_sourceLanguage;
	QString m_targetLanguage;
	QString m_callingTime;
	QString m_cost;
	QString m_isSuccess;
	QString m_isCallout;
};

PhoneGetHistoryCDRResultPrivate::PhoneGetHistoryCDRResultPrivate() :
	QSharedData(),
	m_ID(),
	m_meetCDRID(),
	m_calleeID(),
	m_sourceLanguage(),
	m_targetLanguage(),
	m_callingTime(),
	m_cost(),
	m_isSuccess(),
	m_isCallout()
{
}

PhoneGetHistoryCDRResultPrivate::PhoneGetHistoryCDRResultPrivate(const PhoneGetHistoryCDRResultPrivate& rhs) :
	QSharedData(rhs),
	m_ID(rhs.m_ID),
	m_meetCDRID(rhs.m_meetCDRID),
	m_calleeID(rhs.m_calleeID),
	m_sourceLanguage(rhs.m_sourceLanguage),
	m_targetLanguage(rhs.m_targetLanguage),
	m_callingTime(rhs.m_callingTime),
	m_cost(rhs.m_cost),
	m_isSuccess(rhs.m_isSuccess),
	m_isCallout(rhs.m_isCallout)
{
}

PhoneGetHistoryCDRResult::PhoneGetHistoryCDRResult() :
	d(new PhoneGetHistoryCDRResultPrivate)
{
}

PhoneGetHistoryCDRResult::PhoneGetHistoryCDRResult(const PhoneGetHistoryCDRResult& rhs) :
	d(rhs.d)
{
}

PhoneGetHistoryCDRResult::~PhoneGetHistoryCDRResult()
{
}

QString PhoneGetHistoryCDRResult::ID() const
{
	return d->m_ID;
}

void PhoneGetHistoryCDRResult::setID(const QString& id)
{
	d->m_ID = id;
}

QString PhoneGetHistoryCDRResult::meetCDRID() const
{
	return d->m_meetCDRID;
}

void PhoneGetHistoryCDRResult::setMeetCDRID(const QString& id)
{
	d->m_meetCDRID = id;
}

QString PhoneGetHistoryCDRResult::calleeID() const
{
	return d->m_calleeID;
}

void PhoneGetHistoryCDRResult::setCalleeID(const QString& id)
{
	d->m_calleeID = id;
}

QString PhoneGetHistoryCDRResult::sourceLanguage() const
{
	return d->m_sourceLanguage;
}

void PhoneGetHistoryCDRResult::setSourceLanguage(const QString& language)
{
	d->m_sourceLanguage = language;
}

QString PhoneGetHistoryCDRResult::targetLanguage() const
{
	return d->m_targetLanguage;
}

void PhoneGetHistoryCDRResult::setTargetLanguage(const QString& language)
{
	d->m_targetLanguage = language;
}

QString PhoneGetHistoryCDRResult::callingTime() const
{
	return d->m_callingTime;
}

void PhoneGetHistoryCDRResult::setCallingTime(const QString& callingTime)
{
	d->m_callingTime = callingTime;
}

QString PhoneGetHistoryCDRResult::cost() const
{
	return d->m_cost;
}

void PhoneGetHistoryCDRResult::setCost(const QString& cost)
{
	d->m_cost = cost;
}

QString PhoneGetHistoryCDRResult::isSuccess() const
{
	return d->m_isSuccess;
}

void PhoneGetHistoryCDRResult::setIsSuccess(const QString& success)
{
	d->m_isSuccess = success;
}

QString PhoneGetHistoryCDRResult::isCallout() const
{
	return d->m_isCallout;
}

void PhoneGetHistoryCDRResult::setIsCallout(const QString& callout)
{
	d->m_isCallout = callout;
}

PhoneGetHistoryCDRResult& PhoneGetHistoryCDRResult::operator=(const PhoneGetHistoryCDRResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneGetUserInfoParam
class PhoneGetUserInfoParamPrivate : public QSharedData
{
public:
	PhoneGetUserInfoParamPrivate();
	PhoneGetUserInfoParamPrivate(const PhoneGetUserInfoParamPrivate& rhs);

	QString m_voiceraID;
};

PhoneGetUserInfoParamPrivate::PhoneGetUserInfoParamPrivate() :
	QSharedData(),
	m_voiceraID()
{
}

PhoneGetUserInfoParamPrivate::PhoneGetUserInfoParamPrivate(const PhoneGetUserInfoParamPrivate& rhs) :
	QSharedData(rhs),
	m_voiceraID(rhs.m_voiceraID)
{
}

PhoneGetUserInfoParam::PhoneGetUserInfoParam() :
	d(new PhoneGetUserInfoParamPrivate)
{
}

PhoneGetUserInfoParam::PhoneGetUserInfoParam(const PhoneGetUserInfoParam& rhs) :
	d(rhs.d)
{
}

PhoneGetUserInfoParam::~PhoneGetUserInfoParam()
{
}

QString PhoneGetUserInfoParam::voiceraID() const
{
	return d->m_voiceraID;
}

void PhoneGetUserInfoParam::setVoiceraID(const QString& id)
{
	d->m_voiceraID = id;
}

PhoneGetUserInfoParam& PhoneGetUserInfoParam::operator=(const PhoneGetUserInfoParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneGetUserInfoParam::operator==(const PhoneGetUserInfoParam& rhs) const
{
	return d->m_voiceraID == rhs.d->m_voiceraID;
}

bool PhoneGetUserInfoParam::operator!=(const PhoneGetUserInfoParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneGetUserInfoResult
class PhoneGetUserInfoResultPrivate : public QSharedData
{
public:
	PhoneGetUserInfoResultPrivate();
	PhoneGetUserInfoResultPrivate(const PhoneGetUserInfoResultPrivate& rhs);

	QString m_email;
	QString m_voideraID;
	QString m_SIPCallerID;
	QString m_balance;
	QString m_nickname;
	QString m_nativeLanguage;
	QString m_major;
	QString m_country;
	QString m_smallPhoto;
	QString m_largePhoto;
	QString m_onlineStatus;
};

PhoneGetUserInfoResultPrivate::PhoneGetUserInfoResultPrivate() :
	QSharedData(),
	m_email(),
	m_voideraID(),
	m_SIPCallerID(),
	m_balance(),
	m_nickname(),
	m_nativeLanguage(),
	m_major(),
	m_country(),
	m_smallPhoto(),
	m_largePhoto(),
	m_onlineStatus()
{
}

PhoneGetUserInfoResultPrivate::PhoneGetUserInfoResultPrivate(const PhoneGetUserInfoResultPrivate& rhs) :
	QSharedData(rhs),
	m_email(rhs.m_email),
	m_voideraID(rhs.m_voideraID),
	m_SIPCallerID(rhs.m_SIPCallerID),
	m_balance(rhs.m_balance),
	m_nickname(rhs.m_nickname),
	m_nativeLanguage(rhs.m_nativeLanguage),
	m_major(rhs.m_major),
	m_country(rhs.m_country),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto),
	m_onlineStatus(rhs.m_onlineStatus)
{
}

PhoneGetUserInfoResult::PhoneGetUserInfoResult() :
	d(new PhoneGetUserInfoResultPrivate)
{
}

PhoneGetUserInfoResult::PhoneGetUserInfoResult(const PhoneGetUserInfoResult& rhs) :
	d(rhs.d)
{
}

PhoneGetUserInfoResult::~PhoneGetUserInfoResult()
{
}

QString PhoneGetUserInfoResult::email() const
{
	return d->m_email;
}

void PhoneGetUserInfoResult::setEmail(const QString& email)
{
	d->m_email = email;
}

QString PhoneGetUserInfoResult::voiceraID() const
{
	return d->m_voideraID;
}

void PhoneGetUserInfoResult::setVoiceraID(const QString& id)
{
	d->m_voideraID = id;
}

QString PhoneGetUserInfoResult::SIPCallerID() const
{
	return d->m_SIPCallerID;
}

void PhoneGetUserInfoResult::setSIPCallerID(const QString& callerID)
{
	d->m_SIPCallerID = callerID;
}

QString PhoneGetUserInfoResult::balance() const
{
	return d->m_balance;
}

void PhoneGetUserInfoResult::setBalance(const QString& balance)
{
	d->m_balance = balance;
}

QString PhoneGetUserInfoResult::nickname() const
{
	return d->m_nickname;
}

void PhoneGetUserInfoResult::setNickname(const QString& nickname)
{
	d->m_nickname = nickname;
}

QString PhoneGetUserInfoResult::nativeLanguage() const
{
	return d->m_nativeLanguage;
}

void PhoneGetUserInfoResult::setNativeLanguage(const QString& language)
{
	d->m_nativeLanguage = language;
}

QString PhoneGetUserInfoResult::major() const
{
	return d->m_major;
}

void PhoneGetUserInfoResult::setMajor(const QString& major)
{
	d->m_major = major;
}

QString PhoneGetUserInfoResult::country() const
{
	return d->m_country;
}

void PhoneGetUserInfoResult::setCountry(const QString& country)
{
	d->m_country = country;
}

QString PhoneGetUserInfoResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void PhoneGetUserInfoResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString PhoneGetUserInfoResult::largePhoto() const
{
	return d->m_largePhoto;
}

void PhoneGetUserInfoResult::setLargePhoto(const QString& photo)
{
	d->m_largePhoto = photo;
}

QString PhoneGetUserInfoResult::onlineStatus() const
{
	return d->m_onlineStatus;
}

void PhoneGetUserInfoResult::setOnlineStatus(const QString& status)
{
	d->m_onlineStatus = status;
}

PhoneGetUserInfoResult& PhoneGetUserInfoResult::operator=(const PhoneGetUserInfoResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneEditUserInfoParam
class PhoneEditUserInfoParamPrivate : public QSharedData
{
public:
	PhoneEditUserInfoParamPrivate();
	PhoneEditUserInfoParamPrivate(const PhoneEditUserInfoParamPrivate& rhs);

	QString m_nickname;
	QString m_nativeLanguage;
	QString m_nationlity;
	QString m_major;
	QString m_onlineStatus;
	QString m_photo;
	QString m_updateType;
};

PhoneEditUserInfoParamPrivate::PhoneEditUserInfoParamPrivate() :
	QSharedData(),
	m_nickname(),
	m_nativeLanguage(),
	m_nationlity(),
	m_major(),
	m_onlineStatus(),
	m_photo(),
	m_updateType()
{
}

PhoneEditUserInfoParamPrivate::PhoneEditUserInfoParamPrivate(const PhoneEditUserInfoParamPrivate& rhs) :
	QSharedData(rhs),
	m_nickname(rhs.m_nickname),
	m_nativeLanguage(rhs.m_nativeLanguage),
	m_nationlity(rhs.m_nationlity),
	m_major(rhs.m_major),
	m_onlineStatus(rhs.m_onlineStatus),
	m_photo(rhs.m_photo),
	m_updateType(rhs.m_updateType)
{
}

PhoneEditUserInfoParam::PhoneEditUserInfoParam() :
	d(new PhoneEditUserInfoParamPrivate)
{
}

PhoneEditUserInfoParam::PhoneEditUserInfoParam(const PhoneEditUserInfoParam& rhs) :
	d(rhs.d)
{
}

PhoneEditUserInfoParam::~PhoneEditUserInfoParam()
{
}

QString PhoneEditUserInfoParam::nickname() const
{
	return d->m_nickname;
}

void PhoneEditUserInfoParam::setNickname(const QString& nickname)
{
	d->m_nickname = nickname;
}

QString PhoneEditUserInfoParam::nativeLanguage() const
{
	return d->m_nativeLanguage;
}

void PhoneEditUserInfoParam::setNativeLanguage(const QString& language)
{
	d->m_nativeLanguage = language;
}

QString PhoneEditUserInfoParam::nationlity() const
{
	return d->m_nationlity;
}

void PhoneEditUserInfoParam::setNationlity(const QString& nation)
{
	d->m_nationlity = nation;
}

QString PhoneEditUserInfoParam::major() const
{
	return d->m_major;
}

void PhoneEditUserInfoParam::setMajor(const QString& major)
{
	d->m_major = major;
}

QString PhoneEditUserInfoParam::onlineStatus() const
{
	return d->m_onlineStatus;
}

void PhoneEditUserInfoParam::setOnlineStatus(const QString& status)
{
	d->m_onlineStatus = status;
}

QString PhoneEditUserInfoParam::photo() const
{
	return d->m_photo;
}

void PhoneEditUserInfoParam::setPhoto(const QString& photo)
{
	d->m_photo = photo;
}

QString PhoneEditUserInfoParam::updateType() const
{
	return d->m_updateType;
}

void PhoneEditUserInfoParam::setUpdateType(const QString& type)
{
	d->m_updateType = type;
}

PhoneEditUserInfoParam& PhoneEditUserInfoParam::operator=(const PhoneEditUserInfoParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneEditUserInfoParam::operator==(const PhoneEditUserInfoParam& rhs) const
{
	return d->m_nickname == rhs.d->m_nickname
		&& d->m_nativeLanguage == rhs.d->m_nativeLanguage
		&& d->m_nationlity == rhs.d->m_nationlity
		&& d->m_major == rhs.d->m_major
		&& d->m_onlineStatus == rhs.d->m_onlineStatus
		&& d->m_photo == rhs.d->m_photo
		&& d->m_updateType == rhs.d->m_updateType;
}

bool PhoneEditUserInfoParam::operator!=(const PhoneEditUserInfoParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneEditUserInfoResult
class PhoneEditUserInfoResultPrivate : public QSharedData
{
public:
	PhoneEditUserInfoResultPrivate();
	PhoneEditUserInfoResultPrivate(const PhoneEditUserInfoResultPrivate& rhs);
};

PhoneEditUserInfoResultPrivate::PhoneEditUserInfoResultPrivate() :
	QSharedData()
{
}

PhoneEditUserInfoResultPrivate::PhoneEditUserInfoResultPrivate(const PhoneEditUserInfoResultPrivate& rhs) :
	QSharedData(rhs)
{
}

PhoneEditUserInfoResult::PhoneEditUserInfoResult() :
	d(new PhoneEditUserInfoResultPrivate)
{
}

PhoneEditUserInfoResult::PhoneEditUserInfoResult(const PhoneEditUserInfoResult& rhs) :
	d(rhs.d)
{
}

PhoneEditUserInfoResult::~PhoneEditUserInfoResult()
{
}

PhoneEditUserInfoResult& PhoneEditUserInfoResult::operator=(const PhoneEditUserInfoResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneChangePasswordParam
class PhoneChangePasswordParamPrivate : public QSharedData
{
public:
	PhoneChangePasswordParamPrivate();
	PhoneChangePasswordParamPrivate(const PhoneChangePasswordParamPrivate& rhs);

	QString m_authCode;
	QString m_email;
};

PhoneChangePasswordParamPrivate::PhoneChangePasswordParamPrivate() :
	QSharedData(),
	m_authCode(),
	m_email()
{
}

PhoneChangePasswordParamPrivate::PhoneChangePasswordParamPrivate(const PhoneChangePasswordParamPrivate& rhs) :
	QSharedData(rhs),
	m_authCode(rhs.m_authCode),
	m_email(rhs.m_email)
{
}

PhoneChangePasswordParam::PhoneChangePasswordParam() :
d(new PhoneChangePasswordParamPrivate)
{
}

PhoneChangePasswordParam::PhoneChangePasswordParam(const PhoneChangePasswordParam& rhs) :
d(rhs.d)
{
}

PhoneChangePasswordParam::~PhoneChangePasswordParam()
{
}

QString PhoneChangePasswordParam::authCode() const
{
	return d->m_authCode;
}

void PhoneChangePasswordParam::setAuthCode(const QString& code)
{
	d->m_authCode = code;
}

QString PhoneChangePasswordParam::email() const
{
	return d->m_email;
}

void PhoneChangePasswordParam::setEmail(const QString& email)
{
	d->m_email = email;
}

PhoneChangePasswordParam& PhoneChangePasswordParam::operator=(const PhoneChangePasswordParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneChangePasswordParam::operator==(const PhoneChangePasswordParam& rhs) const
{
	return d->m_authCode == rhs.d->m_authCode
		&& d->m_email == rhs.d->m_email;
}

bool PhoneChangePasswordParam::operator!=(const PhoneChangePasswordParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneChangePasswordResult
class PhoneChangePasswordResultPrivate : public QSharedData
{
public:
	PhoneChangePasswordResultPrivate();
	PhoneChangePasswordResultPrivate(const PhoneChangePasswordResultPrivate& rhs);
};

PhoneChangePasswordResultPrivate::PhoneChangePasswordResultPrivate() :
	QSharedData()
{
}

PhoneChangePasswordResultPrivate::PhoneChangePasswordResultPrivate(const PhoneChangePasswordResultPrivate& rhs) :
	QSharedData(rhs)
{
}

PhoneChangePasswordResult::PhoneChangePasswordResult() :
	d(new PhoneChangePasswordResultPrivate)
{
}

PhoneChangePasswordResult::PhoneChangePasswordResult(const PhoneChangePasswordResult& rhs) :
	d(rhs.d)
{
}

PhoneChangePasswordResult::~PhoneChangePasswordResult()
{
}

PhoneChangePasswordResult& PhoneChangePasswordResult::operator=(const PhoneChangePasswordResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneNewPasswordParam
class PhoneNewPasswordParamPrivate : public QSharedData
{
public:
	PhoneNewPasswordParamPrivate();
	PhoneNewPasswordParamPrivate(const PhoneNewPasswordParamPrivate& rhs);

	QString m_newPassword1;
	QString m_newPassword2;
	QString m_autoCode;
	QString m_email;
};

PhoneNewPasswordParamPrivate::PhoneNewPasswordParamPrivate() :
	QSharedData(),
	m_newPassword1(),
	m_newPassword2(),
	m_autoCode(),
	m_email()
{
}

PhoneNewPasswordParamPrivate::PhoneNewPasswordParamPrivate(const PhoneNewPasswordParamPrivate& rhs) :
	QSharedData(rhs),
	m_newPassword1(rhs.m_newPassword1),
	m_newPassword2(rhs.m_newPassword2),
	m_autoCode(rhs.m_autoCode),
	m_email(rhs.m_email)
{
}

PhoneNewPasswordParam::PhoneNewPasswordParam() :
	d(new PhoneNewPasswordParamPrivate)
{
}

PhoneNewPasswordParam::PhoneNewPasswordParam(const PhoneNewPasswordParam& rhs) :
	d(rhs.d)
{
}

PhoneNewPasswordParam::~PhoneNewPasswordParam()
{
}

QString PhoneNewPasswordParam::newPassword1() const
{
	return d->m_newPassword1;
}

void PhoneNewPasswordParam::setNewPassword1(const QString& password)
{
	d->m_newPassword1 = password;
}

QString PhoneNewPasswordParam::newPassword2() const
{
	return d->m_newPassword2;
}

void PhoneNewPasswordParam::setNewPassword2(const QString& password)
{
	d->m_newPassword2 = password;
}

QString PhoneNewPasswordParam::authCode() const
{
	return d->m_autoCode;
}

void PhoneNewPasswordParam::setAuthCode(const QString& code)
{
	d->m_autoCode = code;
}

QString PhoneNewPasswordParam::email() const
{
	return d->m_email;
}

void PhoneNewPasswordParam::setEmail(const QString& email)
{
	d->m_email = email;
}

PhoneNewPasswordParam& PhoneNewPasswordParam::operator=(const PhoneNewPasswordParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneNewPasswordParam::operator==(const PhoneNewPasswordParam& rhs) const
{
	return d->m_newPassword1 == rhs.d->m_newPassword1
		&& d->m_newPassword2 == rhs.d->m_newPassword2
		&& d->m_autoCode == rhs.d->m_autoCode
		&& d->m_email == rhs.d->m_email;
}

bool PhoneNewPasswordParam::operator!=(const PhoneNewPasswordParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneNewPasswordResult
class PhoneNewPasswordResultPrivate : public QSharedData
{
public:
	PhoneNewPasswordResultPrivate();
	PhoneNewPasswordResultPrivate(const PhoneNewPasswordResultPrivate& rhs);
};

PhoneNewPasswordResultPrivate::PhoneNewPasswordResultPrivate() :
	QSharedData()
{
}

PhoneNewPasswordResultPrivate::PhoneNewPasswordResultPrivate(const PhoneNewPasswordResultPrivate& rhs) :
	QSharedData(rhs)
{
}

PhoneNewPasswordResult::PhoneNewPasswordResult() :
	d(new PhoneNewPasswordResultPrivate)
{
}

PhoneNewPasswordResult::PhoneNewPasswordResult(const PhoneNewPasswordResult& rhs) :
	d(rhs.d)
{
}

PhoneNewPasswordResult::~PhoneNewPasswordResult()
{
}

PhoneNewPasswordResult& PhoneNewPasswordResult::operator=(const PhoneNewPasswordResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneGetCDRIDParam
class PhoneGetCDRIDParamPrivate : public QSharedData
{
public:
	PhoneGetCDRIDParamPrivate();
	PhoneGetCDRIDParamPrivate(const PhoneGetCDRIDParamPrivate& rhs);

	QString m_CDRID;
};

PhoneGetCDRIDParamPrivate::PhoneGetCDRIDParamPrivate() :
	QSharedData(),
	m_CDRID()
{
}

PhoneGetCDRIDParamPrivate::PhoneGetCDRIDParamPrivate(const PhoneGetCDRIDParamPrivate& rhs) :
	QSharedData(rhs),
	m_CDRID(rhs.m_CDRID)
{
}

PhoneGetCDRIDParam::PhoneGetCDRIDParam() :
	d(new PhoneGetCDRIDParamPrivate)
{
}

PhoneGetCDRIDParam::PhoneGetCDRIDParam(const PhoneGetCDRIDParam& rhs) :
	d(rhs.d)
{
}

PhoneGetCDRIDParam::~PhoneGetCDRIDParam()
{
}

QString PhoneGetCDRIDParam::CDRID() const
{
	return d->m_CDRID;
}

void PhoneGetCDRIDParam::setCDRID(const QString& id)
{
	d->m_CDRID = id;
}

PhoneGetCDRIDParam& PhoneGetCDRIDParam::operator=(const PhoneGetCDRIDParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneGetCDRIDParam::operator==(const PhoneGetCDRIDParam& rhs) const
{
	return d->m_CDRID == rhs.d->m_CDRID;
}

bool PhoneGetCDRIDParam::operator!=(const PhoneGetCDRIDParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneGetCDRIDResult
class PhoneGetCDRIDResultPrivate : public QSharedData
{
public:
	PhoneGetCDRIDResultPrivate();
	PhoneGetCDRIDResultPrivate(const PhoneGetCDRIDResultPrivate& rhs);

	QString m_ID;
	QString m_callerID;
	QString m_deductionTime;
	QString m_meetCDRID;
	QString m_callTime;
	QString m_contactName;
	QString m_smallPhoto;
	QString m_largePhoto;
	QString m_callStatus;
	QString m_callMode;
	QString m_deductionCost;
	QString m_sourceLanguage;
	QString m_targetLanguage;
	QString m_PSTNCountry;
	QString m_voiceraID;
};

PhoneGetCDRIDResultPrivate::PhoneGetCDRIDResultPrivate() :
	QSharedData(),
	m_ID(),
	m_callerID(),
	m_deductionTime(),
	m_meetCDRID(),
	m_callTime(),
	m_contactName(),
	m_smallPhoto(),
	m_largePhoto(),
	m_callStatus(),
	m_callMode(),
	m_deductionCost(),
	m_sourceLanguage(),
	m_targetLanguage(),
	m_PSTNCountry(),
	m_voiceraID()
{
}

PhoneGetCDRIDResultPrivate::PhoneGetCDRIDResultPrivate(const PhoneGetCDRIDResultPrivate& rhs) :
	QSharedData(rhs),
	m_ID(rhs.m_ID),
	m_callerID(rhs.m_callerID),
	m_deductionTime(rhs.m_deductionTime),
	m_meetCDRID(rhs.m_meetCDRID),
	m_callTime(rhs.m_callTime),
	m_contactName(rhs.m_contactName),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto),
	m_callStatus(rhs.m_callStatus),
	m_callMode(rhs.m_callMode),
	m_deductionCost(rhs.m_deductionCost),
	m_sourceLanguage(rhs.m_sourceLanguage),
	m_targetLanguage(rhs.m_targetLanguage),
	m_PSTNCountry(rhs.m_PSTNCountry),
	m_voiceraID(rhs.m_voiceraID)
{
}

PhoneGetCDRIDResult::PhoneGetCDRIDResult() :
	d(new PhoneGetCDRIDResultPrivate)
{
}

PhoneGetCDRIDResult::PhoneGetCDRIDResult(const PhoneGetCDRIDResult& rhs) :
	d(rhs.d)
{
}

PhoneGetCDRIDResult::~PhoneGetCDRIDResult()
{
}

QString PhoneGetCDRIDResult::ID() const
{
	return d->m_ID;
}

void PhoneGetCDRIDResult::setID(const QString& id)
{
	d->m_ID = id;
}

QString PhoneGetCDRIDResult::callerID() const
{
	return d->m_callerID;
}

void PhoneGetCDRIDResult::setCallerID(const QString& callerID)
{
	d->m_callerID = callerID;
}

QString PhoneGetCDRIDResult::deductionTime() const
{
	return d->m_deductionTime;
}

void PhoneGetCDRIDResult::setDeductionTime(const QString& deductionTime)
{
	d->m_deductionTime = deductionTime;
}

QString PhoneGetCDRIDResult::meetCDRID() const
{
	return d->m_meetCDRID;
}

void PhoneGetCDRIDResult::setMeetCDRID(const QString& id)
{
	d->m_meetCDRID = id;
}

QString PhoneGetCDRIDResult::callTime() const
{
	return d->m_callTime;
}

void PhoneGetCDRIDResult::setCallTime(const QString& callTime)
{
	d->m_callTime = callTime;
}

QString PhoneGetCDRIDResult::contactName() const
{
	return d->m_contactName;
}

void PhoneGetCDRIDResult::setContactName(const QString& name)
{
	d->m_contactName = name;
}

QString PhoneGetCDRIDResult::smallPhoto() const
{
	return d->m_smallPhoto;
}

void PhoneGetCDRIDResult::setSmallPhoto(const QString& photo)
{
	d->m_smallPhoto = photo;
}

QString PhoneGetCDRIDResult::largePhoto() const
{
	return d->m_largePhoto;
}

void PhoneGetCDRIDResult::setLargePhoto(const QString& photo)
{
	d->m_largePhoto = photo;
}

QString PhoneGetCDRIDResult::callStatus() const
{
	return d->m_callStatus;
}

void PhoneGetCDRIDResult::setCallStatus(const QString& status)
{
	d->m_callStatus = status;
}

QString PhoneGetCDRIDResult::callMode() const
{
	return d->m_callMode;
}

void PhoneGetCDRIDResult::setCallMode(const QString& mode)
{
	d->m_callMode = mode;
}

QString PhoneGetCDRIDResult::deductionCost() const
{
	return d->m_deductionCost;
}

void PhoneGetCDRIDResult::setDeductionCost(const QString& cost)
{
	d->m_deductionCost = cost;
}

QString PhoneGetCDRIDResult::sourceLanguage() const
{
	return d->m_sourceLanguage;
}

void PhoneGetCDRIDResult::setSourceLanguage(const QString& language)
{
	d->m_sourceLanguage = language;
}

QString PhoneGetCDRIDResult::targetLanguage() const
{
	return d->m_targetLanguage;
}

void PhoneGetCDRIDResult::setTargetLanguage(const QString& language)
{
	d->m_targetLanguage = language;
}

QString PhoneGetCDRIDResult::PSTNCountry() const
{
	return d->m_PSTNCountry;
}

void PhoneGetCDRIDResult::setPSTNCountry(const QString& country)
{
	d->m_PSTNCountry = country;
}

QString PhoneGetCDRIDResult::voiceraID() const
{
	return d->m_voiceraID;
}

void PhoneGetCDRIDResult::setVoiceraID(const QString& id)
{
	d->m_voiceraID = id;
}

PhoneGetCDRIDResult& PhoneGetCDRIDResult::operator=(const PhoneGetCDRIDResult& rhs)
{
	d = rhs.d;
	return *this;
}

//PhoneDeleteCDRParam
class PhoneDeleteCDRParamPrivate : public QSharedData
{
public:
	PhoneDeleteCDRParamPrivate();
	PhoneDeleteCDRParamPrivate(const PhoneDeleteCDRParamPrivate& rhs);

	QString m_meetCDRID;
};

PhoneDeleteCDRParamPrivate::PhoneDeleteCDRParamPrivate() :
	QSharedData(),
	m_meetCDRID()
{
}

PhoneDeleteCDRParamPrivate::PhoneDeleteCDRParamPrivate(const PhoneDeleteCDRParamPrivate& rhs) :
	QSharedData(rhs),
	m_meetCDRID(rhs.m_meetCDRID)
{
}

PhoneDeleteCDRParam::PhoneDeleteCDRParam() :
	d(new PhoneDeleteCDRParamPrivate)
{
}

PhoneDeleteCDRParam::PhoneDeleteCDRParam(const PhoneDeleteCDRParam& rhs) :
	d(rhs.d)
{
}

PhoneDeleteCDRParam::~PhoneDeleteCDRParam()
{
}

QString PhoneDeleteCDRParam::meetCDRID() const
{
	return d->m_meetCDRID;
}

void PhoneDeleteCDRParam::setMeetCDRID(const QString& id)
{
	d->m_meetCDRID = id;
}

PhoneDeleteCDRParam& PhoneDeleteCDRParam::operator=(const PhoneDeleteCDRParam& rhs)
{
	d = rhs.d;
	return *this;
}

bool PhoneDeleteCDRParam::operator==(const PhoneDeleteCDRParam& rhs) const
{
	return d->m_meetCDRID == rhs.d->m_meetCDRID;
}

bool PhoneDeleteCDRParam::operator!=(const PhoneDeleteCDRParam& rhs) const
{
	return !operator==(rhs);
}

//PhoneDeleteCDRResult
class PhoneDeleteCDRResultPrivate : public QSharedData
{
public:
	PhoneDeleteCDRResultPrivate();
	PhoneDeleteCDRResultPrivate(const PhoneDeleteCDRResultPrivate& rhs);

	QString m_error;
	QString m_errorMsg;
};

PhoneDeleteCDRResultPrivate::PhoneDeleteCDRResultPrivate() :
	QSharedData(),
	m_error(),
	m_errorMsg()
{
}

PhoneDeleteCDRResultPrivate::PhoneDeleteCDRResultPrivate(const PhoneDeleteCDRResultPrivate& rhs) :
	QSharedData(rhs),
	m_error(rhs.m_error),
	m_errorMsg(rhs.m_errorMsg)
{
}

PhoneDeleteCDRResult::PhoneDeleteCDRResult() :
	d(new PhoneDeleteCDRResultPrivate)
{
}

PhoneDeleteCDRResult::PhoneDeleteCDRResult(const PhoneDeleteCDRResult& rhs) :
	d(rhs.d)
{
}

PhoneDeleteCDRResult::~PhoneDeleteCDRResult()
{
}

PhoneDeleteCDRResult& PhoneDeleteCDRResult::operator=(const PhoneDeleteCDRResult& rhs)
{
	d = rhs.d;
	return *this;
}

QString PhoneDeleteCDRResult::error() const
{
	return d->m_error;
}
void PhoneDeleteCDRResult::setError(const QString& err)
{
	d->m_error = err;
}

QString PhoneDeleteCDRResult::errorMsg() const
{
	return d->m_errorMsg;
}

void PhoneDeleteCDRResult::setErrorMsg(const QString& errMsg)
{
	d->m_errorMsg = errMsg;
}


} // namespace ns::Command
} // namespace ns
