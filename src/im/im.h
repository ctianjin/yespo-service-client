#ifndef IM_H
#define IM_H

#include <QObject>
#include "imglobal.h"
#include "imcommand.h"

QT_BEGIN_NAMESPACE
class QTimer;
class QTime;
class QUdpSocket;
class QMutex;
QT_END_NAMESPACE

namespace ns {

class ImPrivate;

class BUILDSYS_IM_DECL Im : public QObject
{
	Q_OBJECT

public:
	explicit Im(QObject *parent = 0);
	~Im();

	QString loginUid() const;
	void setLoginUid(const QString& uid);

	QString loginToken() const;
	void setLoginToken(const QString& token);

	int loginUserType() const;
	void setLoginUserType(const int userType);

	int loginNetworkType() const;
	void setLoginNetworkType(const int networkType);

	QString imAddress() const;
	void setImAddress(const QString& address);

	quint16 imPort() const;
	void setImPort(const quint16 port);

	bool login();
	bool logout();
	bool reconnect();

Q_SIGNALS:
	void parseMsgFinished(const ImCommand::GetUploadPhotoMsgResult& result);
	void parseMsgError();
	void loginSucceed();
	void loginFailed(const QString& reason);
	void pluseFailed();
	void requestQuit();

protected:
	Im(ImPrivate& dd, QObject* parent);
	ImPrivate* d_ptr;

private:
	Q_DECLARE_PRIVATE(Im)
	Q_DISABLE_COPY(Im)

	Q_PRIVATE_SLOT(d_func(), void _q_processReadData())
	Q_PRIVATE_SLOT(d_func(), void _q_slotTimeout())
	Q_PRIVATE_SLOT(d_func(), void _q_pluseFailed())
	Q_PRIVATE_SLOT(d_func(), void _q_retransFailed(qint16))

};

} // namespace ns

#endif // IM_H
