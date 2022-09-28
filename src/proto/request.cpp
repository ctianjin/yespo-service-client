#include <QtCore/QSharedData>
#include <QtCore/QString>
#include "request.h"

namespace ns {

//Response Message
class RspMsgPrivate : public QSharedData
{
public:
	RspMsgPrivate();
	RspMsgPrivate(const RspMsgPrivate& rhs);

	int m_rspCode;
	QString m_rspMsg;
};

RspMsgPrivate::RspMsgPrivate() : 
    QSharedData(),
    m_rspCode(0),
    m_rspMsg()
{

}

RspMsgPrivate::RspMsgPrivate(const RspMsgPrivate& rhs) :
    QSharedData(rhs),
	m_rspCode(0),
	m_rspMsg()
{

}

RspMsg::RspMsg() :
    d(new RspMsgPrivate)
{
}

RspMsg::RspMsg(const RspMsg& rhs) :
    d(rhs.d)
{
}

RspMsg::~RspMsg()
{
}

RspMsg& RspMsg::operator=(const RspMsg& rhs)
{
	d = rhs.d;
	return *this;
}

int RspMsg::rspCode() const
{
	return d->m_rspCode;
}

void RspMsg::setRspCode(int val)
{
	d->m_rspCode = val;
}

QString RspMsg::rspMsg() const
{
	return d->m_rspMsg;
}

void RspMsg::setRspMsg(const QString& val)
{
	d->m_rspMsg = val;
}

bool RspMsg::operator==(const RspMsg& rhs) const
{
	return d->m_rspCode == rhs.d->m_rspCode
		&& d->m_rspMsg == rhs.d->m_rspMsg;
}

bool RspMsg::operator!=(const RspMsg& rhs) const
{
	return !operator==(rhs);
}


//Login Request
class LoginReqPrivate : public QSharedData
{
public:
	LoginReqPrivate();
	LoginReqPrivate(const LoginReqPrivate& rhs);

	qint64 m_userId;
	int m_userType;
	QString m_token;
	QString m_deviceToken;
};

LoginReqPrivate::LoginReqPrivate() : 
	QSharedData(),
	m_userId(0),
	m_userType(0),
	m_token(),
	m_deviceToken()
{

}

LoginReqPrivate::LoginReqPrivate(const LoginReqPrivate& rhs) :
    QSharedData(rhs),
	m_userId(rhs.m_userId),
	m_userType(rhs.m_userType),
	m_token(rhs.m_token),
	m_deviceToken(rhs.m_deviceToken)
{

}

LoginReq::LoginReq() :
    d(new LoginReqPrivate)
{
}

LoginReq::LoginReq(const LoginReq& rhs) :
    d(rhs.d)
{
}

LoginReq::~LoginReq()
{
}

LoginReq& LoginReq::operator=(const LoginReq& rhs)
{
	d = rhs.d;
	return *this;
}

qint64 LoginReq::userId() const
{
	return d->m_userId;
}

void LoginReq::setUserId(qint64 val)
{
	d->m_userId = val;
}

int LoginReq::userType() const
{
	return d->m_userType;
}

void LoginReq::setUserType(int val)
{
	d->m_userType = val;
}

QString LoginReq::token() const
{
	return d->m_token;
}

void LoginReq::setToken(const QString& val)
{
	d->m_token = val;
}

QString LoginReq::deviceToken() const
{
	return d->m_deviceToken;
}

void LoginReq::setDeviceToken(const QString& val)
{
	d->m_deviceToken = val;
}

bool LoginReq::operator==(const LoginReq& rhs) const
{
	return d->m_userId == rhs.d->m_userId
		&& d->m_userType == rhs.d->m_userType
		&& d->m_token == rhs.d->m_token
		&& d->m_deviceToken == rhs.d->m_deviceToken;
}

bool LoginReq::operator!=(const LoginReq& rhs) const
{
	return !operator==(rhs);
}

//Call Cancel Request
class CallCancelReqPrivate : public QSharedData
{
public:
	CallCancelReqPrivate();
	CallCancelReqPrivate(const CallCancelReqPrivate& rhs);

