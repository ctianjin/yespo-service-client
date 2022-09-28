#ifndef SIPSERVICE_P_H
#define SIPSERVICE_P_H

#include <QtCore/QPointer>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include "sipservice.h"
#include "abstractservice_p.h"
#include <pjsua.h>
#include <mediacore.h>

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

namespace ns {

class SipServicePrivate : public AbstractServicePrivate
{
    Q_DECLARE_PUBLIC(SipService)

public:
	QPointer<UserService> m_userService;
	pjsua_acc_id m_accountID;

	QString m_streamWave;
	pjsua_player_id m_playerID;
	pjsua_recorder_id m_recorderID;

	QString m_userAgent;

	MediaCore m_webrtcMedia;
	QHash<QChar, int> m_eventCode;

    SipServicePrivate(ServiceManager* manager, const QString& name);
    virtual ~SipServicePrivate();

	static SipServicePrivate* instance();

	int registerServer(const QString& account, const QString& secret, const QString& domain);
	void unregisterServer();
	void unregisterServer(int accountId);

	int maxAccountCount() const;

	int makeCall(int accountId, const QString& dstUri, const QString& srcLang, 
		const QString& tarLang, const QString& service, const QString& country);
	void ringringCall(int callId);
	void answerCall(int callId);
	void rejectCall(int callId);
	void sendDtmf(int callId, const QChar& dtmf);
	void hangupCall(int callId);
	void muteCall(int callId, bool mute);
	int callVolume(int callId);
	void setCallVolume(int callId, unsigned vol);
	void callQuality(int callId, float* rxLossPercent, float* txLossPercent); 

	int buddySubscribe(const QString& buddyURI);
	void buddySubscribeCancel(int buddyID);

	void streamWave(const QString& fileName);
	void destroyStream();
	void recordCall(const QString& fileName);
	void destroyRecord();

	void setVideoWindow(void* local, void* remote);

	void setCodecSize(int width, int height);
	void setVideoIncoming(int callId, bool on);

	void setVideoOpenWin();
	void setVideoCloseWin();

	void prepareVideoStream();
	void stopPsdVideoStream();

	bool capture(const QString& fileName);

	void setVideoRotation(int angle);

	void setVideoBrightness(int brightness);

	void getRtpInfo(unsigned int& byte);

protected:
	virtual void startup();
    virtual void cleanup();

	static void on_log(int level, const char *data, int len);
	/* Callback called by the library when register status has changed*/
	static void on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info);
	/* Callback called by the library upon receiving incoming call */
	static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata);
	/* Callback called by the library when call's state has changed */
	static void on_call_state(pjsua_call_id call_id, pjsip_event *e);
	/* Callback called by the library when call's media state has changed */
	static void on_call_media_state(pjsua_call_id call_id);
	/* Callback called by the library when receive text */
	static void on_pager(pjsua_call_id call_id, const pj_str_t *from,
		const pj_str_t *to, const pj_str_t *contact,
		const pj_str_t *mime_type, const pj_str_t *body);
	/* Callback when received dtmf */
	static void on_dtmf_digit(pjsua_call_id call_id, int digit);
	/* Callback when buddy state has changed*/
	static void on_buddy_state(pjsua_buddy_id buddy_id);
	/* */
	static pjmedia_transport* on_create_media_transport(pjsua_call_id call_id,
		unsigned media_idx,
		pjmedia_transport *base_tp,
		unsigned flags);
	/* */
	static pj_status_t send_rtcp(pjmedia_transport *tp, const void *pkt, pj_size_t size);
	/* */
	static pj_status_t send_rtcp2(pjmedia_transport *tp, const pj_sockaddr_t *addr, unsigned addr_len, const void *pkt, pj_size_t size);
	/* */
	static void on_call_media_socket_create(pjsua_call_id call_id, int rtp_port, int rtcp_port);
	/* */
	static void on_call_sdp_created(pjsua_call_id call_id,
		pjmedia_sdp_session *sdp,
		pj_pool_t *pool,
		const pjmedia_sdp_session *rem_sdp);
	/* */
	static void on_nat_detect(const pj_stun_nat_detect_result *res);
	/* Display error and exit application */
	static void error_exit(const char *title, pj_status_t status);

private:
    void init();
    void clearResult();
	
	static QString headerValue(const pjsip_msg* msg, const QString& header);
	QString errorText(int status);

	static SipServicePrivate* m_instance;

	void sipLog(const QString& msg);
	void regState(int accountid, int code, const QString& reason);
	//void incomingCall(const SIP::CallInParam& param);
	void incomingCall(const QStringList& callInList);
	void callState(int callId, int state);
	void callMediaState(int callId);
	void receiveText(int callId, const QString& remote, const QString& text);
	void receiveDtmf(int callId, int digit);
	void buddyState(int buddyID);
	void callStreamRecv(int audioLocalPort, int videoLocalPort);
	void prepareCall(int rtp_port, int rtcp_port);
	void errorExit();

	void* m_widgetLocal;
	void* m_widgetRemote;

	QPixmap m_pixmapCapture;

	int m_audioLocalPort;
	int m_videoLocalPort;

	int m_codecWidth;
	int m_codecHeight;

	QString m_addr;
	int m_port;
};

} // namespace ns

#endif // SIPSERVICE_P_H
