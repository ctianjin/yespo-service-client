#ifndef SIPSERVICE_H
#define SIPSERVICE_H

#include "abstractservice.h"
#include "serviceglobal.h"
//#include "sipcommand.h"

namespace ns {

class UserService;
class SipServicePrivate;

class BUILDSYS_SERVICE_DECL SipService : public AbstractService
{
    Q_OBJECT
    Q_PROPERTY(UserService* userService READ userService WRITE setUserService)

public:
	enum CallinContent {
		AcconutID = 0,
		CallID,
		SourceLanguage,
		TargetLanguage,
		CallMode,
		ServiceMode,
		RemoteContect,
		CalleeNumber,
		LocalPartner,
		LocalCountry,
		LocalLanguage,
		CallPhotoes,
		CallRecordId,
		Max = 13
	};

public:
    explicit SipService(ServiceManager* manager, const QString& userAgent, QObject* parent = 0);
    SipService(ServiceManager* manager, const QString& name, const QString& userAgent, QObject* parent = 0);
    virtual ~SipService();

	UserService* userService() const;
	void setUserService(UserService* userService);

	int registerServer(const QString& account, const QString& secret, const QString& domain);
	void unregisterServer();
	void unregisterServer(int accountId);

	int maxAccountCount() const;

	int makeCall(int accountid, const QString& dstUri, const QString& srcLang, 
		const QString& tarLang, const QString& service, const QString& country);
	void ringringCall(int callId);
	void answerCall(int callId);
	void rejectCall(int callId);
	void sendDtmf(int callId, const QChar& dtmf);
	void hangupCall(int callId);
	void muteCall(int callId, bool mute);
	void setCallVolume(int callId, unsigned level);
	int callVolume(int callId);
	void callQuality(int callId, float* rxLossPercent, float* txLossPercent);
	void sendVideo(void* remote, void* local, int bitRate, int frameRate);
	void stopVideo();

	int buddySubscribe(const QString& buddyURI);
	void buddySubscribeCancel(int buddyID);

	void streamWave(const QString& fileName);
	void destroyStream();

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

public Q_SLOTS:

Q_SIGNALS:
    void sipLog(const QString& msg);
	void regState(int accountid, int code, const QString& reason);
	void incomingCall(int accountId, int callId, const QString& srcLang, const QString& targetLang, const QString& callMode, const QString& serviceMode, const QString& remote, const QString& calleeNumber);
	void callState(int callId, int state);
	void receiveText(int callId, const QString& remote, const QString& text);
	void receiveDtmf(int callId, int digit);
	void buddyStateChanged(int buddyID, int state);

	//void incomingCallEx(const SIP::CallInParam& param);
	void incomingCallEx(const QStringList& callInList);

protected:
    SipService(SipServicePrivate& dd, QObject* parent = 0);

	virtual void applyPreference(const QSet<QString>& paths, bool force);

private:

    Q_DECLARE_PRIVATE(SipService)
    Q_DISABLE_COPY(SipService)
};

} // namespace ns

#endif // SIPSERVICE_H
