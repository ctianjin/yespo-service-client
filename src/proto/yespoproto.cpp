#include <QtCore/QDebug>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QDateTime>
#include <qjson/parser.h>
#include <qjson/serializer.h>
#include <WinSock.h>

#include "yespoproto.h"
#include "yespoproto_p.h"
#include "yespo_protocol.pb.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "YespoProto:"
#endif // DEBUG_PREFIX

namespace ns {

YespoProtoPrivate::YespoProtoPrivate() :
    q_ptr(0)
{
	m_conn = new QTcpSocket();
	m_msgId = 0x0000000F;

	m_recvLength = 0;
	m_sendLength = 0;
	memset(m_recvBuffer, 0, YP_MAX_BUFFER);
	memset(m_sendBuffer, 0, YP_MAX_BUFFER);
}

YespoProtoPrivate::~YespoProtoPrivate()
{
	if(m_conn) {
		m_conn->close();
		delete m_conn;
	}
}

void YespoProtoPrivate::connect(const QString& hostName, int port)
{
	if (m_conn->state() != QAbstractSocket::UnconnectedState)
		m_conn->disconnectFromHost();
	
	m_conn->connectToHost(hostName, port);
}

void YespoProtoPrivate::disconnect()
{
	m_conn->disconnectFromHost();
}

void YespoProtoPrivate::login(const LoginReq& req)
{
	Package pkgReq;
	char protobuf[YP_MAX_BUFFER] = {0};

	makeRequestOutHeader(yespo::MSG_TYPE_LOGINSVR_LOGIN_REQ, req.userId(), pkgReq);

	yespo::LoginSvrLoginReq pbReq;
	pbReq.set_user_id(req.userId());
	pbReq.set_user_type(req.userType());
	pbReq.set_token(req.token().toLocal8Bit().constData());
	pbReq.SerializeToArray(protobuf, sizeof(protobuf)-1);
	pkgReq.SetPayLoad((const BYTE*)protobuf, qstrlen(protobuf));

	sendRequestAsync(pkgReq);
}

void YespoProtoPrivate::logout(const LogoutReq& req)
{
	Package pkgReq;
	char protobuf[YP_MAX_BUFFER] = {0};

	makeRequestOutHeader(yespo::MSG_TYPE_LOGINSVR_LOGOUT_REQ, req.userId(), pkgReq);

	yespo::LoginSvrLogoutReq pbReq;
	pbReq.set_user_id(req.userId());
	pbReq.SerializeToArray(protobuf, sizeof(protobuf)-1);
	pkgReq.SetPayLoad((const BYTE*)protobuf, qstrlen(protobuf));

	sendRequestAsync(pkgReq);
}

void YespoProtoPrivate::callCancel(const CallCancelReq& req)
{
	Package pkgReq;
	char protobuf[YP_MAX_BUFFER] = {0};

	makeRequestOutHeader(yespo::MSG_TYPE_ROOMSVR_CALL_CANCEL_REQ, req.partnerId(), pkgReq);

	yespo::RoomsvrCallCancelReq pbReq;
	pbReq.set_user_id(req.userId());
	pbReq.set_partner_id(req.partnerId());
	pbReq.SerializeToArray(protobuf, sizeof(protobuf) - 1);
	pkgReq.SetPayLoad((const BYTE*)protobuf, qstrlen(protobuf));

	sendRequestAsync(pkgReq);
}

void YespoProtoPrivate::callCancel()
{
	Package pkgReq;
	char protobuf[YP_MAX_BUFFER] = {0};

	makeRequestOutHeader(yespo::MSG_TYPE_ROOMSVR_CALL_CANCEL_REQ, m_partnerId, pkgReq);

	yespo::RoomsvrCallCancelReq pbReq;
	pbReq.set_user_id(m_callerId);
	pbReq.set_partner_id(m_partnerId);
	pbReq.SerializeToArray(protobuf, sizeof(protobuf) - 1);
	pkgReq.SetPayLoad((const BYTE*)protobuf, qstrlen(protobuf));

	sendRequestAsync(pkgReq);
}

void YespoProtoPrivate::startCall(const RoomDistributeReq& req)
{
	Package pkgReq;
	char protobuf[YP_MAX_BUFFER] = {0};

	makeRequestOutHeader(yespo::MSG_TYPE_ROOMSVR_DISTRIBUTE_REQ, req.userId(), pkgReq);

	yespo::RoomsvrDistributeReq pbReq;
	pbReq.set_user_id(req.userId());
	pbReq.set_partner_id(req.partnerId());
	pbReq.set_callee_id(req.calleeId());
	pbReq.set_source_language(req.sourceLang().toLocal8Bit().constData());
	pbReq.set_target_language(req.targetLang().toLocal8Bit().constData());
	pbReq.set_call_mode(req.callMode().toLocal8Bit().constData());
	pbReq.set_call_country(req.callCountry().toLocal8Bit().constData());
	pbReq.set_country_code(req.countryCode().toLocal8Bit().constData());
	pbReq.set_parent_cdr_id(req.parentCdrId().toLocal8Bit().constData());
	pbReq.SerializeToArray(protobuf, sizeof(protobuf)-1);
	pkgReq.SetPayLoad((const BYTE*)protobuf, qstrlen(protobuf));

	sendRequestAsync(pkgReq);
}

void YespoProtoPrivate::init()
{
	Q_Q(YespoProto);
	Q_ASSERT(q);

	QObject::connect(m_conn, SIGNAL(disconnected()), q, SLOT(_q_connectionClosed()));
	QObject::connect(m_conn, SIGNAL(error(QAbstractSocket::SocketError)), q, SLOT(_q_connectionError(QAbstractSocket::SocketError)));
	QObject::connect(m_conn, SIGNAL(stateChanged(QAbstractSocket::SocketState)), q, SLOT(_q_connectionStateChanged(QAbstractSocket::SocketState)));
	QObject::connect(m_conn, SIGNAL(bytesWritten(qint64)), q, SLOT(_q_connectionWritten(qint64)));
	QObject::connect(m_conn, SIGNAL(readyRead()), q, SLOT(_q_connectionReadyRead()));
}

void YespoProtoPrivate::makeRequestOutHeader(int msgType, qint64 userId, Package& pkgReq)
{
	char protobuf[YP_MAX_BUFFER] = {0};
	yespo::UserAgent pbUserAgent;
	yespo::Header pbOutHeader;
	pbOutHeader.set_msg_id(m_msgId++);
	pbOutHeader.set_msg_type(msgType);
	pbOutHeader.set_user_id(userId);
	pbOutHeader.set_allocated_user_agent(&pbUserAgent);
	pbOutHeader.set_timestamp(QDateTime::currentDateTime().toTime_t());
	pbOutHeader.SerializeToArray(protobuf, sizeof(protobuf)-1);
	pbOutHeader.release_user_agent();
	pkgReq.SetOuterHeader((const BYTE*)protobuf, qstrlen(protobuf));
}

void YespoProtoPrivate::sendRequestAsync(Package& pkgReq)
{
	int reqLen = 0;
	char reqData[YP_MAX_BUFFER] = {0};
	reqLen = pkgReq.BuildPack(reqData);
	int hasSent = m_conn->write(reqData, reqLen);
	if (hasSent != reqLen) {
		qWarning() << DEBUG_PREFIX << "The data has not fully sent";
	}
}

int YespoProtoPrivate::checkComplete(BYTE* buf, int size)
{
	if(m_recvLength == 0)
		return -1;

	int needsize, data_length;
	memcpy((void*)&data_length, (void*)(m_recvBuffer+4), sizeof(int));
	needsize = ntohl(data_length);

	if(needsize > size || needsize < YP_PACK_HEADER_LEN) {
		qDebug() << DEBUG_PREFIX << "Insufficient buffer size";
		memset(m_recvBuffer, 0, sizeof(m_recvBuffer));
		m_recvLength = 0;
		return -1;
	}

	memcpy(buf, m_recvBuffer, needsize);

	m_recvLength = m_recvLength-needsize;
	if(m_recvLength > 0)
		memmove(m_recvBuffer, m_recvBuffer+needsize, m_recvLength);
	else if(m_recvLength < 0) {
		qDebug() << DEBUG_PREFIX << "Receive data length error";
	} else {
		memset(m_recvBuffer, 0, sizeof(m_recvBuffer));
	}

	return needsize;
}

void YespoProtoPrivate::parsePackage(const BYTE* data, int dataLength)
{
	Q_Q(YespoProto);

	Package pkg;

	if (pkg.ParsePack(data, dataLength) == false) {
		qDebug() << DEBUG_PREFIX << "Parse package failed";
		return;
	}
	int messageType = pkg.OuterHeader().msg_type();
	switch(messageType) 
	{
	case yespo::MSG_TYPE_APP_HEARTBEAT_REQ:
		{
			yespo::AppHeartbeatReq pbReq;
			pbReq.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());
			qDebug() <<DEBUG_PREFIX << "Heartbeat request, timestamp : " << pbReq.timestamp();

			Q_EMIT q->heartbeat();

			char protobuf[YP_MAX_BUFFER] = {0};
			Package pkgRsp;
			yespo::RspMsg pbRspMsg;
			pbRspMsg.set_rsp_code(0);
			pbRspMsg.set_rsp_msg("OK");

			yespo::Header pbHeader;
			pbHeader.set_msg_id(pkg.OuterHeader().msg_id());
			pbHeader.set_msg_type(yespo::MSG_TYPE_APP_HEARTBEAT_RSP);
			pbHeader.set_user_id(pkg.OuterHeader().user_id());
			pbHeader.set_timestamp(pkg.OuterHeader().timestamp());
			pbHeader.SerializeToArray(protobuf, sizeof(protobuf));
			pkgRsp.SetOuterHeader((const BYTE*)protobuf, qstrlen(protobuf));

			memset(protobuf, 0, YP_MAX_BUFFER);

			yespo::AppHeartbeatRsp pbRsp;
			pbRsp.set_timestamp(pbReq.timestamp());
			pbRsp.set_allocated_rsp_msg(&pbRspMsg);
			pbRsp.SerializeToArray(protobuf, sizeof(protobuf));
			pbRsp.release_rsp_msg();
			pkgRsp.SetPayLoad((const BYTE*)protobuf, qstrlen(protobuf));

			sendRequestAsync(pkgRsp);
		}
		break;
	case yespo::MSG_TYPE_LOGINSVR_LOGIN_RSP:
		{
			yespo::LoginSvrLoginRsp pbRsp;
			pbRsp.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());
			qDebug() <<DEBUG_PREFIX << "Login response : " << pbRsp.rsp_msg().rsp_code();

			Q_EMIT q->loginFinished(pbRsp.rsp_msg().rsp_code());
			if (pbRsp.rsp_msg().rsp_code() == 0) {
				m_partnerId  = pkg.OuterHeader().user_id();
			}
		}
		break;
	case yespo::MSG_TYPE_LOGINSVR_LOGOUT_RSP:
		{
			yespo::LoginSvrLogoutRsp pbRsp;
			pbRsp.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());
			qDebug() <<DEBUG_PREFIX << "Logout response : " << pbRsp.rsp_msg().rsp_code();

