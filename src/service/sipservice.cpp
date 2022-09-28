#include <QDebug>
#include <QWidget>
#include "sipservice.h"
#include "sipservice_p.h"
#include "userservice.h"
#include "servicemanager.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "SipService:"
#endif // DEBUG_PREFIX

namespace ns {

class SipServiceStr 
{
public:
	SipServiceStr(const QString& s) : m_str(NULL) {
		m_str = strdup(s.toLatin1().constData());
	}
	~SipServiceStr() {
		if (m_str) {
			free(m_str);
			m_str = NULL;
		}
	}

	char* str() {
		return m_str;
	}
private:
	char* m_str;

	SipServiceStr(const SipServiceStr& rhs);
	SipServiceStr& operator=(const SipServiceStr& rhs);
};

struct SIPExtended
{
	static const QString sourceLanguage;
	static const QString targetLanguage;
	static const QString callMode;
	static const QString serviceMode;
	static const QString callCountry;
	static const QString calleeNumber;

	//local partner
	static const QString localPartner;
	static const QString localCountry;
	static const QString localLanguage;

	//uploadphoto
	static const QString photoId;
	//call record ID
	static const QString callRecordId;
};

const QString SIPExtended::sourceLanguage = QLatin1String("X-Source-Lang");
const QString SIPExtended::targetLanguage = QLatin1String("X-Target-Lang");
const QString SIPExtended::callMode = QLatin1String("X-Call-Mode");
const QString SIPExtended::serviceMode = QLatin1String("X-Service-Mode");
const QString SIPExtended::callCountry = QLatin1String("X-Call-Country");
const QString SIPExtended::calleeNumber = QLatin1String("X-Callee-Number");
//local partner
const QString SIPExtended::localPartner = QLatin1String("X-Local-Partner");
const QString SIPExtended::localCountry = QLatin1String("X-Local-Country");
const QString SIPExtended::localLanguage = QLatin1String("X-Local-Language");

//uploadphoto
const QString SIPExtended::photoId = QLatin1String("X-Call-Photos");
const QString SIPExtended::callRecordId = QLatin1String("X-Call-Cdr-ID");
SipServicePrivate* SipServicePrivate::m_instance = NULL;

SipServicePrivate::SipServicePrivate(ServiceManager* manager, const QString& name) :
    AbstractServicePrivate(manager, name),
	m_accountID(-1),
	m_playerID(-1),
	m_recorderID(-1),
	m_userAgent(),
	m_streamWave()
{
	if (m_instance == NULL) 
		m_instance = this;

	m_eventCode.insert(QLatin1Char('0'), 0);
	m_eventCode.insert(QLatin1Char('1'), 1);
	m_eventCode.insert(QLatin1Char('2'), 2);
	m_eventCode.insert(QLatin1Char('3'), 3);
	m_eventCode.insert(QLatin1Char('4'), 4);
	m_eventCode.insert(QLatin1Char('5'), 5);
	m_eventCode.insert(QLatin1Char('6'), 6);
	m_eventCode.insert(QLatin1Char('7'), 7);
	m_eventCode.insert(QLatin1Char('8'), 8);
	m_eventCode.insert(QLatin1Char('9'), 9);
	m_eventCode.insert(QLatin1Char('*'), 10);
	m_eventCode.insert(QLatin1Char('#'), 11);
	m_eventCode.insert(QLatin1Char('A'), 12);
	m_eventCode.insert(QLatin1Char('B'), 13);
	m_eventCode.insert(QLatin1Char('C'), 14);
	m_eventCode.insert(QLatin1Char('D'), 15);
}

SipServicePrivate::~SipServicePrivate()
{
	/* Destroy pjsua */
	pjsua_destroy();

    cleanup();

	m_webrtcMedia.DeInitStack();
}

void SipServicePrivate::init()
{
    Q_Q(SipService);
    Q_ASSERT(q);
    Q_ASSERT(m_manager);

	/* Init WebRTC Stack */
 	bool result = m_webrtcMedia.InitStack();
 	if (!result) {
 		setError(Service::InternalError);
 		return;
 	}

	pj_status_t status;

	/* Create pjsua first! */
	status = pjsua_create();
	if (status != PJ_SUCCESS) {
		setError(Service::InternalError);
		return;
	}

	/* Init pjsua */
	{
		pjsua_config global_cfg;
		pjsua_logging_config log_cfg;
		pjsua_media_config med_cfg;

		pjsua_config_default(&global_cfg);

		global_cfg.max_calls = 10;

		SipServiceStr userAgent(m_userAgent);
		global_cfg.user_agent = pj_str(userAgent.str());

		global_cfg.cb.on_reg_state2 = &on_reg_state2;
		global_cfg.cb.on_incoming_call = &on_incoming_call;
		global_cfg.cb.on_call_media_state = &on_call_media_state;
		global_cfg.cb.on_call_state = &on_call_state;
		global_cfg.cb.on_dtmf_digit = &on_dtmf_digit;
		global_cfg.cb.on_pager = &on_pager;
		global_cfg.cb.on_buddy_state = &on_buddy_state;
		global_cfg.cb.on_create_media_transport = &on_create_media_transport;
		global_cfg.cb.on_call_media_socket_create = &on_call_media_socket_create;
		global_cfg.cb.on_call_sdp_created = &on_call_sdp_created;
		global_cfg.cb.on_nat_detect = &on_nat_detect;

		pjsua_logging_config_default(&log_cfg);
		log_cfg.level = 5;
		log_cfg.console_level = 5;
		log_cfg.cb = &on_log;

		pjsua_media_config_default(&med_cfg);
		med_cfg.ice_no_rtcp = PJ_TRUE;
		med_cfg.no_vad = PJ_TRUE;

		status = pjsua_init(&global_cfg, &log_cfg, &med_cfg);
		if (status != PJ_SUCCESS) {
			setError(Service::InternalError);
			return;
		}
	}

	/* Add UDP transport. */
	{
		pjsua_transport_config transport_cfg;

		pjsua_transport_config_default(&transport_cfg);
		transport_cfg.port_range = 65500;
//		status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transport_cfg, NULL);
		status = pjsua_transport_create(PJSIP_TRANSPORT_TCP, &transport_cfg, NULL);
		if (status != PJ_SUCCESS) {
			setError(Service::InternalError);
			return;
		}
	}

	/* Initialization is done, now start pjsua */
	status = pjsua_start();
	if (status != PJ_SUCCESS) {
		setError(Service::InternalError);
		return;
	}

	/* Disconnect the main conference bridge 
	*  from any sound devices 
	*/
	pjsua_set_no_snd_dev();

	pjsua_codec_info id[256];
	unsigned int count = 256;
	status = pjsua_enum_codecs(id, &count);
	if (status == PJ_SUCCESS) {
		int i = 0;
		for (i = 0; i < count; ++i) 
			qDebug() << id[i].codec_id.ptr << id[i].priority;
	}

	/* disable codec not used yet */
	pj_str_t pjCodec = pj_str("opus/48000");
	pjsua_codec_set_priority(&pjCodec, 253);

	pjCodec = pj_str("PCMA/8000");
	pjsua_codec_set_priority(&pjCodec, 0);

	pjCodec = pj_str("PCMU/8000");
	pjsua_codec_set_priority(&pjCodec, 0);

	pjCodec = pj_str("iLBC/8000");
	pjsua_codec_set_priority(&pjCodec, 0);

	pjCodec = pj_str("GSM/8000");
	pjsua_codec_set_priority(&pjCodec, 0);

	pjCodec = pj_str("speex/8000");
	pjsua_codec_set_priority(&pjCodec, 0);

	pjCodec = pj_str("speex/16000");
	pjsua_codec_set_priority(&pjCodec, 0);

	pjCodec = pj_str("speex/32000");
	pjsua_codec_set_priority(&pjCodec, 0);
}

SipServicePrivate* SipServicePrivate::instance()
{
	return m_instance;
}

int SipServicePrivate::registerServer(const QString& account, const QString& secret, const QString& domain)
{
	SipServiceStr id(QLatin1String("sip:")+account+QLatin1String("@")+domain);
	SipServiceStr reguri(QLatin1String("sip:")+domain);
	SipServiceStr username(account);
	SipServiceStr data(secret);
	
	SipServiceStr proxy(QLatin1String("sip:")+domain+QLatin1String(";transport=tcp"));

	/* Register to SIP server by creating SIP account. */
	pj_status_t status;
	pjsua_acc_config cfg;

	pjsua_acc_config_default(&cfg);
	cfg.id = pj_str(id.str());
	cfg.reg_uri = pj_str(reguri.str());
	cfg.cred_count = 1;
	cfg.cred_info[0].realm = pj_str("*");
	cfg.cred_info[0].scheme = pj_str("digest");
	cfg.cred_info[0].username = pj_str(username.str());
	cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	cfg.cred_info[0].data = pj_str(data.str());

	cfg.proxy[cfg.proxy_cnt++] = pj_str(proxy.str());

	cfg.drop_calls_on_reg_fail = PJ_TRUE;

 	cfg.rtp_cfg.port = 20000;
 	cfg.rtp_cfg.port_range = 30000;

	cfg.reg_timeout = 60;

	status = pjsua_acc_add(&cfg, PJ_TRUE, &m_accountID);
	if (status != PJ_SUCCESS) {
		setError(Service::InternalError);
		return -1;
	}
	return m_accountID;
}

void SipServicePrivate::unregisterServer()
{
	if (m_accountID >= 0) {
		pjsua_acc_del(m_accountID);
		m_accountID = -1;
	}
}

void SipServicePrivate::unregisterServer(int accountId)
{
	if (accountId >=0) {
		pjsua_acc_del(accountId);
	} else {
		qWarning() << DEBUG_PREFIX << "Invalid account ID : " << accountId;
	}
	
}

int SipServicePrivate::maxAccountCount() const
{
	return (int)PJSUA_MAX_ACC;
}

int SipServicePrivate::makeCall(int accountId, const QString& dstUri, const QString& srcLang, 
			const QString& tarLang, const QString& service, const QString& country)
{
	pj_status_t status;

	status = pjsua_set_snd_dev(0, 0);
	if (status != PJ_SUCCESS) {
// 		setError(Service::MakeCallError, errorText(status));
// 		return -1;
	} 

	pjsua_msg_data msg;
	pjsua_msg_data_init(&msg);

	pjsip_generic_string_hdr sourceLanguageHeader;
	pjsip_generic_string_hdr targetLanguageHeader;
	pjsip_generic_string_hdr serviceModeHeader;
	pjsip_generic_string_hdr callCountryHeader;

	SipServiceStr sourceLanguageName(SIPExtended::sourceLanguage);
	SipServiceStr targetLanguageName(SIPExtended::targetLanguage);
	SipServiceStr serviceModeName(SIPExtended::serviceMode);
	SipServiceStr callCountryName(SIPExtended::callCountry);

	SipServiceStr sourceLanguageValue(srcLang);
	SipServiceStr targetLanguageValue(tarLang);
	SipServiceStr serviceModeValue(service);
	SipServiceStr callCountryValue(country);

	if (!srcLang.isEmpty()) {
		pjsip_generic_string_hdr_init2(&sourceLanguageHeader, &pj_str(sourceLanguageName.str()), &pj_str(sourceLanguageValue.str()));
		pj_list_push_back(&msg.hdr_list, &sourceLanguageHeader);
	}

	if (!tarLang.isEmpty()) {
		pjsip_generic_string_hdr_init2(&targetLanguageHeader, &pj_str(targetLanguageName.str()), &pj_str(targetLanguageValue.str()));
		pj_list_push_back(&msg.hdr_list, &targetLanguageHeader);
	}

	if (!service.isEmpty()) {
		pjsip_generic_string_hdr_init2(&serviceModeHeader, &pj_str(serviceModeName.str()), &pj_str(serviceModeValue.str()));
		pj_list_push_back(&msg.hdr_list, &serviceModeHeader);
	}

	if (!country.isEmpty()) {
		pjsip_generic_string_hdr_init2(&callCountryHeader, &pj_str(callCountryName.str()), &pj_str(callCountryValue.str()));
		pj_list_push_back(&msg.hdr_list, &callCountryHeader);
	}

	pjsua_call_id callid = -1;
	status = pjsua_call_make_call(m_accountID, &pj_str(dstUri.toLatin1().data()), NULL, NULL, &msg, &callid);

	if (status != PJ_SUCCESS) {
		setError(Service::MakeCallError, errorText(status));
	} 

	return callid;
}

void SipServicePrivate::ringringCall(int callId)
{
	/* Ringring incoming calls with 180/Ringring */

	pj_status_t status;
	status = pjsua_call_answer(callId, 180, NULL, NULL);
	if (status != PJ_SUCCESS) {
		setError(Service::InternalError);
	}
}

void SipServicePrivate::answerCall(int callId)
{
	/* Answer incoming calls with 200/OK */

	pjsua_call_setting callSet;
	pjsua_call_setting_default(&callSet);
	callSet.aud_cnt = 1;
	callSet.vid_cnt = 1;
	pj_status_t status = pjsua_call_answer2(callId, &callSet, 200, NULL, NULL);
	
// 	pj_status_t status;
// 	status = pjsua_call_answer(callId, 200, NULL, NULL);
// 	if (status != PJ_SUCCESS) {
// 		setError(Service::InternalError);
// 	}
}

void SipServicePrivate::rejectCall(int callId)
{
	/* Reject incoming calls with 603/Decline */

	pj_status_t status;
	status = pjsua_call_hangup(callId, 603, NULL, NULL);
	if (status != PJ_SUCCESS) {
		setError(Service::InternalError);
	}
}

void SipServicePrivate::sendDtmf(int callId, const QChar& dtmf)
{
//	m_webrtcMedia.SendDTMF(m_eventCode.value(dtmf, 0));
// 	SipServiceStr tempDtmf(dtmf);
// 
// 	pj_status_t status;
// 	pj_str_t pjDtmf = pj_str(tempDtmf.str());
// 	status = pjsua_call_dial_dtmf(callId, &pjDtmf);
// 	if (status != PJ_SUCCESS) {
// 		setError(Service::InternalError);
// 	}
}

void SipServicePrivate::hangupCall(int callId)
{
	pjsua_call_hangup_all();

	/* Hangup all of current call */
// 	if (callId == -1) {
// 		pjsua_call_hangup_all();
// 	} else {
// 		pjsua_call_hangup(callId, 0, NULL, NULL);
// 	}
}

void SipServicePrivate::muteCall(int callId, bool mute)
{
	pjsua_call_info ci;
	pj_status_t status;
	status = pjsua_call_get_info(callId, &ci);
	if (status != PJ_SUCCESS) {
		setError(Service::InternalError);
		return;
	}

	if (ci.conf_slot == -1) {
		return;
	}

	if (mute) {
		pjsua_conf_adjust_tx_level(ci.conf_slot, 0);
	} else {
		pjsua_conf_adjust_tx_level(ci.conf_slot, 1.0);
	}
}

int SipServicePrivate::callVolume(int callId)
{
	unsigned vol = 0;
	pj_status_t status;
	status = pjsua_snd_get_setting(PJMEDIA_AUD_DEV_CAP_OUTPUT_VOLUME_SETTING, &vol);

	qDebug() << DEBUG_PREFIX << "Volume : " << vol;

	if (status != PJ_SUCCESS) {
//		setError(Service::InternalError);
		qWarning() << DEBUG_PREFIX << "Can't get call volume";
	}

	return vol;
}

void SipServicePrivate::setCallVolume(int callId, unsigned vol)
{
	Q_UNUSED(callId);
	m_webrtcMedia.SetSpeakerVolume(vol);
#if 0
	Q_ASSERT(vol>=0 && vol<=199);

	unsigned value = vol;
	pj_status_t status;
	status = pjsua_snd_set_setting(PJMEDIA_AUD_DEV_CAP_OUTPUT_VOLUME_SETTING, &value, false);
	if (status != PJ_SUCCESS) {
//		setError(Service::InternalError);
		qWarning() << DEBUG_PREFIX << "Can't set call volume to " << vol;
		return;
	}
#endif
}

void SipServicePrivate::callQuality(int callId, float* rxLossPercent, float* txLossPercent)
{
	*rxLossPercent = *txLossPercent = 0.0;

	pjsua_call_info ci;
	pjsua_call_get_info(callId, &ci);

	if (ci.state < PJSIP_INV_STATE_CONFIRMED) {
		qDebug() << DEBUG_PREFIX << "The call has not been confirmed.";
		return;
	}

	if (ci.media_cnt > 0) {

		pjsua_stream_stat stream_state;
		pjsua_call_get_stream_stat(callId, ci.media[0].index, &stream_state);

// 		qDebug() << "=========================RX Statictics====================================";
// 		qDebug() << stream_state.rtcp.rx.pkt;
// 		qDebug() << stream_state.rtcp.rx.bytes;
// 		qDebug() << "Bytes per packet: " << (stream_state.rtcp.rx.bytes/stream_state.rtcp.rx.pkt);
// 		qDebug() << stream_state.rtcp.rx.loss;
		*rxLossPercent = stream_state.rtcp.rx.loss*100.0/(stream_state.rtcp.rx.pkt+stream_state.rtcp.rx.loss);
// 		qWarning() << "RX Percent loss: " << *rxLossPercent;
// 		qDebug() << stream_state.rtcp.rx.dup;
// 		qDebug() << stream_state.rtcp.rx.reorder;
// 		qDebug() << stream_state.rtcp.rx.jitter.min << stream_state.rtcp.rx.jitter.max << stream_state.rtcp.rx.jitter.mean;
// 		qDebug() << "=========================TX Statictics====================================";
// 		qDebug() << stream_state.rtcp.tx.pkt;
// 		qDebug() << stream_state.rtcp.tx.bytes;
// 		qDebug() << "Bytes per packet: " << (stream_state.rtcp.tx.bytes/stream_state.rtcp.tx.pkt);
// 		qDebug() << stream_state.rtcp.tx.loss;
		*txLossPercent = stream_state.rtcp.tx.loss*100.0/(stream_state.rtcp.tx.pkt+stream_state.rtcp.tx.loss);
// 		qWarning() << "TX Percent loss: " << *txLossPercent;
// 		qDebug() << stream_state.rtcp.tx.dup;
// 		qDebug() << stream_state.rtcp.tx.reorder;
// 		qDebug() << stream_state.rtcp.tx.jitter.min << stream_state.rtcp.tx.jitter.max << stream_state.rtcp.tx.jitter.mean;
// 		qDebug() << "=====================End Statistics=======================================";
	}
}

int SipServicePrivate::buddySubscribe(const QString& buddyURI)
{
	SipServiceStr tempURI(buddyURI);

	pjsua_buddy_config cfg;
	pjsua_buddy_id buddyID = -1;
	pj_status_t status;

	pjsua_buddy_config_default(&cfg);
	cfg.uri = pj_str(tempURI.str());
	cfg.subscribe = true;

	status = pjsua_buddy_add(&cfg, &buddyID);
	if (status != PJ_SUCCESS) {
		setError(Service::InternalError);
		return -1;
	}

	return buddyID;
}

void SipServicePrivate::buddySubscribeCancel(int buddyID)
{
	if (buddyID < 0)
		return;

	pj_status_t status;

	status = pjsua_buddy_del(buddyID);
	if (status != PJ_SUCCESS) {
		setError(Service::InternalError);
		return;
	}
}

void SipServicePrivate::streamWave(const QString& fileName)
{
	m_streamWave = fileName;
}

void SipServicePrivate::destroyStream()
{
	if (m_playerID != -1) {
		pjsua_player_destroy(m_playerID);
		m_playerID = -1;
	}
}

void SipServicePrivate::recordCall(const QString& fileName)
{
	//All call will record to one file
	SipServiceStr tempFileName(fileName);

	pj_status_t status;
	status = pjsua_recorder_create(&pj_str(tempFileName.str()), 0, NULL, 0, 0, &m_recorderID);
	if (status != PJ_SUCCESS) {
		m_recorderID = -1;
		setError(Service::InternalError);
	}
}

void SipServicePrivate::destroyRecord()
{
	if (m_recorderID != -1) {
		pjsua_player_destroy(m_recorderID);
		m_recorderID = -1;
	}
}

void SipServicePrivate::setVideoWindow(void* local, void* remote)
{
	m_widgetLocal = local;
	m_widgetRemote = remote;
	//m_webrtcMedia.SetVideoRenderWin(local, remote);
}

void SipServicePrivate::setCodecSize(int width, int height)
{
	m_codecWidth = width;
	m_codecHeight = height;
}

void SipServicePrivate::setVideoIncoming(int callId, bool on)
{
	if (callId < 0) {
		qWarning() << DEBUG_PREFIX << "No call valid of call id : " << callId;
		return ;
	}
	QString enable;
	pj_status_t status;

	if(on == true) {
		enable = QLatin1String("uaVideoEnable");
	} else {
		enable = QLatin1String("uaVideoDisable");
	}

	SipServiceStr tempEnable(enable);
	status = pjsua_call_send_im(callId, NULL, &pj_str(tempEnable.str()), NULL, NULL);

	if(status != PJ_SUCCESS) {
		setError(Service::InternalError);
	}
}

void SipServicePrivate::setVideoOpenWin()
{
	m_webrtcMedia.SetVideoOpenWin(/*(HWND)m_widgetLocal,*/ (HWND)m_widgetRemote);
}

void SipServicePrivate::setVideoCloseWin()
{
	m_webrtcMedia.SetVideoCloseWin();
}

void SipServicePrivate::prepareVideoStream()
{
	m_webrtcMedia.PrepareVideoStream(m_videoLocalPort, m_port, m_addr.toAscii().data()/*, m_codecWidth, m_codecHeight, 300, 15*/);
}

void SipServicePrivate::stopPsdVideoStream()
{
	m_webrtcMedia.StopPsdVideoSend();
}

bool SipServicePrivate::capture(const QString& fileName)
{
	return true;
}

void SipServicePrivate::setVideoRotation(int angle)
{
	m_webrtcMedia.SetVidRotation(angle);
}

void SipServicePrivate::setVideoBrightness(int brightness)
{
	m_webrtcMedia.SetVidLuminance(brightness);
}

void SipServicePrivate::getRtpInfo(unsigned int& byte)
{
	m_webrtcMedia.GetVidRtpInfo(byte);
}

void SipServicePrivate::startup()
{
	Q_Q(SipService);
	Q_ASSERT(q);

}

void SipServicePrivate::cleanup()
{
	Q_Q(SipService);
	Q_ASSERT(q);

	setState(Service::StoppedState);

	destroyStream();
	destroyRecord();

    clearResult();

    AbstractServicePrivate::cleanup();
}

void SipServicePrivate::on_log(int level, const char *data, int len)
{
	PJ_UNUSED_ARG(len);

	//qWarning() << DEBUG_PREFIX << data;

	instance()->sipLog(QString::fromLatin1(data));
}

void SipServicePrivate::on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
	PJ_UNUSED_ARG(acc_id);

