#ifndef REQUEST_H
#define REQUEST_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include "protoglobal.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace ns {

//Response Message
class RspMsgPrivate;
class BUILDSYS_PROTO_DECL RspMsg
{
public:
	RspMsg();
	RspMsg(const RspMsg& rhs);
	~RspMsg();

	RspMsg& operator=(const RspMsg& rhs);

	int rspCode() const;
	void setRspCode(int val);

	QString rspMsg() const;
	void setRspMsg(const QString& val);

	bool operator==(const RspMsg& rhs) const;
	bool operator!=(const RspMsg& rhs) const;

private:
	QSharedDataPointer<RspMsgPrivate> d;
};

//Login Request
class LoginReqPrivate;
class BUILDSYS_PROTO_DECL LoginReq
{
public:
	LoginReq();
	LoginReq(const LoginReq& rhs);
	~LoginReq();

	LoginReq& operator=(const LoginReq& rhs);

	qint64 userId() const;
	void setUserId(qint64 val);

	int userType() const;
	void setUserType(int val);

	QString token() const;
	void setToken(const QString& val);

	QString deviceToken() const;
	void setDeviceToken(const QString& val);

	bool operator==(const LoginReq& rhs) const;
	bool operator!=(const LoginReq& rhs) const;

private:
	QSharedDataPointer<LoginReqPrivate> d;
};


//Logout Request
class LogoutReqPrivate;
class BUILDSYS_PROTO_DECL LogoutReq
{
public:
	LogoutReq();
	LogoutReq(const LogoutReq& rhs);
	~LogoutReq();

	LogoutReq& operator=(const LogoutReq& rhs);

	qint64 userId() const;
	void setUserId(qint64 val);

	bool operator==(const LogoutReq& rhs) const;
	bool operator!=(const LogoutReq& rhs) const;

private:
	QSharedDataPointer<LogoutReqPrivate> d;
};

//Call Cancel Request
class CallCancelReqPrivate;
class BUILDSYS_PROTO_DECL CallCancelReq
{
public:
	CallCancelReq();
	CallCancelReq(const CallCancelReq& rhs);
	~CallCancelReq();

	CallCancelReq& operator=(const CallCancelReq& rhs);

	qint64 userId() const;
	void setUesrId(qint64 val);

	qint64 partnerId() const;
	void setPartnerId(qint64 val);

	bool operator==(const CallCancelReq& rhs) const;
	bool operator!=(const CallCancelReq& rhs) const;

private:
	QSharedDataPointer<CallCancelReqPrivate> d;
};

//Photo Notify
class PhotoNotifyPrivate;
class BUILDSYS_PROTO_DECL PhotoNotify
{
public:
	PhotoNotify();
	PhotoNotify(const PhotoNotify& rhs);
	~PhotoNotify();

	PhotoNotify& operator=(const PhotoNotify& rhs);

	QString receiver() const;
	void setReceiver(const QString& receiver);

	QString sender() const;
	void setSender(const QString& sender);

	QString sendTime() const;
	void setSendTime(const QString& sendTime);

	QString photoId() const;
	void setPhotoId(const QString& id);

	QString expire() const;
	void setExpire(const QString& expire);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& smallPhoto);

	QString largePhoto() const;
	void setLargePhoto(const QString& largePhoto);

private:
	QSharedDataPointer<PhotoNotifyPrivate> d;
};

//Callee Notify
class CalleeNotifyPrivate;
class BUILDSYS_PROTO_DECL CalleeNotify
{
public:
	CalleeNotify();
	CalleeNotify(const CalleeNotify& rhs);
	~CalleeNotify();

	CalleeNotify& operator=(const CalleeNotify& rhs);

	qint64 userId() const;
	void setUserId(qint64 val);

	qint64 calleeId() const;
	void setCalleeId(qint64 val);

	QString roomAddress() const;
	void setRoomAddress(const QString& val);

	QString deviceToken() const;
	void setDeviceToken(const QString& val);

	QString sourceLang() const;
	void setSourceLang(const QString& val);

	QString targetLang() const;
	void setTargetLang(const QString& val);

	QString callMode() const;
	void setCallMode(const QString& val);

	QString cdrId() const;
	void setCdrId(const QString& val);

	bool operator==(const CalleeNotify& rhs) const;
	bool operator!=(const CalleeNotify& rhs) const;

private:
	QSharedDataPointer<CalleeNotifyPrivate> d;
};

//Room Distribute Request
class RoomDistributeReqPrivate;
class BUILDSYS_PROTO_DECL RoomDistributeReq
{
public:
	RoomDistributeReq();
	RoomDistributeReq(const RoomDistributeReq& rhs);
	~RoomDistributeReq();

	RoomDistributeReq& operator=(const RoomDistributeReq& rhs);

	qint64 userId() const;
	void setUserId(qint64 val);

	qint64 partnerId() const;
	void setPartnerId(qint64 val);

	qint64 calleeId() const;
	void setCalleeId(qint64 val);

	QString sourceLang() const;
	void setSourceLang(const QString& val);

	QString targetLang() const;
	void setTargetLang(const QString& val);

	QString callMode() const;
	void setCallMode(const QString& val);

	QString callCountry() const;
	void setCallCountry(const QString& val);

	QString countryCode() const;
	void setCountryCode(const QString& val);

	QString parentCdrId() const;
	void setParentCdrId(const QString& val);

	bool operator==(const RoomDistributeReq& rhs) const;
	bool operator!=(const RoomDistributeReq& rhs) const;

private:
	QSharedDataPointer<RoomDistributeReqPrivate> d;
};


//Room Distribute Response
class RoomDistributeRspPrivate;
class BUILDSYS_PROTO_DECL RoomDistributeRsp
{
public:
	RoomDistributeRsp();
	RoomDistributeRsp(const RoomDistributeRsp& rhs);
	~RoomDistributeRsp();

	RoomDistributeRsp& operator=(const RoomDistributeRsp& rhs);

	RspMsg rspMsg() const;
	void setRspMsg(const RspMsg& val);

	qint64 userId() const;
	void setUserId(qint64 val);

	QString roomAddress() const;
	void setRoomAddress(const QString& val);

	QString cdrId() const;
	void setCdrId(const QString& val);

	QString recordPath() const;
	void setRecordPath(const QString& val);

	QString parentCdrId() const;
	void setParentCdrId(const QString& val);

	bool operator==(const RoomDistributeRsp& rhs) const;
	bool operator!=(const RoomDistributeRsp& rhs) const;

private:
	QSharedDataPointer<RoomDistributeRspPrivate> d;
};



} // namespace ns

Q_DECLARE_METATYPE(ns::LoginReq)
Q_DECLARE_METATYPE(ns::LogoutReq)
Q_DECLARE_METATYPE(ns::CallCancelReq)
Q_DECLARE_METATYPE(ns::CalleeNotify)
Q_DECLARE_METATYPE(ns::PhotoNotify)
Q_DECLARE_METATYPE(ns::RoomDistributeReq)
Q_DECLARE_METATYPE(ns::RoomDistributeRsp)

#endif // REQUEST_H
