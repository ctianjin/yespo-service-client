#include <QtGui/QMessageBox>
#include <QDebug>
#include "mainwidget.h"
#include "ui_mainwidget.h"

#include "yespoproto.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "MainWidget:"
#endif // QPID_DEBUG_PREFIX

namespace ns {

MainWidget::MainWidget(QWidget* parent) :
    QWidget(parent),
    m_ui(new Ui_MainWidget)
{
    m_ui->setupUi(this);

	m_proto.reset(new YespoProto(this));

	connect(m_proto.data(), SIGNAL(socketError(const QString&)), this, SLOT(slotSocketError(const QString&)));
	connect(m_proto.data(), SIGNAL(connected()), this, SLOT(slotConnected()));
	connect(m_proto.data(), SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
	connect(m_proto.data(), SIGNAL(loginFinished(int)), this, SLOT(slotLoginFinished(int)));
	connect(m_proto.data(), SIGNAL(logoutFinished()), this, SLOT(slotLogoutFinished()));
	connect(m_proto.data(), SIGNAL(startCallFinished(const RoomDistributeRsp&)), this, SLOT(slotStartCallFinished(const RoomDistributeRsp&)));

}

MainWidget::~MainWidget()
{
	
}

void MainWidget::on_pushButtonConnect_clicked()
{
	m_proto->connect(QString::fromLocal8Bit("115.159.118.223"), 9500);

}

void MainWidget::on_pushButtonDisconnect_clicked()
{
	m_proto->disconnect();
} 

void MainWidget::on_pushButtonLogin_clicked()
{
	LoginReq req;
	req.setUserId(100123456789);
	req.setUserType(1);
	req.setToken(QString::fromLocal8Bit("asdfasfaserqwerasdfaser"));
	m_proto->login(req);
}

void MainWidget::on_pushButtonLogout_clicked()
{
	LogoutReq req;
	req.setUserId(100123456789);
	m_proto->logout(req);
}

void MainWidget::on_pushButtonStartCall_clicked()
{
	RoomDistributeReq req;

	req.setUserId(100123456789);
	req.setPartnerId(900123456789);

	m_proto->startCall(req);
}

void MainWidget::slotSocketError(const QString& errorMsg)
{
	QMessageBox::warning(this, QString::fromLocal8Bit("Socket Error"), errorMsg);
}

void MainWidget::slotConnected()
{
}

void MainWidget::slotDisconnected()
{

}

void MainWidget::slotLoginFinished(int rspCode)
{
// 	LogoutReq req;
// 	req.setUserId(100123456789);
// 	m_proto->logout(req);

}

void MainWidget::slotLogoutFinished()
{
// 	LoginReq req;
// 	req.setUserId(100123456789);
// 	req.setUserType(1);
// 	req.setToken(QString::fromLocal8Bit("asdfasfaserqwerasdfaser"));
// 	m_proto->login(req);
}

void MainWidget::slotStartCallFinished(const RoomDistributeRsp& rsp)
{
	qDebug() << DEBUG_PREFIX << rsp.rspMsg().rspCode() << rsp.rspMsg().rspMsg() << rsp.userId() << rsp.roomAddress() << rsp.cdrId() << rsp.recordPath() << rsp.parentCdrId();
}


} // namespace ns