	qDebug() << DEBUG_PREFIX << "Register status changed " 
		<< info->cbparam->code
		<< info->cbparam->status;
/*		<< info->cbparam->reason.ptr;*/

	instance()->regState(acc_id, info->cbparam->code, QString::fromLatin1(info->cbparam->reason.ptr));
}

void SipServicePrivate::on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata)
{
	pjsua_call_info ci;

	PJ_UNUSED_ARG(acc_id);

	qDebug() << DEBUG_PREFIX << headerValue(rdata->msg_info.msg, SIPExtended::sourceLanguage) 
		<< headerValue(rdata->msg_info.msg, SIPExtended::targetLanguage) 
		<< headerValue(rdata->msg_info.msg, SIPExtended::callMode) 
		<< headerValue(rdata->msg_info.msg, SIPExtended::serviceMode);

	pjsua_call_get_info(call_id, &ci);

	qDebug() << DEBUG_PREFIX << "Incoming call from " << ci.remote_info.ptr << acc_id << call_id;

	QStringList callInContentList;
	callInContentList.append(QString::number(acc_id));
	callInContentList.append(QString::number(call_id));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::sourceLanguage));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::targetLanguage));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::callMode));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::serviceMode));
	callInContentList.append(QString::fromLatin1(ci.remote_info.ptr));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::calleeNumber));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::localPartner));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::localCountry));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::localLanguage));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::photoId));
	callInContentList.append(headerValue(rdata->msg_info.msg, SIPExtended::callRecordId));
	instance()->incomingCall(callInContentList);
}

