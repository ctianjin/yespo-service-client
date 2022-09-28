#ifndef RETRANSQUEUE_H
#define RETRANSQUEUE_H

#include <QThread>
#include <QMutex>
#include <QtNetwork/QUdpSocket>
#include "imglobal.h"

QT_BEGIN_NAMESPACE
class QUdpSocket;
class QMutex;
QT_END_NAMESPACE


namespace ns {

class ImUdpPackage;

class ImReTransQueue : public QThread
{
	Q_OBJECT

public:
	ImReTransQueue(QObject *parent = 0);
	~ImReTransQueue();

	void setRetransSocket(QUdpSocket* socket) {m_socket = socket;}
	QList <ImUdpPackage*> queue() const {return m_udpPackQueue;}
	bool isDataNull();
	void setSocketData(QHostAddress& address, qint16 port);
	void run();
	void add(const ImUdpPackage& udpPack);
	void remove(qint16 clientSeq);
	void remove(ImUdpPackage* udpPack);
	bool contains(qint16 clientSeq);
	void clearQueue();
	void quitQueue(bool quitRetransmit) {m_quitRetransmit = quitRetransmit;}

Q_SIGNALS:
	void retransFailed(qint16 packType);
private:
	QMutex m_mutex;
	bool m_quitRetransmit;
	QUdpSocket* m_socket;
	ImUdpPackage* m_retransPack;
	QHostAddress m_hostAddress;
	quint16 m_port;
	int m_retransCount;
	QList<ImUdpPackage*> m_udpPackQueue;
	QList<ImUdpPackage* >::iterator m_it;
};

} // namespace ns

#endif // RETRANSQUEUE_H
