#ifndef IM_P_H
#define IM_P_H

#include <QUdpSocket>
#include <QTimer>
#include <QMutex>
#include "im.h"

QT_BEGIN_NAMESPACE
class QTimer;
class QUdpSocket;
class QByteArray;
QT_END_NAMESPACE

namespace ns {

class ImUdpPackage;
class ImReTransQueue;
class ImWorkerThread;

class ImPrivate
{
	Q_DECLARE_PUBLIC(Im)

	enum Status {
			Login = 0,
			Plus,
			Quit
		};

public:
	ImPrivate();
	virtual ~ImPrivate();

	Im* q_ptr;

	Status m_currentStatus;
	QTimer *m_timer;
	ImUdpPackage* m_udpPack;
	QUdpSocket* m_socket;
	quint16 m_port;
	ImWorkerThread* m_workerThread;
	QByteArray m_loginPack;
	QByteArray m_uid;
	QString m_hostAddress;
	ImReTransQueue* m_retransQueue;
	bool m_isLogin;

	QString m_loginUid;
	QString m_loginToken;
	int m_loginUserType;
	int m_loginNetworkType;

	//slot
	void _q_processReadData();
	void _q_slotTimeout();
	void _q_pluseFailed();
	void _q_retransFailed(qint16 type);

	bool sendQuitPack();
	bool sendLoginPack();
	bool reconnect();
private:
	void init();
	int parsePack(QByteArray& pack);
	void sendAckPack();

	void addRetransPackage(ImUdpPackage* udpPack);
	void removeRetransPackage(qint16 clientSeq);
};

}


#endif // IM_P_H