void SipServicePrivate::on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
	PJ_UNUSED_ARG(e);

	instance()->callState(call_id, PJSIP_INV_STATE_NULL);
}

void SipServicePrivate::on_call_media_state(pjsua_call_id call_id)
{
	qDebug() << DEBUG_PREFIX << "Call media state ";

	instance()->callMediaState(call_id);
}

void SipServicePrivate::on_pager(pjsua_call_id call_id, 
								 const pj_str_t *from, 
								 const pj_str_t *to, 
								 const pj_str_t *contact, 
								 const pj_str_t *mime_type, 
								 const pj_str_t *body)
{
	PJ_UNUSED_ARG(to);
	PJ_UNUSED_ARG(contact);
	PJ_UNUSED_ARG(mime_type);
	
	qDebug() << DEBUG_PREFIX << "Receive text from " << call_id << from->ptr << "Content:" << body->ptr;

	instance()->receiveText(call_id, QString::fromLatin1(from->ptr), QString::fromLatin1(body->ptr));
}

void SipServicePrivate::on_dtmf_digit(pjsua_call_id call_id, int digit)
{
	qDebug() << DEBUG_PREFIX << "Receive dtmf from " << call_id << digit;

	instance()->receiveDtmf(call_id, digit);
}

void SipServicePrivate::on_buddy_state(pjsua_buddy_id buddy_id)
{
	instance()->buddyState(buddy_id);
}