			Q_EMIT q->logoutFinished();
		}
		break;
	case yespo::MSG_TYPE_ROOMSVR_CALL_CANCEL_RSP:
		{
			yespo::RoomsvrCallCancelRsp pbRsp;
			pbRsp.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());
			qDebug() <<DEBUG_PREFIX << "Call cancel response : " << pbRsp.rsp_msg().rsp_code();
		}
		break;
	case yespo::MSG_TYPE_APP_CALLEE_NOTIFY_REQ:
		{
			yespo::AppCalleeNotifyReq pbReq;
			pbReq.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());

			CalleeNotify notify;
			notify.setUserId(pbReq.user_id());
			notify.setCalleeId(pbReq.callee_id());
			notify.setCallMode(QString::fromStdString(pbReq.call_mode()));
			notify.setRoomAddress(QString::fromStdString(pbReq.room_address()));
			notify.setSourceLang(QString::fromStdString(pbReq.source_language()));
			notify.setTargetLang(QString::fromStdString(pbReq.target_language()));
			notify.setDeviceToken(QString::fromStdString(pbReq.device_token()));
			notify.setCdrId(QString::fromStdString(pbReq.cdr_id()));

			m_callerId = pbReq.user_id();
			Q_EMIT q->calleeNotify(notify);
		}
		break;
	case yespo::MSG_TYPE_APP_CALL_TERMINATE_REQ:
		{
			yespo::AppCallTerminateReq pbReq;
			pbReq.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());

			qDebug() <<DEBUG_PREFIX << "Request call terminate";

			if (pbReq.user_id() > 0) {
				Q_EMIT q->unregNotify();
				m_callerId = 0;
			}
		}
		break;
	case yespo::MSG_TYPE_APP_VIDEO_SEND_REQ:
		{
			yespo::AppVideoSendReq pbReq;
			pbReq.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());
			qDebug() << DEBUG_PREFIX << "Video request info: " << pbReq.video_status() << pbReq.user_id() << pbReq.partner_id();

			Q_EMIT q->receiveVideoStatus(pbReq.video_status());

			//answer video request
			if (pbReq.video_status() == 1) {
				char protobuf[YP_MAX_BUFFER] = {0};

				Package pkgReq;
				makeRequestOutHeader(yespo::MSG_TYPE_ROOMSVR_VIDEO_RECV_REQ, m_callerId, pkgReq);

				yespo::RoomsvrVideoRecvReq pbVideoReq;
				pbVideoReq.set_partner_id(m_partnerId);
				pbVideoReq.set_user_id(m_callerId);
				pbVideoReq.set_result(pbReq.video_status());
				pbVideoReq.SerializeToArray(protobuf, sizeof(protobuf) - 1);
				pkgReq.SetPayLoad((const BYTE*)protobuf, qstrlen(protobuf));

				sendRequestAsync(pkgReq);
			}
		}
		break;
	case yespo::MSG_TYPE_APP_CALL_STATUS_REQ:
		{
			yespo::AppCallStatusReq pbReq;
			pbReq.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());
			qDebug() << DEBUG_PREFIX << "call status: " << pbReq.call_status();

			Q_EMIT q->receiveCallStatus(pbReq.call_status());
		}
		break;
	case yespo::MSG_TYPE_APP_PHOTO_MSG_REQ:
		{
			yespo::AppPhotoMsgReq pbReq;
			pbReq.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());
	
			QByteArray base = QByteArray::fromBase64(QString::fromStdString(pbReq.msg()).toAscii());
			qDebug() << DEBUG_PREFIX << base;
			QJson::Parser parser;
			const QVariant record = parser.parse(base);
			if (record.canConvert<QVariantMap>()) {
				const QVariantMap dataMap = record.toMap();
				PhotoNotify notify;
				notify.setPhotoId(dataMap.value(QLatin1String("photo_id")).toString());
				notify.setSmallPhoto(dataMap.value(QLatin1String("small_photo")).toString());
				notify.setLargePhoto(dataMap.value(QLatin1String("large_photo")).toString());
				Q_EMIT q->photoNotify(notify);
				return;
			}	
			Q_EMIT q->photoError();
		} 
		break;
	case yespo::MSG_TYPE_ROOMSVR_DISTRIBUTE_RSP:
		{
			yespo::RoomsvrDistributeRsp pbRsp;
			pbRsp.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());
			qDebug() <<DEBUG_PREFIX << "Start call response : " << pbRsp.rsp_msg().rsp_code();

			RspMsg rspMsg;
			rspMsg.setRspCode(pbRsp.rsp_msg().rsp_code());
			rspMsg.setRspMsg(QString::fromLocal8Bit(pbRsp.rsp_msg().rsp_msg().c_str()));
			RoomDistributeRsp rsp;
			rsp.setRspMsg(rspMsg);
			rsp.setUserId(pbRsp.user_id());
			rsp.setRoomAddress(QString::fromLocal8Bit(pbRsp.room_address().c_str()));
			rsp.setCdrId(QString::fromLocal8Bit(pbRsp.cdr_id().c_str()));
			rsp.setRecordPath(QString::fromLocal8Bit(pbRsp.record_path().c_str()));
			rsp.setParentCdrId(QString::fromLocal8Bit(pbRsp.parent_cdr_id().c_str()));

			Q_EMIT q->startCallFinished(rsp);
		}
		break;
	case yespo::MSG_TYPE_APP_REPETITION_LOGIN_REQ:
		{
			yespo::AppRepetitionLogin pbReq;
			pbReq.ParseFromArray(pkg.GetPayLoad(), pkg.GetPayloadLength());
			qDebug() << DEBUG_PREFIX << "Repetition login: " << pbReq.user_id();

			Q_EMIT q->repetitionLoginNotify();
		}
		break;
	default:
		qDebug() <<DEBUG_PREFIX << "Unknown message type : " << messageType;
		break;
	}
}

