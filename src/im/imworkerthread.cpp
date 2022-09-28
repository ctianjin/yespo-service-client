#include "imworkerthread.h"
#include "imudppackage.h"
#include "imretransqueue.h"
#include <QtEndian>
#define PLUSETIME 10

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "ImWorkerThread:"
#endif // DEBUG_PREFIX

namespace ns {

ImWorkerThread::ImWorkerThread(QObject *parent)
	: QThread(parent),
	m_address(QHostAddress::Null),
	m_port(9500),
	m_plusePack(0),
	m_uid(0),
	m_retransQueue(0),
	m_quitThread(false),
	m_expire(0),
	m_retransTimes(0),
	m_interval(0)
{
	m_plusePack = new ImUdpPackage();
}

ImWorkerThread::~ImWorkerThread()
{
	resetData();
}

void ImWorkerThread::setPluseData(QByteArray& uid)
{
	m_uid = uid;
}

bool ImWorkerThread::isDataNull()
{
	return (m_address.isNull() || m_port == 0);
}

void ImWorkerThread::setWorkerSocket(QUdpSocket* socket)
{
	if (!socket)
		return;

	m_socket = socket;
}

void ImWorkerThread::setSocketData(QHostAddress& address, qint16 port)
{
	m_address = address;
	m_port = port;
}

void ImWorkerThread::setPlusePack(ImUdpPackage* udpPack) 
{
	m_plusePack->setServerSeq(udpPack->serverSeq());
	m_plusePack->setBlockNum(udpPack->blockNum());
	m_plusePack->setBlockSeq(udpPack->blockSeq());
	m_plusePack->setSessionId(udpPack->sessionId());
	m_plusePack->setAck(1);
	m_plusePack->setProVersion(udpPack->proVersion());
	m_plusePack->setClientSeq(udpPack->clientSeq());
	m_plusePack->setCheckSum(udpPack->checkSum());
	m_plusePack->setDataLen(/*qToBigEndian*/(qint16)m_uid.length());
	m_plusePack->setReserved(udpPack->reserved());
	m_plusePack->setPackType(/*qToBigEndian*/(qint16)3);
	m_plusePack->setResult(/*qToBigEndian*/(qint16)0);
	m_plusePack->setData(m_uid);
}

ImUdpPackage* ImWorkerThread::plusePack() const 
{
	return m_plusePack;
}

void ImWorkerThread::resetData()
{
	m_expire = 0;
	m_retransTimes = 0;
}

void ImWorkerThread::run()
{
	while(!m_quitThread) {
		if(m_interval == 10) {
			m_interval = 0;
			//10s
			if (m_expire > 0 && m_retransTimes >= 3) {
				Q_EMIT pluseFailed();
				break;
			}
			QByteArray pack; 
			if(m_quitThread)
				break;
			m_plusePack->setClientSeq(m_plusePack->clientSeq() + 1);
			if(m_quitThread)
				break;
			m_plusePack->packPack(pack);
			if(m_quitThread)
				break;
			m_socket->writeDatagram(m_plusePack->packageData(), m_address, m_port);
			m_expire = PLUSETIME;
			m_retransTimes += 1;
		}
		++m_interval;
		sleep(1);
	}
}

} // namespace ns