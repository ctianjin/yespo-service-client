#include "im.h"
#include "im_p.h"
#include "imudppackage.h"
#include "imworkerthread.h"
#include "imretransqueue.h"
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "improtocol.h"
#include <QDebug>
#include <QThread>
#include <QVariantMap>
#include <QtEndian>
#include <QTimer>
#include <QFile>
#include <QMutex>

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "Im:"
#endif // DEBUG_PREFIX

namespace ns {

ImPrivate::ImPrivate()
	: q_ptr(0)
{

}

ImPrivate::~ImPrivate()
{

}


void ImPrivate::init()
{
	Q_Q(Im);	

	m_isLogin = false;

	m_udpPack = new ImUdpPackage();

	m_timer = new QTimer(q);
	m_timer->setInterval(60000);
	if (m_timer) {
		QObject::connect(m_timer, SIGNAL(timeout()), q, SLOT(_q_slotTimeout()));
	}

	m_workerThread = new ImWorkerThread(q);
	QObject::connect(m_workerThread, SIGNAL(pluseFailed()), q, SLOT(_q_pluseFailed()));
// 	m_retransQueue = new ImReTransQueue(q);
// 	QObject::connect(m_retransQueue, SIGNAL(retransFailed(qint16)), q, SLOT(_q_retransFailed(qint16)));

	m_socket = new QUdpSocket(q);

	m_socket->bind(0);

	if (m_socket) {
		QObject::connect(m_socket, SIGNAL(readyRead()), q, SLOT(_q_processReadData()));
		if (m_workerThread) {
			m_workerThread->setWorkerSocket(m_socket);
		}
	}
}

bool ImPrivate::sendLoginPack()
{
	Q_Q(Im);

	if (m_currentStatus == Login || m_currentStatus == Plus)
		return true;

	if (!m_udpPack) {
		m_udpPack = new ImUdpPackage();
	}

	if (m_hostAddress.isEmpty() || m_port == 0) {
		return false;
	}

	if (!m_socket) {
		return false;
	}
#if 1
	ImUdpPackage *loginPack = new ImUdpPackage;
	qint32 serverSeq = m_udpPack->serverSeq();
	if (serverSeq < 0)
		serverSeq = 1;
	qint8 blockNum = m_udpPack->blockNum();
	if (blockNum < 0)
		blockNum = 1;
	qint8 blockSeq = m_udpPack->blockSeq();
	if (blockSeq < 0)
		blockSeq = 1;
	qint32 sessionId = m_udpPack->sessionId();
	if (sessionId < 0)
		sessionId = 15151;
	qint8 ack = 1;
	qint8 proVersion = m_udpPack->proVersion();
	if (proVersion < 0)
		proVersion = 1;
	qint16 clientSeq = m_udpPack->clientSeq();
	if (clientSeq < 0)
		clientSeq = 1;
	++clientSeq;
	qint16 checkSum = m_udpPack->checkSum();
	if (checkSum < 0)
		checkSum = 10;
	qint16 dataLen = 0;
	qint16 packType = 1;
	qint32 reserved = m_udpPack->reserved();
	if (reserved < 0)
		reserved = 1;
	QByteArray pack;

	//loginPack->setServerSeq(qToBigEndian((qint32)serverSeq));
	loginPack->setServerSeq(serverSeq);
	loginPack->setBlockNum(blockNum);
	loginPack->setBlockSeq(blockSeq);
	//loginPack->setSessionId(qToBigEndian((qint32)sessionId));
	loginPack->setSessionId(sessionId);
	loginPack->setAck(ack);
	loginPack->setProVersion(proVersion);
	loginPack->setClientSeq(clientSeq);
	loginPack->setCheckSum(checkSum);
	loginPack->setReserved(reserved);
	loginPack->setPackType(packType);
	loginPack->setResult((qint16)1);
// 	loginPack->setClientSeq(qToBigEndian((qint16)clientSeq));
// 	loginPack->setCheckSum(qToBigEndian((qint16)checkSum));
// 	loginPack->setReserved(qToBigEndian((qint32)reserved));
// 	loginPack->setPackType(qToBigEndian((qint16)packType));
// 	loginPack->setResult(qToBigEndian((qint16)1));
#endif
#if 0
	m_udpPack->setServerSeq(qToBigEndian((qint32)1));
	m_udpPack->setBlockNum(1);
	m_udpPack->setBlockSeq(1);
	m_udpPack->setSessionId(qToBigEndian((qint32)15151));
	m_udpPack->setAck(1);
	m_udpPack->setProVersion(1);
	m_udpPack->setClientSeq(qToBigEndian((qint16)0));
	m_udpPack->setCheckSum(qToBigEndian((qint16)10));
	m_udpPack->setReserved(qToBigEndian((qint32)1));
	m_udpPack->setPackType(qToBigEndian((qint16)1));
	m_udpPack->setResult(qToBigEndian((qint16)1));
#endif
	QVariantMap loginMap;
	QByteArray datagram;
	QJson::Parser parser;
	QJson::Serializer serializer;

	loginMap.insert(QLatin1String("uid"), m_loginUid);
	m_uid = serializer.serialize(loginMap);
	loginMap.insert(QLatin1String("token"), m_loginToken);
	loginMap.insert(QLatin1String("usertype"), m_loginUserType);
	loginMap.insert(QLatin1String("networktype"), m_loginNetworkType);

	datagram = serializer.serialize(loginMap);

//	loginPack->setDataLen(qToBigEndian((qint16)datagram.length()));
	loginPack->setDataLen((qint16)datagram.length());
	loginPack->setData(datagram);

	QByteArray login;

	loginPack->packPack(login);

	m_socket->writeDatagram(login, QHostAddress(m_hostAddress), m_port);

	m_currentStatus = Login;

	delete loginPack;

	if (m_timer && !m_timer->isActive())
		m_timer->start();

	return true;
}

bool ImPrivate::reconnect()
{
	Q_Q(Im);

	if (m_currentStatus != Quit)
		if (!sendQuitPack())
			return false;

	if (m_hostAddress.isEmpty() || m_port == 0) {
		return false;
	}

	if (!m_socket) {
		return false;
	}

	qint32 serverSeq = m_udpPack->serverSeq();
	qint8 blockNum = m_udpPack->blockNum();
	qint8 blockSeq = m_udpPack->blockSeq();
	qint32 sessionId = m_udpPack->sessionId();
	qint8 ack = 1;
	m_udpPack->setAck(ack);
	qint8 proVersion = m_udpPack->proVersion();
	qint16 clientSeq = m_udpPack->clientSeq();
	++clientSeq;
	m_udpPack->setClientSeq(clientSeq);
	qint16 checkSum = m_udpPack->checkSum();
	qint32 reserved = m_udpPack->reserved();
	qint16 type = 1;
	m_udpPack->setPackType(type);
	qint16 result = m_udpPack->result();

	QByteArray reconnect;

	QVariantMap loginMap;
	QByteArray datagram;
	QJson::Parser parser;
	QJson::Serializer serializer;

	loginMap.insert(QLatin1String("uid"), m_loginUid);
	m_uid = serializer.serialize(loginMap);
	loginMap.insert(QLatin1String("token"), m_loginToken);
	loginMap.insert(QLatin1String("usertype"), m_loginUserType);
	loginMap.insert(QLatin1String("networktype"), m_loginNetworkType);

	datagram = serializer.serialize(loginMap);

	m_udpPack->setDataLen((qint16)datagram.length());
	m_udpPack->setData(datagram);

	m_udpPack->packPack(reconnect);

	m_socket->writeDatagram(reconnect, QHostAddress(m_hostAddress), m_port);

	m_currentStatus = Login;

	return true;
}

bool ImPrivate::sendQuitPack()
{
	Q_Q(Im);

	if (m_currentStatus == Quit)
		return true;

	if (!m_udpPack) {
		return false;
	}

	if (m_hostAddress.isEmpty() || m_port == 0) {
		return false;
	}

	if (!m_socket) {
		return false;
	}

	QVariantMap quitMap;
	QByteArray datagram;
	QJson::Serializer serializer;

	if (!m_udpPack)
		return false;

	if (m_loginUid.isEmpty())
		return false;

	quitMap.insert(QLatin1String("uid"), m_loginUid);

	datagram = serializer.serialize(quitMap);

	qint32 serverSeq = m_udpPack->serverSeq();
	qint8 blockNum = m_udpPack->blockNum();
	qint8 blockSeq = m_udpPack->blockSeq();
	qint32 sessionId = m_udpPack->sessionId();
	qint8 ack = 1;
	m_udpPack->setAck(ack);
	qint8 proVersion = m_udpPack->proVersion();
	qint16 clientSeq = m_udpPack->clientSeq();
	++clientSeq;
	m_udpPack->setClientSeq(clientSeq);
	qint16 checkSum = m_udpPack->checkSum();
	m_udpPack->setDataLen(datagram.length());
	qint32 reserved = m_udpPack->reserved();
	qint16 type = 4;
	m_udpPack->setPackType(type);
	qint16 result = m_udpPack->result();
	m_udpPack->setData(datagram);

	QByteArray quit;
	m_udpPack->packPack(quit);

	m_socket->writeDatagram(quit, QHostAddress(m_hostAddress), m_port);

	m_currentStatus = Quit;

	if (m_workerThread && m_workerThread->isRunning()) {
		m_workerThread->quitThread(true);
		m_workerThread->quit();
		m_workerThread->wait();
	}

	return true;
}

void ImPrivate::sendAckPack()
{
	Q_Q(Im);

	if (!m_udpPack)
		return;

	ImUdpPackage *ackPack = new ImUdpPackage;
	qint32 serverSeq = m_udpPack->serverSeq();
	qint8 blockNum = m_udpPack->blockNum();
	qint8 blockSeq = m_udpPack->blockSeq();
	qint32 sessionId = m_udpPack->sessionId();
	qint8 ack = 0;
	qint8 proVersion = m_udpPack->proVersion();
	qint16 clientSeq = m_udpPack->clientSeq();
	++clientSeq;
	qint16 checkSum = m_udpPack->checkSum();
	qint16 dataLen = 0;
	qint32 reserved = m_udpPack->reserved();
	QByteArray pack;

	ackPack->setServerSeq(serverSeq);
	ackPack->setBlockNum(blockNum);
	ackPack->setBlockSeq(blockSeq);
	ackPack->setSessionId(sessionId);
	ackPack->setAck(0);
	ackPack->setProVersion(proVersion);
	ackPack->setClientSeq(clientSeq);
	ackPack->setCheckSum(checkSum);
	ackPack->setDataLen(0);
	ackPack->setReserved(reserved);

	ackPack->packPack(pack);

	if (m_socket)
		m_socket->writeDatagram(pack, QHostAddress(m_hostAddress), m_port);

	delete ackPack;
}

int ImPrivate::parsePack(QByteArray& pack)
{
	Q_Q(Im);
	/*Q_ASSERT(q);*/

	QString errorString;
	//	QMutex mutex;
	int ret = m_udpPack->parsePack(pack);
	if (ret < 0) {
		//need to retrans
		qDebug() << "result error code" << ret;
	}
	else {
		if (m_udpPack->ack() == 0 && pack.size() == 24) {
			//ack pack
			/*removeRetransPackage(m_udpPack->clientSeq());*/
			return 0;
		}

		qint16 type = m_udpPack->packType();
		if (ret == 2) {
			sendAckPack();
			switch(type) {
				case YP_TYPE_MESSAGE:
					{
						qWarning() << m_udpPack->data();
						ImCommand::GetUploadPhotoMsgResult result;
						QByteArray array = m_udpPack->data();
						QJson::Parser parser;
						const QVariant record = parser.parse(array);
						QString receiver,sender,sendtime,photoId,smallPhoto,largePhoto;
						if (record.canConvert<QVariantMap>()) {
							const QVariantMap dataMap = record.toMap();
							receiver = dataMap.value(QLatin1String("receiver")).toString();
							sender = dataMap.value(QLatin1String("sender")).toString();
							sendtime = dataMap.value(QLatin1String("sendtime")).toString();
							QByteArray data =  QByteArray::fromBase64(dataMap.value(QLatin1String("msg")).toByteArray());
							const QVariant d = parser.parse(data);
							if (d.canConvert<QVariantMap>()) {
								const QVariantMap dMap = d.toMap();
								photoId = dMap.value(QLatin1String("photo_id")).toString();
								smallPhoto = dMap.value(QLatin1String("small_photo")).toString();
								largePhoto = dMap.value(QLatin1String("large_photo")).toString();
								result.setReceiver(receiver);
								result.setSender(sender);
								result.setSendTime(sendtime);
								result.setPhotoId(photoId);
								result.setSmallPhoto(smallPhoto);
								result.setLargePhoto(largePhoto);
								Q_EMIT q->parseMsgFinished(result);
							}
						} else {
							Q_EMIT q->parseMsgError();
						}
						break;
					}
				default:
					break;
			}
		} else if (ret == 1) {
			sendAckPack();

			qint16 result = m_udpPack->result();
			switch(type) {
				case YP_TYPE_RESIGNIN:
					{
						qDebug() << "Resignin,quit!";
						break;
					}
				case YP_TYPE_PLUSH:
					{
						break;
					}
				default:
					break;
			}
			switch(result) {
				case YP_RESULT_NORMAL:
					{					
						if (type == YP_TYPE_RETURN) {
							if (m_currentStatus == Login) {
								m_isLogin = true;
								Q_EMIT q->loginSucceed();
								if (m_timer && !m_timer->isActive())
									m_timer->start();

								if (m_workerThread) {
									m_currentStatus = Plus;
									m_workerThread->quitThread(false);
									m_workerThread->setPlusePack(m_udpPack);
									if (!m_workerThread->isRunning()) {
										m_workerThread->start();
									}	
								}
							}
							if (m_currentStatus == Plus && m_workerThread &&
								m_udpPack->clientSeq() == m_workerThread->plusePack()->clientSeq()) {
									m_workerThread->resetData();
							}

							if (m_currentStatus == Quit) {
								m_isLogin = false;
								if (m_timer)
									m_timer->stop();

								if (m_workerThread) {
									if (m_workerThread->isRunning()) {
										m_workerThread->quitThread(true);
										m_workerThread->quit();
										m_workerThread->wait();
									}
								}
							}				
						}					
						break;
					}
				case YP_RESULT_ARGUMENT_ERROR:
					{
						if (m_currentStatus == Login) {
							errorString = QLatin1String("Argument error.");
							Q_EMIT q->loginFailed(errorString);
						}
						break;
					}
				case YP_RESULT_SESSIONID_ERROR:
					{
						if (m_currentStatus == Login) {
							errorString = QLatin1String("SessionID error.");
							Q_EMIT q->loginFailed(errorString);
						}
						break;
					}
				case YP_RESULT_TOKEN_ERROR:
					{
						if (m_currentStatus == Login) {
							errorString = QLatin1String("Token error.");
							Q_EMIT q->loginFailed(errorString);
						}
						break;
					}
				case YP_RESULT_NAME_PW_ERROR:
					{
						if (m_currentStatus == Login) {
							errorString = QLatin1String("User name or password error.");
							Q_EMIT q->loginFailed(errorString);
						}
						break;
					}
				case YP_RESULT_SERVER_FULL:
					{
						if (m_currentStatus == Login) {
							errorString = QLatin1String("Server full.");
							Q_EMIT q->loginFailed(errorString);
						}
						break;
					}
				case YP_RESULT_USER_NOEXIT:
					{
						if (m_currentStatus == Login) {
							errorString = QLatin1String("User no exit.");
							Q_EMIT q->loginFailed(errorString);
						}
						break;
					}

				default:
					break;
			}
		}
	}
	return ret;
}

void ImPrivate::addRetransPackage(ImUdpPackage* udpPack)
{
	if (udpPack && m_retransQueue)
		m_retransQueue->add(*udpPack);
}

void ImPrivate::removeRetransPackage(qint16 clientSeq)
{
	if (m_retransQueue)
		m_retransQueue->remove(clientSeq);
}

#if 1
void ImPrivate::_q_processReadData()
{
	Q_Q(Im);
	/*Q_ASSERT(q);*/

	while(m_socket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(m_socket->pendingDatagramSize());
		QHostAddress sender;
		quint16 senderPort;

		m_socket->readDatagram(datagram.data(), datagram.size(),
			&sender, &senderPort);
		qDebug() << "processReadDataSize:" << datagram.size() << datagram;
		if (parsePack(datagram) < 0)
			return;

		if (m_workerThread->isDataNull()) {
			m_workerThread->setSocketData(sender, senderPort);
			/*m_workerThread->setRetransQueue(m_retransQueue);*/
			if (!m_uid.isEmpty())
				m_workerThread->setPluseData(m_uid);
		}
// 		if (m_retransQueue->isDataNull()) {
// 			m_retransQueue->setSocketData(sender, senderPort);
// 			m_retransQueue->start();
// 		}
	}
}

void ImPrivate::_q_slotTimeout()
{
	Q_Q(Im);
	Q_ASSERT(q);

	if (!m_isLogin) {
		reconnect();
	}
}

void ImPrivate::_q_pluseFailed()
{
	Q_Q(Im);
	Q_ASSERT(q);

	m_currentStatus = Quit;
	m_isLogin = false;
	if (m_workerThread) {
		m_workerThread->quitThread(true);
		m_workerThread->quit();
		m_workerThread->wait();
	}

	Q_EMIT q->pluseFailed();
}

void ImPrivate::_q_retransFailed(qint16 type)
{
	Q_Q(Im);

	switch(type) {
case YP_TYPE_PLUSH:
	{

		break;
	}
default:
	break;
	}
}
#endif

Im::Im(QObject *parent)
	: QObject(parent),
	d_ptr(new ImPrivate())
{
	d_ptr->q_ptr = this;
	Q_D(Im);	

	d->init();
}

Im::Im(ImPrivate& dd, QObject* parent /* = 0 */) :
QObject(parent),
	d_ptr(&dd)
{

	d_ptr->q_ptr = this;

	Q_D(Im);
	d_ptr->init();
}


Im::~Im()
{

}

QString Im::loginUid() const 
{
	Q_D(const Im);
	return d->m_loginUid;
}

void Im::setLoginUid(const QString& uid)
{
	Q_D(Im);
	d->m_loginUid = uid;
}

QString Im::loginToken() const 
{
	Q_D(const Im);
	return d->m_loginToken;
}

void Im::setLoginToken(const QString& token)
{
	Q_D(Im);
	d->m_loginToken = token;
}

int Im::loginUserType() const 
{
	Q_D(const Im);
	return d->m_loginUserType;
}

void Im::setLoginUserType(const int userType)
{
	Q_D(Im);
	d->m_loginUserType = userType;
}

int Im::loginNetworkType() const 
{
	Q_D(const Im);
	return d->m_loginNetworkType;
}

void Im::setLoginNetworkType(const int networkType)
{
	Q_D(Im);
	d->m_loginNetworkType = networkType;
}

QString Im::imAddress() const 
{
	Q_D(const Im);
	return d->m_hostAddress;
}

void Im::setImAddress(const QString& address)
{
	Q_D(Im);
	d->m_hostAddress = address;
}

quint16 Im::imPort() const 
{
	Q_D(const Im);

	return d->m_port;
}

void Im::setImPort(const quint16 port)
{
	Q_D(Im);

	d->m_port = port;
}

bool Im::login()
{
	Q_D(Im);

	return d->sendLoginPack();
}

bool Im::logout()
{
	Q_D(Im);

	return d->sendQuitPack();
}

bool Im::reconnect()
{
	Q_D(Im);

	return d->reconnect();
}

} // namespace ns

#include "moc_im.cpp"