void YespoProtoPrivate::_q_connectionClosed()
{
	Q_Q(YespoProto);

	qDebug() << DEBUG_PREFIX << "Connection has been disconnected";

	Q_EMIT q->disconnected();
}

void YespoProtoPrivate::_q_connectionError(QAbstractSocket::SocketError e)
{
	Q_Q(YespoProto);

	qDebug() << DEBUG_PREFIX << "Connection error, error number is : " << e;

	QString errorMsg;
	switch (e)
	{
	case QAbstractSocket::ConnectionRefusedError:
		errorMsg = QString::fromLocal8Bit("The connection was refused by the peer (or timed out).");
		break;
	case QAbstractSocket::RemoteHostClosedError:
		errorMsg = QString::fromLocal8Bit("The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.");
		break;
	case QAbstractSocket::HostNotFoundError:
		errorMsg = QString::fromLocal8Bit("The host address was not found.");
		break;
	case QAbstractSocket::SocketAccessError:
		errorMsg = QString::fromLocal8Bit("The socket operation failed because the application lacked the required privileges.");
		break;
	case QAbstractSocket::SocketResourceError:
		errorMsg = QString::fromLocal8Bit("The local system ran out of resources (e.g., too many sockets).");
		break;
	case QAbstractSocket::SocketTimeoutError:
		errorMsg = QString::fromLocal8Bit("The socket operation timed out.");
		break;
	case QAbstractSocket::DatagramTooLargeError:
		errorMsg = QString::fromLocal8Bit("The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).");
		break;
	case QAbstractSocket::NetworkError:
		errorMsg = QString::fromLocal8Bit("An error occurred with the network (e.g., the network cable was accidentally plugged out).");
		break;
	case QAbstractSocket::AddressInUseError:
		errorMsg = QString::fromLocal8Bit("The address specified to QUdpSocket::bind() is already in use and was set to be exclusive.");
		break;
	case QAbstractSocket::SocketAddressNotAvailableError:
		errorMsg = QString::fromLocal8Bit("The address specified to QUdpSocket::bind() does not belong to the host.");
		break;
	case QAbstractSocket::UnsupportedSocketOperationError:
		errorMsg = QString::fromLocal8Bit("The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).");
		break;
	case QAbstractSocket::ProxyAuthenticationRequiredError:
		errorMsg = QString::fromLocal8Bit("The socket is using a proxy, and the proxy requires authentication.");
		break;
	case QAbstractSocket::SslHandshakeFailedError	:
		errorMsg = QString::fromLocal8Bit("The SSL/TLS handshake failed, so the connection was closed (only used in QSslSocket) (This value was introduced in 4.4.)");
		break;
	case QAbstractSocket::UnfinishedSocketOperationError:
		errorMsg = QString::fromLocal8Bit("Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background). (This value was introduced in 4.4.)");
		break;
	case QAbstractSocket::ProxyConnectionRefusedError:
		errorMsg = QString::fromLocal8Bit("Could not contact the proxy server because the connection to that server was denied (This value was introduced in 4.5.)");
		break;
	case QAbstractSocket::ProxyConnectionClosedError:
		errorMsg = QString::fromLocal8Bit("The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established) (This value was introduced in 4.5.)");
		break;
	case QAbstractSocket::ProxyConnectionTimeoutError:
		errorMsg = QString::fromLocal8Bit("The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase. (This value was introduced in 4.5.)");
		break;
	case QAbstractSocket::ProxyNotFoundError:
		errorMsg = QString::fromLocal8Bit("The proxy address set with setProxy() (or the application proxy) was not found. (This value was introduced in 4.5.)");
		break;
	case QAbstractSocket::ProxyProtocolError:
		errorMsg = QString::fromLocal8Bit("The connection negotiation with the proxy server because the response from the proxy server could not be understood. (This value was introduced in 4.5.)");
		break;
	case QAbstractSocket::UnknownSocketError:
		errorMsg = QString::fromLocal8Bit("An unidentified error occurred.");
		break;
	}

	Q_EMIT q->socketError(errorMsg);
}

