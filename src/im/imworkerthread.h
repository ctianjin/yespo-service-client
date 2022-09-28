#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <QtNetwork/QUdpSocket>
#include "imglobal.h"

namespace ns {

class ImUdpPackage;
class ImReTransQueue;
class ImWorkerThread : public QThread
{
	Q_OBJECT

public:
	ImWorkerThread(QObject *parent = 0);
	~ImWorkerThread();

	void setPluseData(QByteArray& uid);
	void setWorkerSocket(QUdpSocket* socket);
	bool isDataNull();
	void setSocketData(QHostAddress& address, qint16 port);
	void setPlusePack(ImUdpPackage* udpPack);
	ImUdpPackage* plusePack() const;
	void run();
	void setRetransQueue(ImReTransQueue* queue) {m_retransQueue = queue;}
	void quitThread(bool quitThread) {m_quitThread = quitThread;}
	void resetData();
Q_SIGNALS:
	void pluseFailed();
private:
	qint8 m_expire;
	qint32 m_retransTimes;
	bool m_quitThread;
	QByteArray m_uid;
	ImUdpPackage* m_plusePack;
	QUdpSocket* m_socket;
	QHostAddress m_address;
	quint16 m_port;
	ImReTransQueue* m_retransQueue;
	int m_interval;
};

} // namespace ns

#endif // WORKERTHREAD_H
