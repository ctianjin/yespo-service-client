#ifndef REMOTEVIDEODIALOG_H
#define REMOTEVIDEODIALOG_H

#include <QMetaType>
#include <QScopedPointer>
//#include <QPointer>
#include <QString>
#include <QIcon>
#include "remotivideosetting.h"
#include "framelessvideodialog.h"

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

namespace ns {

class Ui_RemoteVideoDialog;
class PromptDialog;
class RemoteVideoSetting;
class SipService;

class RemoteVideoDialog : public FramelessVideoDialog
{
    Q_OBJECT
	Q_PROPERTY(SipService* sipService READ sipService WRITE setSipService)

	static const QString stylesheetNetworkDisable;
	static const QString stylesheetNetworkEnable;

public:
    explicit RemoteVideoDialog(QWidget* parent = 0);
    virtual ~RemoteVideoDialog();

	SipService* sipService() const;
	void setSipService(SipService* sipService);

	QWidget* widgetVideo() const;

	void setPosition(int position);

	void closeWindow();

	void startDuration();

	void stopDuration();

	void startNetworkDetect();

	void stopNetworkDetect();

	void resetCommonControls();

Q_SIGNALS:
	void userCancelled();
	
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void changeEvent(QEvent* event);
	virtual bool eventFilter(QObject *obj, QEvent *event);
	virtual void resizeEvent(QResizeEvent* event);

private Q_SLOTS:
	void slotShowFullScreen(bool fullScreen);
	void slotMove(const QPoint& point);
	void on_pushButtonMaxmize_clicked();
	void on_pushButtonNormal_clicked();
	void on_pushButtonSetting_clicked();
	void on_pushButtonMax_clicked();
	void on_pushButtonMin_clicked();
	void slotRotationChanged(ns::RemoteVideoSetting::RotationStyle style);
	void slotProportionChanged(ns::RemoteVideoSetting::ProportionStyle style);
	void slotBrightnessChanged(int brightness);
	void slotDuration();
	void slotNetworkDetect();
private:
	int countFlag(QPoint p, int row);
	void setCursorType(int flag);
	int countRow(QPoint p);
	void showFull(bool fullScreen);
	void proportionChanged(int proportionWidth, int proportionHeight, int proportion);
	void rotationExchanged();
	void detectNetworkResult(quint32 byteRecv);
	void notByteRecv();
	void byteRecvOne();
	void byteRecvTwo();
	void byteRecvThree();
	void byteRecvFour();
	void byteRecvNormal();
	bool m_showFullScreen;
	bool m_isLeftPressed;
	int m_curPos;
	QPoint m_pLast;

	int m_position;
	int m_proportionW;
	int m_proportionH;
	int m_proportion;
	int m_duration;
	int m_rotation;

	quint32 m_rtpByteRecv;

	QTimer* m_timer;
	QTimer* m_networkTimer;
	QPointer<SipService> m_sipService;
	QPointer<PromptDialog> m_promptDialog;
    QScopedPointer<Ui_RemoteVideoDialog> m_ui;
	QScopedPointer<RemoteVideoSetting> m_remoteVideoSetting;

    Q_DISABLE_COPY(RemoteVideoDialog)
};

} // namespace ns


#endif // REMOTEVIDEODIALOG_H