pjmedia_transport* SipServicePrivate::on_create_media_transport(pjsua_call_id call_id, 
			unsigned media_idx, pjmedia_transport *base_tp, unsigned flags)
{
	/* UDP transport is created already, just overload the method here to stop rtcp */
	pjmedia_transport *tp = base_tp;
	tp->op->send_rtcp = &send_rtcp;
	tp->op->send_rtcp2 = &send_rtcp2;
	return tp;
}

pj_status_t SipServicePrivate::send_rtcp(pjmedia_transport *tp, const void *pkt, pj_size_t size)
{
	return PJ_TRUE ;
}

pj_status_t SipServicePrivate::send_rtcp2(pjmedia_transport *tp, const pj_sockaddr_t *addr, unsigned addr_len, const void *pkt, pj_size_t size)
{
	return PJ_TRUE;
}

void SipServicePrivate::on_call_media_socket_create(pjsua_call_id call_id, int rtp_port, int rtcp_port)
{
	instance()->prepareCall(rtp_port, rtcp_port);
}

void SipServicePrivate::on_call_sdp_created(pjsua_call_id call_id, 
			pjmedia_sdp_session *sdp, pj_pool_t *pool, const pjmedia_sdp_session *rem_sdp)
{
	int rtp_port = -1;
	int rtcp_port = -1;
	int rtp_port_video = -1;
	int rtcp_port_video = -1;
	pj_str_t codec = pj_str("107");

	for(int i = 0; i < sdp->media_count; i++) {
		pjmedia_sdp_media *media = sdp->media [i];
		if(media && 0 == pj_strcmp2(&media->desc.media, (const char *)"audio")) {
			rtp_port = media->desc.port;
			if(media->desc .fmt_count > 0) {
				codec = media->desc.fmt[0];
			}

			pjmedia_sdp_attr *attr = pjmedia_sdp_attr_find(media->attr_count , media->attr, &pj_str("rtcp"), NULL);
			if(attr) {
				pjmedia_sdp_rtcp_attr rtcp;
				pjmedia_sdp_attr_get_rtcp(attr, &rtcp);
				rtcp_port = rtcp.port;
			}
		}
		if(media && 0 == pj_strncmp2(&media->desc.media, (const char *)"video", 5)) {
			// find rtp port
			rtp_port_video = media->desc.port;
			if(media->desc.fmt_count > 0) {
				codec = media->desc.fmt[0];
			}

			// find rtcp port
			pjmedia_sdp_attr *attr = pjmedia_sdp_attr_find(media->attr_count , media->attr, &pj_str( "rtcp"), NULL);
			if(attr) {
				pjmedia_sdp_rtcp_attr rtcp;
				pjmedia_sdp_attr_get_rtcp(attr, &rtcp);
				rtcp_port_video = rtcp. port;
			}
		} 
	}

	instance()->callStreamRecv(rtp_port, rtp_port_video);
}

