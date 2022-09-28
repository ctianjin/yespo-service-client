#include "imretransqueue.h"
#include "imudppackage.h"
#include "improtocol.h"
#include <QtNetwork/QUdpSocket>
#include <QTime>

namespace ns {

ImReTransQueue::ImReTransQueue(QObject *parent)
	: QThread(parent),
	m_quitRetransmit(false),
	m_retransPack(),
	m_retransCount(0)
{
	m_udpPackQueue.clear();
}

ImReTransQueue::~ImReTransQueue()
{
	if (m_retransPack != NULL) {
		delete m_retransPack;
		m_retransPack = NULL;
	}
	quit();
	wait();
}

bool ImReTransQueue::isDataNull()
{
	return (m_hostAddress.isNull() || m_port == 0);
}

void ImReTransQueue::setSocketData(QHostAddress& address, qint16 port)
{
	m_hostAddress = address;
	m_port = port;
}

void ImReTransQueue::run()
{
	qint8 retransCount = 0;
	ImUdpPackage* tempPack;
	int index = 0;
	while (!m_quitRetransmit) {
		m_mutex.lock();
		if (m_udpPackQueue.size() > 0) {
			m_retransPack = m_udpPackQueue.at(0);
			tempPack = m_udpPackQueue.at(0);
		} else {
			m_retransPack = NULL;
		}
		m_mutex.unlock();

		if (m_retransPack != NULL) {
			if(m_retransPack->retransCount() < RETRANSMITCOUNT && m_retransPack->timeStamp().secsTo(QTime::currentTime()) >= RETRANSTIMESTAMP) {
				m_retransCount = m_retransPack->retransCount();
				QByteArray package;
				//m_retransPack->packPack(package);
				m_socket->writeDatagram(m_retransPack->packageData(), QHostAddress(m_hostAddress), m_port);
				m_retransPack->setRetransCount(++m_retransCount);
				m_retransPack->setTimeStamp(QTime::currentTime());
			}
			if (m_retransPack->retransCount() >= RETRANSMITCOUNT) {
				Q_EMIT retransFailed(m_retransPack->packType());
				m_mutex.lock();
				if (!m_udpPackQueue.removeOne(tempPack)) {
					
				}
				m_mutex.unlock();
				m_retransPack = NULL;
			}
		}
	}
}

void ImReTransQueue::add(const ImUdpPackage& udpPack)
{
 	ImUdpPackage* pack = new ImUdpPackage(udpPack);
	pack->setRetransCount(0);
	pack->setTimeStamp(QTime::currentTime());
	m_mutex.lock();
	m_udpPackQueue.append(pack);
	m_mutex.unlock();
}

void ImReTransQueue::remove(qint16 clientSeq)
{
	QMutexLocker l(&m_mutex);

	for (int i = 0; i < m_udpPackQueue.size(); i++) {
		if (m_udpPackQueue.at(i)->clientSeq() == clientSeq) {
			m_udpPackQueue.removeAt(i);
			break;
		}
	}
}

void ImReTransQueue::remove(ImUdpPackage* udpPack)
{
	QMutexLocker l(&m_mutex);

/*	ImUdpPackage* pack = new ImUdpPackage(udpPack);*/

	m_udpPackQueue.removeOne(udpPack);

/*	delete pack;*/
}

bool ImReTransQueue::contains(/*ImUdpPackage* udpPack*/qint16 clientSeq)
{
	QMutexLocker l(&m_mutex);

	bool result = false;
	for (int i = 0; i < m_udpPackQueue.size(); i++) {
		if(m_udpPackQueue.at(i)->clientSeq() == clientSeq) {
			result = true;
			break;
		}
	}
	return result;
}

void ImReTransQueue::clearQueue()
{
	QMutexLocker l(&m_mutex);

	m_udpPackQueue.clear();
}

} // namespace ns