	qint64 m_userId;
	qint64 m_partnerId;
};

CallCancelReqPrivate::CallCancelReqPrivate() : 
	QSharedData(),
	m_userId(0),
	m_partnerId(0)
{

}

CallCancelReqPrivate::CallCancelReqPrivate(const CallCancelReqPrivate& rhs) : 
	QSharedData(rhs),
	m_userId(rhs.m_userId),
	m_partnerId(rhs.m_partnerId)
{

}

CallCancelReq::CallCancelReq() :
	d(new CallCancelReqPrivate)
{
	
}

CallCancelReq::CallCancelReq(const CallCancelReq& rhs) :
	d(rhs.d)
{

}

CallCancelReq::~CallCancelReq()
{
}

CallCancelReq& CallCancelReq::operator=(const CallCancelReq& rhs)
{
	d = rhs.d;
	return *this;
}

qint64 CallCancelReq::userId() const 
{
	return d->m_userId;
}

void CallCancelReq::setUesrId(qint64 val)
{
	d->m_userId = val;
}

qint64 CallCancelReq::partnerId() const 
{
	return d->m_partnerId;
}

void CallCancelReq::setPartnerId(qint64 val)
{
	d->m_partnerId = val;
}

bool CallCancelReq::operator==(const CallCancelReq& rhs) const
{
	return d->m_userId == rhs.d->m_userId;
}

bool CallCancelReq::operator!=(const CallCancelReq& rhs) const
{
	return !operator==(rhs);
}

//Photo Notify
class PhotoNotifyPrivate : public QSharedData
{
public:
	PhotoNotifyPrivate();
	PhotoNotifyPrivate(const PhotoNotifyPrivate& rhs);

	QString m_receiver;
	QString m_sender;
	QString m_sendTime;
	QString m_photoId;
	QString m_expire;
	QString m_smallPhoto;
	QString m_largePhoto;
};

PhotoNotifyPrivate::PhotoNotifyPrivate() : 
	QSharedData(),
	m_receiver(),
	m_sender(),
	m_sendTime(),
	m_photoId(),
	m_expire(),
	m_smallPhoto(),
	m_largePhoto()
{

}

PhotoNotifyPrivate::PhotoNotifyPrivate(const PhotoNotifyPrivate& rhs) : 
	QSharedData(rhs),
	m_receiver(rhs.m_receiver),
	m_sender(rhs.m_sender),
	m_sendTime(rhs.m_sendTime),
	m_photoId(rhs.m_photoId),
	m_expire(rhs.m_expire),
	m_smallPhoto(rhs.m_smallPhoto),
	m_largePhoto(rhs.m_largePhoto)
{

}

PhotoNotify::PhotoNotify() :
	d(new PhotoNotifyPrivate)
{
	
}

PhotoNotify::PhotoNotify(const PhotoNotify& rhs) :
	d(rhs.d)
{

}
	
PhotoNotify::~PhotoNotify()
{

}

PhotoNotify& PhotoNotify::operator=(const PhotoNotify& rhs)
{
	d = rhs.d;
	return *this;
}

QString PhotoNotify::receiver() const 
{
	return d->m_receiver;
}

void PhotoNotify::setReceiver(const QString& receiver)
{
	d->m_receiver = receiver;
}

QString PhotoNotify::sender() const
{
	return d->m_sender;
}

void PhotoNotify::setSender(const QString& sender)
{
	d->m_sender = sender;
}

QString PhotoNotify::sendTime() const 
{
	return d->m_sendTime;
}

void PhotoNotify::setSendTime(const QString& sendTime)
{
	d->m_sendTime = sendTime;
}

QString PhotoNotify::photoId() const 
{
	return d->m_photoId;
}

void PhotoNotify::setPhotoId(const QString& id)
{
	d->m_photoId = id;
}

QString PhotoNotify::expire() const 
{
	return d->m_expire;
}

void PhotoNotify::setExpire(const QString& expire)
{
	d->m_expire = expire;
}

QString PhotoNotify::smallPhoto() const 
{
	return d->m_smallPhoto;
}

void PhotoNotify::setSmallPhoto(const QString& smallPhoto)
{
	d->m_smallPhoto = smallPhoto;
}

QString PhotoNotify::largePhoto() const 
{
	return d->m_largePhoto;
}

void PhotoNotify::setLargePhoto(const QString& largePhoto)
{
	d->m_largePhoto = largePhoto;
}

//Logout Request
class LogoutReqPrivate : public QSharedData
{
public:
	LogoutReqPrivate();
	LogoutReqPrivate(const LogoutReqPrivate& rhs);