void SipServicePrivate::on_nat_detect(const pj_stun_nat_detect_result *res)
{
	if (res->status != PJ_SUCCESS) {
		qDebug() << DEBUG_PREFIX << "NAT detection failed, the NAT type is unknown";
	} else {
		qDebug() << DEBUG_PREFIX << "NAT detection success,  the NAT type is " << res->nat_type << res->nat_type_name;
	}
}

void SipServicePrivate::error_exit(const char *title, pj_status_t status)
{
	qDebug() << DEBUG_PREFIX << title << status;

	pjsua_perror(DEBUG_PREFIX, title, status);
	pjsua_destroy();

	instance()->errorExit();
}

void SipServicePrivate::clearResult()
{
}

QString SipServicePrivate::headerValue(const pjsip_msg* msg, const QString& header)
{
	QString ret;
	if (msg==NULL || header.isEmpty())
		return ret;
	SipServiceStr hd(header);
	pjsip_hdr* result = (pjsip_hdr*)pjsip_msg_find_hdr_by_name(msg, &pj_str(hd.str()), NULL);

	do {
		if (result == NULL)
			break;
		QString text = QString::fromLatin1(result->name.ptr);
		int index = text.indexOf(QLatin1String("\r\n"));
		if (index == -1)
			break;
		ret = text.left(index);
		if (ret.startsWith(header)) 
			ret = ret.mid(header.length()+2);
		else
			ret = QString();
	} while(false);

	return ret;
}

