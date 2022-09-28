#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui/QWidget>
#include <QScopedPointer>
#include "request.h"

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

namespace ns {

class YespoProto;

class Ui_MainWidget;
class MainWidget : public QWidget
{
    Q_OBJECT
	
public:
    explicit MainWidget(QWidget* parent = 0);
    virtual ~MainWidget();
	
public Q_SLOTS:
	void on_pushButtonConnect_clicked();
	void on_pushButtonDisconnect_clicked();
	void on_pushButtonLogin_clicked();
	void on_pushButtonLogout_clicked();
	void on_pushButtonStartCall_clicked();

protected:


private Q_SLOTS:
	void slotSocketError(const QString& errorMsg);
	void slotConnected();
	void slotDisconnected();
	void slotLoginFinished(int rspCode);
	void slotLogoutFinished();
	void slotStartCallFinished(const RoomDistributeRsp& rsp);

private:
    QScopedPointer<Ui_MainWidget> m_ui;
	QScopedPointer<YespoProto> m_proto;


    Q_DISABLE_COPY(MainWidget)
};

}

#endif // MAINWIDGET_H