	qint64 m_userId;
};

LogoutReqPrivate::LogoutReqPrivate() : 
	QSharedData(),
	m_userId(0)
{

}

LogoutReqPrivate::LogoutReqPrivate(const LogoutReqPrivate& rhs) :
	QSharedData(rhs),
	m_userId(rhs.m_userId)
{

}

LogoutReq::LogoutReq() :
    d(new LogoutReqPrivate)
{
}

LogoutReq::LogoutReq(const LogoutReq& rhs) :
    d(rhs.d)
{
}

LogoutReq::~LogoutReq()
{
}

LogoutReq& LogoutReq::operator=(const LogoutReq& rhs)
{
	d = rhs.d;
	return *this;
}

qint64 LogoutReq::userId() const
{
	return d->m_userId;
}

void LogoutReq::setUserId(qint64 val)
{
	d->m_userId = val;
}

bool LogoutReq::operator==(const LogoutReq& rhs) const
{
	return d->m_userId == rhs.d->m_userId;
}

bool LogoutReq::operator!=(const LogoutReq& rhs) const
{
	return !operator==(rhs);
}

//Callee Notify
class CalleeNotifyPrivate : public QSharedData
{
public:
	CalleeNotifyPrivate();
	CalleeNotifyPrivate(const CalleeNotifyPrivate& rhs);

	qint64 m_userId;
	qint64 m_calleeId;
	QString m_sourceLang;
	QString m_targetLang;
	QString m_callMode;
	QString m_token;
	QString m_cdrId;
	QString m_roomAddress;
};

CalleeNotifyPrivate::CalleeNotifyPrivate() :
	QSharedData(),
	m_userId(0),
	m_calleeId(0),
	m_sourceLang(),
	m_targetLang(),
	m_callMode(),
	m_token(),
	m_cdrId(),
	m_roomAddress()
{

}

CalleeNotifyPrivate::CalleeNotifyPrivate(const CalleeNotifyPrivate& rhs) :
	QSharedData(rhs),
	m_userId(rhs.m_userId),
	m_calleeId(rhs.m_calleeId),
	m_sourceLang(rhs.m_sourceLang),
	m_targetLang(rhs.m_targetLang),
	m_callMode(rhs.m_callMode),
	m_token(rhs.m_token),
	m_cdrId(rhs.m_cdrId),
	m_roomAddress(rhs.m_roomAddress)
{

}

CalleeNotify::CalleeNotify() : d(new CalleeNotifyPrivate)
{
	
}

CalleeNotify::CalleeNotify(const CalleeNotify& rhs)
	: d(rhs.d)
{

}

CalleeNotify::~CalleeNotify()
{

}

CalleeNotify& CalleeNotify::operator=(const CalleeNotify& rhs)
{
	d = rhs.d;
	return *this;
}

qint64 CalleeNotify::userId() const 
{
	return d->m_userId;
}

void CalleeNotify::setUserId(qint64 val)
{
	d->m_userId = val;
}

qint64 CalleeNotify::calleeId() const 
{
	return d->m_calleeId;
}

void CalleeNotify::setCalleeId(qint64 val)
{
	d->m_calleeId = val;
}

QString CalleeNotify::roomAddress() const  
{
	return d->m_roomAddress;
}

void CalleeNotify::setRoomAddress(const QString& val)
{
	d->m_roomAddress = val;
}

QString CalleeNotify::deviceToken() const 
{
	return d->m_token;
}

void CalleeNotify::setDeviceToken(const QString& val)
{
	d->m_token = val;
}

QString CalleeNotify::sourceLang() const 
{
	return d->m_sourceLang;
}

void CalleeNotify::setSourceLang(const QString& val)
{
	d->m_sourceLang = val;
}

QString CalleeNotify::targetLang() const 
{
	return d->m_targetLang;
}

void CalleeNotify::setTargetLang(const QString& val)
{
	d->m_targetLang = val;
}

QString CalleeNotify::callMode() const 
{
	return d->m_callMode;
}

void CalleeNotify::setCallMode(const QString& val)
{
	d->m_callMode = val;
}

QString CalleeNotify::cdrId() const 
{
	return d->m_cdrId;
}

void CalleeNotify::setCdrId(const QString& val)
{
	d->m_cdrId = val;
}

//Room Distribute Request
class RoomDistributeReqPrivate : public QSharedData
{
public:
	RoomDistributeReqPrivate();
	RoomDistributeReqPrivate(const RoomDistributeReqPrivate& rhs);