QString SipServicePrivate::errorText(int status)
{
	char errMsg[PJ_ERR_MSG_SIZE];
	pj_strerror(status, errMsg, sizeof(errMsg));
	return QString::fromLocal8Bit(errMsg);
}

void SipServicePrivate::sipLog(const QString& msg)
{
	Q_Q(SipService);
	Q_ASSERT(q);
	
	Q_EMIT q->sipLog(msg);
}

void SipServicePrivate::regState(int accountid, int code, const QString& reason)
{
	Q_Q(SipService);
	Q_ASSERT(q);
	Q_EMIT q->regState(accountid, code, reason);
}

void SipServicePrivate::incomingCall(const QStringList& callInList)
{
	Q_Q(SipService);
	Q_ASSERT(q);

	Q_EMIT q->incomingCallEx(callInList);
}

void SipServicePrivate::callState(int callId, int state)
{
	pjsua_call_info ci;

	pjsua_call_get_info(callId, &ci);

	Q_Q(SipService);
	Q_ASSERT(q);

	if(ci.state == PJSIP_INV_STATE_CONFIRMED)  {
		char* dest_audio_addr = NULL;
		int dest_audio_port = -1;

		char* dest_video_addr = NULL;
		int dest_video_port = -1;

		dest_audio_addr = ci.rem_audio_addr.ptr ;
		dest_audio_port = ci.rem_audio_port;

		dest_video_addr = ci.rem_video_addr.ptr;
		dest_video_port = ci.rem_video_port;

		m_addr = QString::fromLocal8Bit(dest_audio_addr);
		m_port = dest_video_port;

//		m_webrtcMedia.SetCameraOpen((HWND)m_widgetLocal);

		m_webrtcMedia.PrepareAudioStream(m_audioLocalPort, dest_audio_port, dest_audio_addr);

		m_webrtcMedia.PreparePsdVidepStream(m_videoLocalPort, dest_video_port, dest_video_addr);

		//m_webrtcMedia.PrepareVideoStream(m_videoLocalPort, dest_video_port, dest_audio_addr, m_codecWidth, m_codecHeight, 300, 15);

		//m_webrtcMedia.SetVideoOpenWin((HWND)m_widgetRemote);

	}	else if(ci.state == PJSIP_INV_STATE_DISCONNECTED) {
		m_webrtcMedia.StopPsdVideoSend();
		m_webrtcMedia.StopAudioStream();
		m_webrtcMedia.StopVideoStream();
		m_webrtcMedia.SetVideoCloseWin();
//		m_webrtcMedia.SetCameraClose();
		unregisterServer();
	}	  

	qDebug() << DEBUG_PREFIX << "Call " << callId << " state=" << ci.state << ci.state_text.ptr;

	Q_EMIT q->callState(callId, ci.state);
}