void YespoProtoPrivate::_q_connectionStateChanged(QAbstractSocket::SocketState state)
{
	Q_Q(YespoProto);

	qDebug() << DEBUG_PREFIX << "Connection state changed to " << state;
	switch(state) {
		case QAbstractSocket::UnconnectedState:
			//The socket is not connected.
			break;
		case QAbstractSocket::HostLookupState:
			//The socket is performing a host name lookup.
			break;
		case QAbstractSocket::ConnectingState:
			//The socket has started establishing a connection.
			break;
		case QAbstractSocket::ConnectedState: 
			Q_EMIT q->connected();
			break;
		case QAbstractSocket::BoundState:
			//The socket is bound to an address and port (for servers).
			break;
		case QAbstractSocket::ListeningState:
			//For internal use only.
			break;
		case QAbstractSocket::ClosingState:
			//The socket is about to close (data may still be waiting to be written).
			break;
		default:
			break;
	}
}

void YespoProtoPrivate::_q_connectionWritten(qint64 bytes)
{
	qDebug() << DEBUG_PREFIX << "Connection has data to write" << bytes;
}

void YespoProtoPrivate::_q_connectionReadyRead()
{
	qDebug() << DEBUG_PREFIX << "Connection ready to read";
	int bytes = m_conn->read((char*)m_recvBuffer+m_recvLength, YP_MAX_BUFFER-m_recvLength);
	if(bytes < 0) 
		return;
	m_recvLength += bytes;
	int complete = -1;
	do 
	{
		BYTE completeData[YP_MAX_BUFFER] = {0};
		complete = checkComplete(completeData, YP_MAX_BUFFER);
		if (complete == -1)
			break;
		parsePackage(completeData, complete);
	} while (complete != -1);
}


