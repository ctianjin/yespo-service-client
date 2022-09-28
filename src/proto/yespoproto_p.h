#ifndef YESPOPROTO_P_H
#define YESPOPROTO_P_H

#include <QtCore/QPointer>
#include "yespoproto.h"
#include "protoglobal.h"
#include "package.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace ns {

class BUILDSYS_PROTO_DECL YespoProtoPrivate
{
    Q_DECLARE_PUBLIC(YespoProto)

public:
    YespoProto* q_ptr;

    YespoProtoPrivate();
    virtual ~YespoProtoPrivate();

	void connect(const QString& hostName, int port);
	void disconnect();

	void login(const LoginReq& req);
	void logout(const LogoutReq& req);
	void callCancel(const CallCancelReq& req);
	void callCancel();
	void startCall(const RoomDistributeReq& req);

	void _q_connectionClosed();
	void _q_connectionError(QAbstractSocket::SocketError e);
	void _q_connectionStateChanged(QAbstractSocket::SocketState state);
	void _q_connectionWritten(qint64);
	void _q_connectionReadyRead();

private:
	void init();

	void makeRequestOutHeader(int msgType, qint64 userId, Package& pkgReq);
	void sendRequestAsync(Package& pkgReq);
	int checkComplete(BYTE* buf, int size);    //return the bytes copied to buffer, or -1 means not complete.
	                                                                 //this function need calling until return -1
	void parsePackage(const BYTE* data, int dataLength);

	QPointer<QTcpSocket> m_conn;
	qint64 m_msgId;
	qint64 m_partnerId;
	qint64 m_callerId;;

	int m_recvLength;    //The data length in the receive buffer
	int m_sendLength;    //The data length in the send buffer
	BYTE m_recvBuffer[YP_MAX_BUFFER];
	BYTE m_sendBuffer[YP_MAX_BUFFER];
};

} // namespace ns

#endif // YESPOPROTO_P_H