void SipServicePrivate::callMediaState(int callId)
{
	pjsua_call_info ci;

	pjsua_call_get_info(callId, &ci);

	qDebug() << DEBUG_PREFIX << "Media Status: " << ci.media_status
		<< "Media Count: " << ci.media_cnt 
		<< "Media Direction: " << ci.media_dir;

	if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
/*
		if (!m_streamWave.isEmpty()) {

			if (m_playerID != -1) {
				pjsua_player_destroy(m_playerID);
			}

			SipServiceStr tempFileName(m_streamWave);
			pj_status_t status;
			status = pjsua_player_create(&pj_str(tempFileName.str()), 0, &m_playerID);
			if (status != PJ_SUCCESS) {
				m_playerID = -1;
				setError(Service::InternalError);
				return;
			}

			// When media is active, stream wav file to remote peer
			pjsua_conf_connect(pjsua_player_get_conf_port(m_playerID), ci.conf_slot ); //stream wav file to remote peer
			pjsua_conf_connect(ci.conf_slot, 0); //play remote audio to local
		} else {
			// When media is active, connect call to sound device.
			// pjsua_conf_connect(ci.conf_slot, 0);
			// pjsua_conf_connect(0, ci.conf_slot);
		}

		if (m_recorderID != -1) {
			// When media is active, record remote audio to a wav file
			pjsua_conf_connect(ci.conf_slot, pjsua_recorder_get_conf_port(m_recorderID));
		}
*/
	}
}

void SipServicePrivate::receiveText(int callId, const QString& remote, const QString& text)
{
	Q_Q(SipService);
	Q_ASSERT(q);

	Q_EMIT q->receiveText(callId, remote, text);
}

void SipServicePrivate::receiveDtmf(int callId, int digit)
{
	Q_Q(SipService);
	Q_ASSERT(q);

	Q_EMIT q->receiveDtmf(callId, digit);
}

void SipServicePrivate::buddyState(int buddyID)
{
	Q_Q(SipService);
	Q_ASSERT(q);

	pjsua_buddy_info info;
	pjsua_buddy_get_info(buddyID, &info);

	Q_EMIT q->buddyStateChanged(buddyID, info.status);
}

void SipServicePrivate::callStreamRecv(int audioLocalPort, int videoLocalPort)
{
	m_audioLocalPort = audioLocalPort;
	m_videoLocalPort = videoLocalPort;
}

void SipServicePrivate::prepareCall(int rtp_port, int rtcp_port)
{
}

void SipServicePrivate::errorExit()
{
	Q_Q(SipService);
	Q_ASSERT(q);

	setError(Service::InternalError);
}


SipService::SipService(ServiceManager* manager, const QString& userAgent, QObject* parent) :
    AbstractService(*new SipServicePrivate(manager, QString()), parent)
{
    Q_D(SipService);
	d->m_userAgent = userAgent;
    d->init();
}

SipService::SipService(ServiceManager* manager, const QString& name, const QString& userAgent, QObject* parent) :
    AbstractService(*new SipServicePrivate(manager, name), parent)
{
    Q_D(SipService);
	d->m_userAgent = userAgent;
    d->init();
}

SipService::SipService(SipServicePrivate& dd, QObject* parent) :
    AbstractService(dd, parent)
{
    Q_D(SipService);
    d->init();
}

SipService::~SipService()
{
}

UserService* SipService::userService() const
{
	Q_D(const SipService);
	return d->m_userService;
}