	qint64 m_userId;
	qint64 m_partnerId;
	qint64 m_calleeId;
	QString m_sourceLang;
	QString m_targetLang;
	QString m_callMode;
	QString m_callCountry;
	QString m_countryCode;
	QString m_parentCdrId;
};

RoomDistributeReqPrivate::RoomDistributeReqPrivate() : 
    QSharedData(),
    m_userId(0),
	m_partnerId(0),
	m_calleeId(0),
	m_sourceLang(),
	m_targetLang(),
	m_callMode(),
	m_callCountry(),
	m_countryCode(),
	m_parentCdrId()
{

}

RoomDistributeReqPrivate::RoomDistributeReqPrivate(const RoomDistributeReqPrivate& rhs) :
    QSharedData(rhs),
    m_userId(rhs.m_userId),
	m_partnerId(rhs.m_partnerId),
	m_calleeId(rhs.m_calleeId),
	m_sourceLang(rhs.m_sourceLang),
	m_targetLang(rhs.m_targetLang),
	m_callMode(rhs.m_callMode),
	m_callCountry(rhs.m_callCountry),
	m_countryCode(rhs.m_countryCode),
	m_parentCdrId(rhs.m_parentCdrId)
{

}

RoomDistributeReq::RoomDistributeReq() :
    d(new RoomDistributeReqPrivate)
{
}

RoomDistributeReq::RoomDistributeReq(const RoomDistributeReq& rhs) :
    d(rhs.d)
{
}

RoomDistributeReq::~RoomDistributeReq()
{
}

RoomDistributeReq& RoomDistributeReq::operator=(const RoomDistributeReq& rhs)
{
	d = rhs.d;
	return *this;
}

qint64 RoomDistributeReq::userId() const
{
	return d->m_userId;
}

void RoomDistributeReq::setUserId(qint64 val)
{
	d->m_userId = val;
}

qint64 RoomDistributeReq::partnerId() const
{
	return d->m_partnerId;
}

void RoomDistributeReq::setPartnerId(qint64 val)
{
	d->m_partnerId = val;
}

qint64 RoomDistributeReq::calleeId() const
{
	return d->m_calleeId;
}

void RoomDistributeReq::setCalleeId(qint64 val)
{
	d->m_calleeId = val;
}

QString RoomDistributeReq::sourceLang() const
{
	return d->m_sourceLang;
}

void RoomDistributeReq::setSourceLang(const QString& val)
{
	d->m_sourceLang = val;
}

QString RoomDistributeReq::targetLang() const
{
	return d->m_targetLang;
}

void RoomDistributeReq::setTargetLang(const QString& val)
{
	d->m_targetLang = val;
}

QString RoomDistributeReq::callMode() const
{
	return d->m_callMode;
}

void RoomDistributeReq::setCallMode(const QString& val)
{
	d->m_callMode = val;
}

QString RoomDistributeReq::callCountry() const
{
	return d->m_callCountry;
}

void RoomDistributeReq::setCallCountry(const QString& val)
{
	d->m_callCountry = val;
}

QString RoomDistributeReq::countryCode() const
{
	return d->m_countryCode;
}

void RoomDistributeReq::setCountryCode(const QString& val)
{
	d->m_countryCode = val;
}

QString RoomDistributeReq::parentCdrId() const
{
	return d->m_parentCdrId;
}

void RoomDistributeReq::setParentCdrId(const QString& val)
{
	d->m_parentCdrId = val;
}

bool RoomDistributeReq::operator==(const RoomDistributeReq& rhs) const
{
	return d->m_userId == rhs.d->m_userId
		&& d->m_partnerId == rhs.d->m_partnerId
		&& d->m_calleeId == rhs.d->m_calleeId
		&& d->m_sourceLang == rhs.d->m_sourceLang
		&& d->m_targetLang == rhs.d->m_targetLang
		&& d->m_callMode == rhs.d->m_callMode
		&& d->m_callCountry == rhs.d->m_callCountry
		&& d->m_countryCode == rhs.d->m_countryCode
		&& d->m_parentCdrId == rhs.d->m_parentCdrId;
}

bool RoomDistributeReq::operator!=(const RoomDistributeReq& rhs) const
{
	return !operator==(rhs);
}


//Room Distribute Response
class RoomDistributeRspPrivate : public QSharedData
{
public:
	RoomDistributeRspPrivate();
	RoomDistributeRspPrivate(const RoomDistributeRspPrivate& rhs);

