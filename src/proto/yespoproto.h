#ifndef YESPOPROTO_H
#define YESPOPROTO_H

#include <QtCore/QObject>
#include <QtNetwork/QAbstractSocket>
#include "request.h"
#include "protoglobal.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

namespace ns {

class YespoProtoPrivate;

class BUILDSYS_PROTO_DECL YespoProto : public QObject
{
	Q_OBJECT

public:
    explicit YespoProto(QObject* parent = 0);
    virtual ~YespoProto();

	void connect(const QString& hostName, int port);
	void disconnect();

	void login(const LoginReq& req);
	void logout(const LogoutReq& req);
	void callCancel(const CallCancelReq& req);
	void callCancel();
	void startCall(const RoomDistributeReq& req);

Q_SIGNALS:
	void socketError(const QString& errorMsg);
	void connected();
	void disconnected();
	void loginFinished(int rspCode); //0:Succeed, !0:Failed
	void logoutFinished();
	void heartbeat();
	void startCallFinished(const RoomDistributeRsp& rsp);
	void calleeNotify(const CalleeNotify& notify);
	void unregNotify();
	void repetitionLoginNotify();
	void photoNotify(const PhotoNotify& notify);
	void photoError();
	void receiveVideoStatus(int status);
	void receiveCallStatus(int status);

protected:
    YespoProto(YespoProtoPrivate& dd, QObject* parent = 0);

    YespoProtoPrivate* const d_ptr;

private:
	Q_PRIVATE_SLOT(d_func(), void _q_connectionClosed())
	Q_PRIVATE_SLOT(d_func(), void _q_connectionError(QAbstractSocket::SocketError))
	Q_PRIVATE_SLOT(d_func(), void _q_connectionStateChanged(QAbstractSocket::SocketState))
	Q_PRIVATE_SLOT(d_func(), void _q_connectionWritten(qint64))
	Q_PRIVATE_SLOT(d_func(), void _q_connectionReadyRead())

    Q_DECLARE_PRIVATE(YespoProto)
    Q_DISABLE_COPY(YespoProto)
};

} // namespace ns

#endif // YESPOPROTO_H