void SipService::setUserService(UserService* userService)
{
	Q_D(SipService);

	if (d->m_userService == userService)
		return;

	if (!isStopped()) {
		stop();
	}

	if (d->m_userService) {
		disconnect(d->m_userService, SIGNAL(active()), this, SLOT(start()));
		disconnect(d->m_userService, SIGNAL(stopped()), this, SLOT(stop()));
	}

	d->m_userService = userService;

	if (d->m_userService) {
		connect(d->m_userService, SIGNAL(active()), this, SLOT(start()));
		connect(d->m_userService, SIGNAL(stopped()), this, SLOT(stop()));
	}

	if (d->m_userService && d->m_userService->isActive()) {
		start();
	}
}

int SipService::registerServer(const QString& account, const QString& secret, const QString& domain)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "registerServer rejected because of invalid state";
		return -1;
	}

	return d->registerServer(account, secret, domain);
}

void SipService::unregisterServer()
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "unregisterServer rejected because of invalid state";
		return;
	}

	d->unregisterServer();
}

void SipService::unregisterServer(int accountId)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "unregisterServer rejected because of invalid state";
		return;
	}

	d->unregisterServer(accountId);
}

int SipService::maxAccountCount() const
{
	Q_D(const SipService);
	return d->maxAccountCount();
}

int SipService::makeCall(int accountid, const QString& dstUri, const QString& srcLang, 
			const QString& tarLang, const QString& service, const QString& country)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "answerCall rejected because of invalid state";
		return -1;
	}

	return d->makeCall(accountid, dstUri, srcLang, tarLang, service, country);
}

void SipService::ringringCall(int callId)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "ringringCall rejected because of invalid state";
		return;
	}

	d->ringringCall(callId);
}

void SipService::answerCall(int callId)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "answerCall rejected because of invalid state";
		return;
	}

	d->answerCall(callId);
}

void SipService::rejectCall(int callId)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "rejectCall rejected because of invalid state";
		return;
	}

	d->rejectCall(callId);
}

void SipService::sendDtmf(int callId, const QChar& dtmf)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "rejectCall rejected because of invalid state";
		return;
	}

	d->sendDtmf(callId, dtmf);
}

void SipService::hangupCall(int callId)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "hangupCall rejected because of invalid state";
		return;
	}

	d->hangupCall(callId);
}

void SipService::muteCall(int callId, bool mute)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "muteCall rejected because of invalid state";
		return;
	}

	d->muteCall(callId, mute);
}

void SipService::setCallVolume(int callId, unsigned level)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "setCallVolume rejected because of invalid state";
		return;
	}

	d->setCallVolume(callId, level);
}

int SipService::callVolume(int callId)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "callVolume rejected because of invalid state";
		return -1;
	}

	return d->callVolume(callId);
}

void SipService::callQuality(int callId, float* rxLossPercent, float* txLossPercent)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "callQuality rejected because of invalid state";
		return;
	}

	d->callQuality(callId, rxLossPercent, txLossPercent);
}

int SipService::buddySubscribe(const QString& buddyURI)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "buddySubscribe rejected because of invalid state";
		return -1;
	}

	return d->buddySubscribe(buddyURI);
}

void SipService::buddySubscribeCancel(int buddyID)
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "buddySubscribeCancel rejected because of invalid state";
		return;
	}

	d->buddySubscribeCancel(buddyID);
}

void SipService::streamWave(const QString& fileName)
{
	Q_D(SipService);

// 	if (!isActive()) {
// 		qDebug() << DEBUG_PREFIX << "streamWave rejected because of invalid state";
// 		return;
// 	}

	d->streamWave(fileName);
}

void SipService::destroyStream()
{
	Q_D(SipService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "hangupCall rejected because of invalid state";
		return;
	}

	d->destroyStream();
}

void SipService::setVideoWindow(void* local, void* remote)
{
	Q_D(SipService);

	d->setVideoWindow(local, remote);
}

void SipService::setCodecSize(int width, int height)
{
	Q_D(SipService);

	d->setCodecSize(width, height);
}

void SipService::setVideoIncoming(int callId, bool on)
{
	Q_D(SipService);

	d->setVideoIncoming(callId, on);
}

void SipService::setVideoOpenWin()
{
	Q_D(SipService);

	d->setVideoOpenWin();
}

void SipService::setVideoCloseWin()
{
	Q_D(SipService);

	d->setVideoCloseWin();
}

void SipService::prepareVideoStream()
{
	Q_D(SipService);

	d->prepareVideoStream();
}

void SipService::stopPsdVideoStream()
{
	Q_D(SipService);

	d->stopPsdVideoStream();
}

bool SipService::capture(const QString& fileName)
{
	Q_D(SipService);

	return d->capture(fileName);
}

void SipService::setVideoRotation(int angle)
{
	Q_D(SipService);
	
	d->setVideoRotation(angle);
}

void SipService::setVideoBrightness(int brightness)
{
	Q_D(SipService);

	d->setVideoBrightness(brightness);
}

void SipService::getRtpInfo(unsigned int& byte)
{
	Q_D(SipService);

	d->getRtpInfo(byte);
}

void SipService::applyPreference(const QSet<QString>& paths, bool force)
{
    Q_D(SipService);

//     typedef SipServiceOptions Opt;
//     Opt& opt = d->m_opt;
    Preference* pref = preference();
}

} // namespace ns

#include "moc_sipservice.cpp"