	RspMsg m_rspMsg;
	qint64 m_userId;
	QString m_roomAddress;
	QString m_cdrId;
	QString m_recordPath;
	QString m_parentCdrId;
};

RoomDistributeRspPrivate::RoomDistributeRspPrivate() : 
    QSharedData(),
	m_rspMsg(),
    m_userId(0),
    m_roomAddress(),
    m_cdrId(),
    m_recordPath(),
    m_parentCdrId()
{

}

RoomDistributeRspPrivate::RoomDistributeRspPrivate(const RoomDistributeRspPrivate& rhs) :
    QSharedData(rhs),
	m_rspMsg(rhs.m_rspMsg),
	m_userId(rhs.m_userId),
	m_roomAddress(rhs.m_roomAddress),
	m_cdrId(rhs.m_cdrId),
	m_recordPath(rhs.m_recordPath),
	m_parentCdrId(rhs.m_parentCdrId)
{

}

RoomDistributeRsp::RoomDistributeRsp() :
    d(new RoomDistributeRspPrivate)
{
}

RoomDistributeRsp::RoomDistributeRsp(const RoomDistributeRsp& rhs) :
    d(rhs.d)
{
}

RoomDistributeRsp::~RoomDistributeRsp()
{
}

RoomDistributeRsp& RoomDistributeRsp::operator=(const RoomDistributeRsp& rhs)
{
	d = rhs.d;
	return *this;
}

RspMsg RoomDistributeRsp::rspMsg() const
{
	return d->m_rspMsg;
}

void RoomDistributeRsp::setRspMsg(const RspMsg& val)
{
	d->m_rspMsg = val;
}

qint64 RoomDistributeRsp::userId() const
{
	return d->m_userId;
}

void RoomDistributeRsp::setUserId(qint64 val)
{
	d->m_userId = val;
}

QString RoomDistributeRsp::roomAddress() const
{
	return d->m_roomAddress;
}

void RoomDistributeRsp::setRoomAddress(const QString& val)
{
	d->m_roomAddress = val;
}

QString RoomDistributeRsp::cdrId() const
{
	return d->m_cdrId;
}

void RoomDistributeRsp::setCdrId(const QString& val)
{
	d->m_cdrId = val;
}

QString RoomDistributeRsp::recordPath() const
{
	return d->m_recordPath;
}

void RoomDistributeRsp::setRecordPath(const QString& val)
{
	d->m_recordPath = val;
}

QString RoomDistributeRsp::parentCdrId() const
{
	return d->m_parentCdrId;
}

void RoomDistributeRsp::setParentCdrId(const QString& val)
{
	d->m_parentCdrId = val;
}

bool RoomDistributeRsp::operator==(const RoomDistributeRsp& rhs) const
{
	return d->m_rspMsg == rhs.d->m_rspMsg
		&& d->m_userId == rhs.d->m_userId
		&& d->m_roomAddress == rhs.d->m_roomAddress
		&& d->m_cdrId == rhs.d->m_cdrId
		&& d->m_recordPath == rhs.d->m_recordPath
		&& d->m_parentCdrId == rhs.d->m_parentCdrId;
}

bool RoomDistributeRsp::operator!=(const RoomDistributeRsp& rhs) const
{
	return !operator==(rhs);
}


} // namespace ns