//////////////////////////////////////////////////////////////////////////
// YesPo Protocol
//////////////////////////////////////////////////////////////////////////

YespoProto::YespoProto(QObject* parent) :
    QObject(parent),
    d_ptr(new YespoProtoPrivate)
{
    d_ptr->q_ptr = this;

	d_ptr->init();
}

YespoProto::YespoProto(YespoProtoPrivate& dd, QObject* parent) :
    QObject(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;

	d_ptr->init();
}

YespoProto::~YespoProto()
{
    delete d_ptr;
}

void YespoProto::connect(const QString& hostName, int port)
{
	Q_D(YespoProto);

	d->connect(hostName, port);
}

void YespoProto::disconnect()
{
	Q_D(YespoProto);

	d->disconnect();
}

void YespoProto::login(const LoginReq& req)
{
	Q_D(YespoProto);

	d->login(req);
}

void YespoProto::logout(const LogoutReq& req)
{
	Q_D(YespoProto);

	d->logout(req);
}

void YespoProto::callCancel(const CallCancelReq& req)
{
	Q_D(YespoProto);

	d->callCancel(req);
}

void YespoProto::callCancel()
{
	Q_D(YespoProto);

	d->callCancel();
}

void YespoProto::startCall(const RoomDistributeReq& req)
{
	Q_D(YespoProto);

	d->startCall(req);
}

} // namespace ns

#include "moc_yespoproto.cpp"
