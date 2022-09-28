#ifndef COMMAND_H
#define COMMAND_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QByteArray>
#include "service.h"
#include "serviceglobal.h"

QT_BEGIN_NAMESPACE
class QString;
class QStringList;
class QUrl;
QT_END_NAMESPACE

namespace ns {
namespace Command {

// CommonResult
class CommonResultPrivate;

class BUILDSYS_SERVICE_DECL CommonResult
{
public:
	CommonResult();
	CommonResult(const CommonResult& rhs);
	~CommonResult();

	CommonResult& operator=(const CommonResult& rhs);

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	QString errorNum() const;
	void setErrorNum(const QString& errNum);

	QString errorTips() const;
	void setErrorTips(const QString& tips);

private:
	QSharedDataPointer<CommonResultPrivate> d;
};

// LoadConfigParam
class LoadConfigParamPrivate;

class BUILDSYS_SERVICE_DECL LoadConfigParam
{
public:
	LoadConfigParam();
	LoadConfigParam(const LoadConfigParam& rhs);
	~LoadConfigParam();

	LoadConfigParam& operator=(const LoadConfigParam& rhs);

	QString lastTime() const;
	void setLastTime(const QString& lastTime);

	int reloadConfig() const;
	void setReloadConfig(int reload);

	bool operator==(const LoadConfigParam& rhs) const;
	bool operator!=(const LoadConfigParam& rhs) const;

private:
	QSharedDataPointer<LoadConfigParamPrivate> d;
};

// LoadConfigResult
class LoadConfigResultPrivate;

class BUILDSYS_SERVICE_DECL LoadConfigResult
{
public:
	LoadConfigResult();
	LoadConfigResult(const LoadConfigResult& rhs);
	~LoadConfigResult();

	LoadConfigResult& operator=(const LoadConfigResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QVariant config() const;
	void setConfig(const QVariant& result);

	QVariant client() const;
	void setClient(const QVariant& result);

private:
	QSharedDataPointer<LoadConfigResultPrivate> d;
};

// LoginParam
class LoginParamPrivate;

class BUILDSYS_SERVICE_DECL LoginParam
{
public:
    LoginParam();
    LoginParam(const LoginParam& rhs);
    ~LoginParam();

    LoginParam& operator=(const LoginParam& rhs);

    QString userName() const;
    void setUserName(const QString& userName);

    QString password() const;
    void setPassword(const QString& password);

    bool operator==(const LoginParam& rhs) const;
    bool operator!=(const LoginParam& rhs) const;

private:
    QSharedDataPointer<LoginParamPrivate> d;
};

// LoginResult
class LoginResultPrivate;

class BUILDSYS_SERVICE_DECL LoginResult
{
public:
    LoginResult();
    LoginResult(const LoginResult& rhs);
    ~LoginResult();

    LoginResult& operator=(const LoginResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString systemId() const;
	void setSystemId(const QString& Id);

	QString callerId() const;
	void setCallerId(const QString& Id);

 	QString loginId() const;
 	void setLoginId(const QString& Id);

	QString secret() const;
	void setSecret(const QString& secret);

	QString displayName() const;
	void setDisplayName(const QString& name);

	QStringList skill() const;
	void setSkill(const QStringList& skill);

	QStringList local() const;
	void setLocal(const QStringList& local);

	QStringList sipServerList() const;
	void setSipServerList(const QStringList& list);

	QString statusServiceHost() const;
	void setStatusServiceHost(const QString& statusServiceHost);

	QString sipServiceHost() const;
	void setSipServiceHost(const QString& sipServiceHost);

	QString imServiceHost() const;
	void setImServiceHost(const QString& imServiceHost);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString largePhoto() const;
	void setLargePhoto(const QString& photo);

	QString token() const;
	void setToken(const QString& token);

private:
    QSharedDataPointer<LoginResultPrivate> d;
};

//setclientinfo
class SetClientInfoParamPrivate;

class BUILDSYS_SERVICE_DECL SetClientInfoParam
{
public:
	SetClientInfoParam();
	SetClientInfoParam(const SetClientInfoParam& rhs);
	~SetClientInfoParam();

	SetClientInfoParam& operator=(const SetClientInfoParam& rhs);

	QString lastDomainCall() const;
	void setLastDomainCall(const QString& domainCall);

	bool operator==(const SetClientInfoParam& rhs) const;
	bool operator!=(const SetClientInfoParam& rhs) const;

private:
	QSharedDataPointer<SetClientInfoParamPrivate> d;
};

//setclientinfoResult
class SetClientInfoResultPrivate;

class BUILDSYS_SERVICE_DECL SetClientInfoResult
{
public:
	SetClientInfoResult();
	SetClientInfoResult(const SetClientInfoResult& rhs);
	~SetClientInfoResult();

	SetClientInfoResult& operator=(const SetClientInfoResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

private:
	QSharedDataPointer<SetClientInfoResultPrivate> d;
};

//server
class GetServerParamPrivate;

class BUILDSYS_SERVICE_DECL GetServerParam
{
public:
	GetServerParam();
	GetServerParam(const GetServerParam& rhs);
	~GetServerParam();

	GetServerParam& operator=(const GetServerParam& rhs);

	bool operator==(const GetServerParam& rhs) const;
	bool operator!=(const GetServerParam& rhs) const;

private:
	QSharedDataPointer<GetServerParamPrivate> d;
};

//serverResult
class GetServerResultPrivate;

class BUILDSYS_SERVICE_DECL GetServerResult
{
public:
	GetServerResult();
	GetServerResult(const GetServerResult& rhs);
	~GetServerResult();

	GetServerResult& operator=(const GetServerResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QStringList sipServerList() const;
	void setSipServerList(const QStringList& list);

	QString domainStatus() const;
	void setDomainStatus(const QString& status);

	QString domainCall() const;
	void setDomainCall(const QString& call);

	QString domainIM() const;
	void setDomainIM(const QString& IM);

private:
	QSharedDataPointer<GetServerResultPrivate> d;
};

//TodayStat
class TodayStatParamPrivate;

class BUILDSYS_SERVICE_DECL TodayStatParam
{
public:
	TodayStatParam();
	TodayStatParam(const TodayStatParam& rhs);
	~TodayStatParam();

	TodayStatParam& operator=(const TodayStatParam& rhs);

	bool operator==(const TodayStatParam& rhs) const;
	bool operator!=(const TodayStatParam& rhs) const;

private:
	QSharedDataPointer<TodayStatParamPrivate> d;
};

//TodayStatResult
class TodayStatResultPrivate;

class BUILDSYS_SERVICE_DECL TodayStatResult
{
public:
	TodayStatResult();
	TodayStatResult(const TodayStatResult& rhs);
	~TodayStatResult();

	TodayStatResult& operator=(const TodayStatResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString success() const;
	void setSuccess(const QString& value);

	QString answer() const;
	void setAnswer(const QString& value);

	QString noanswer() const;
	void setNoAnswer(const QString& value);

private:
	QSharedDataPointer<TodayStatResultPrivate> d;
};

//GetNoReportParam
class GetNoReportParamPrivate;

class BUILDSYS_SERVICE_DECL GetNoReportParam
{
public:
	GetNoReportParam();
	GetNoReportParam(const GetNoReportParam& rhs);
	~GetNoReportParam();

	GetNoReportParam& operator=(const GetNoReportParam& rhs);

	bool operator==(const GetNoReportParam& rhs) const;
	bool operator!=(const GetNoReportParam& rhs) const;

private:
	QSharedDataPointer<GetNoReportParamPrivate> d;
};

//GetNoReportResult
class GetNoReportResultPrivate;

class BUILDSYS_SERVICE_DECL GetNoReportResult
{
public:
	GetNoReportResult();
	GetNoReportResult(const GetNoReportResult& rhs);
	~GetNoReportResult();

	GetNoReportResult& operator=(const GetNoReportResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString num() const;
	void setNum(const QString& num);

private:
	QSharedDataPointer<GetNoReportResultPrivate> d;
};


//GetCDRList
class GetCDRListParamPrivate;

class BUILDSYS_SERVICE_DECL GetCDRListParam
{
public:
	GetCDRListParam();
	GetCDRListParam(const GetCDRListParam& rhs);
	~GetCDRListParam();

	GetCDRListParam& operator=(const GetCDRListParam& rhs);

	QString Id() const;
	void setId(const QString& Id);

	QString type() const;
	void setType(const QString& type);

	bool operator==(const GetCDRListParam& rhs) const;
	bool operator!=(const GetCDRListParam& rhs) const;

private:
	QSharedDataPointer<GetCDRListParamPrivate> d;
};

// GetCDRListResult
class GetCDRListResultPrivate;

class BUILDSYS_SERVICE_DECL GetCDRListResult
{
public:
	GetCDRListResult();
	GetCDRListResult(const GetCDRListResult& rhs);
	~GetCDRListResult();

	GetCDRListResult& operator=(const GetCDRListResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString Id() const;
	void setId(const QString& Id);

	QString VESysId() const;
	void setVESysId(const QString& Id);

	QString meetCDRId() const;
	void setMeetCDRId(const QString& Id);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString displayName() const;
	void setDisplayName(const QString& name);

	QString startCallTime() const;
	void setStartCallTime(const QString& callTime);

	QString callTime() const;
	void setCallTime(const QString& callTime);

	QString isAnswered() const;
	void setIsAnswered(const QString& answered);

	QString callMode() const;
	void setCallMode(const QString& mode);

	QString isSuccess() const;
	void setIsSuccess(const QString& success);

private:
	QSharedDataPointer<GetCDRListResultPrivate> d;
};

//GetCDRInfo
class GetCDRInfoParamPrivate;

class BUILDSYS_SERVICE_DECL GetCDRInfoParam
{
public:
	GetCDRInfoParam();
	GetCDRInfoParam(const GetCDRInfoParam& rhs);
	~GetCDRInfoParam();

	GetCDRInfoParam& operator=(const GetCDRInfoParam& rhs);

	QString meetCDRId() const;
	void setMeetCDRId(const QString& Id);

	bool operator==(const GetCDRInfoParam& rhs) const;
	bool operator!=(const GetCDRInfoParam& rhs) const;

private:
	QSharedDataPointer<GetCDRInfoParamPrivate> d;
};

// GetCDRInfoResult
class GetCDRInfoResultPrivate;

class BUILDSYS_SERVICE_DECL GetCDRInfoResult
{
public:
	GetCDRInfoResult();
	GetCDRInfoResult(const GetCDRInfoResult& rhs);
	~GetCDRInfoResult();

	GetCDRInfoResult& operator=(const GetCDRInfoResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString CDRType() const;
	void setCDRType(const QString& type);

	QString callerVESysId() const;
	void setCallerVESysId(const QString& Id);

	QString callerId() const;
	void setCallerId(const QString& Id);

	QString callerVELoginId() const;
	void setCallerVELoginId(const QString& Id);

	QString callerDisplayName() const;
	void setCallerDisplayName(const QString& name);

	QString callerMobile() const;
	void setCallerMobile(const QString& mobile);

	QString callerSmallPhoto() const;
	void setCallerSmallPhoto(const QString& photo);

	QString callerLargePhoto() const;
	void setCallerLargePhoto(const QString& photo);

	QString isPSTNCall() const;
	void setIsPSTNCall(const QString& PSTNCall);

	QString callCountry() const;
	void setCallCountry(const QString& country);

	QString calleeVESysId() const;
	void setCalleeVESysId(const QString& Id);

	QString calleeId() const;
	void setCalleeId(const QString& Id);

	QString calleeVELoginId() const;
	void setCalleeVELoginId(const QString& Id);

	QString calleeDisplayName() const;
	void setCalleeDisplayName(const QString& name);

	QString calleeMobile() const;
	void setCalleeMobile(const QString& mobile);

	QString calleeSmallPhoto() const;
	void setCalleeSmallPhoto(const QString& photo);

	QString calleeLargePhoto() const;
	void setCalleeLargePhoto(const QString& photo);

	QString partnerVESysId() const;
	void setPartnerVESysId(const QString& Id);

	QString partnerId() const;
	void setPartnerId(const QString& Id);

	QString partnerLoginId() const;
	void setPartnerLoginId(const QString& Id);

	QString partnerDisplayName() const;
	void setPartnerDisplayName(const QString& name);

	QString partnerSmallPhoto() const;
	void setPartnerSmallPhoto(const QString& photo);

	QString partnerLargePhoto() const;
	void setPartnerLargePhoto(const QString& photo);

	QString isFree() const;
	void setIsFree(const QString& free);

	QString partnerLevel() const;
	void setPartnerLevel(const QString& level);

	QString language1() const;
	void setLanguage1(const QString& lang);

	QString language2() const;
	void setLanguage2(const QString& lang);

	QString callTime() const;
	void setCallTime(const QString& time);

	QString startCallTime() const;
	void setStartCallTime(const QString& time);

private:
	QSharedDataPointer<GetCDRInfoResultPrivate> d;
};

// GetUserInfoParam
class GetUserInfoParamPrivate;

class BUILDSYS_SERVICE_DECL GetUserInfoParam
{
public:
	GetUserInfoParam();
	GetUserInfoParam(const GetUserInfoParam& rhs);
	~GetUserInfoParam();

	GetUserInfoParam& operator=(const GetUserInfoParam& rhs);

	bool operator==(const GetUserInfoParam& rhs) const;
	bool operator!=(const GetUserInfoParam& rhs) const;

private:
	QSharedDataPointer<GetUserInfoParamPrivate> d;
};

// GetUserInfoResult
class GetUserInfoResultPrivate;

class BUILDSYS_SERVICE_DECL GetUserInfoResult
{
public:
	GetUserInfoResult();
	GetUserInfoResult(const GetUserInfoResult& rhs);
	~GetUserInfoResult();

	GetUserInfoResult& operator=(const GetUserInfoResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString systemId() const;
	void setSystemId(const QString& Id);

	QString callerId() const;
	void setCallerId(const QString& Id);

	QString loginId() const;
	void setLoginId(const QString& Id);

	QString secret() const;
	void setSecret(const QString& secret);

	QString displayName() const;
	void setDisplayName(const QString& name);

	QString gender() const;
	void setGender(const QString& gender);

	QString birthday() const;
	void setBirthday(const QString& dateTime);

	QString nationality() const;
	void setNationality(const QString& nation);

	QString education() const;
	void setEducation(const QString& edu);

	QString major() const;
	void setMajor(const QString& name);

	QString introduction() const;
	void setIntroduction(const QString& name);

	QStringList skill() const;
	void setSkill(const QStringList& skill);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString largePhoto() const;
	void setLargePhoto(const QString& photo);

private:
	QSharedDataPointer<GetUserInfoResultPrivate> d;
};

// GetCallerUserParam
class GetCallerUserParamPrivate;

class BUILDSYS_SERVICE_DECL GetCallerUserParam
{
public:
	GetCallerUserParam();
	GetCallerUserParam(const GetCallerUserParam& rhs);
	~GetCallerUserParam();

	GetCallerUserParam& operator=(const GetCallerUserParam& rhs);

	QString callerId() const;
	void setCallerId(const QString& Id);

	bool operator==(const GetCallerUserParam& rhs) const;
	bool operator!=(const GetCallerUserParam& rhs) const;

private:
	QSharedDataPointer<GetCallerUserParamPrivate> d;
};

// GetCallerUserResult
class GetCallerUserResultPrivate;

class BUILDSYS_SERVICE_DECL GetCallerUserResult
{
public:
	GetCallerUserResult();
	GetCallerUserResult(const GetCallerUserResult& rhs);
	~GetCallerUserResult();

	GetCallerUserResult& operator=(const GetCallerUserResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString largePhoto() const;
	void setLargePhoto(const QString& photo);

	QString displayName() const;
	void setDisplayName(const QString& name);

	QString mobile() const;
	void setMobile(const QString& mobile);

private:
	QSharedDataPointer<GetCallerUserResultPrivate> d;
};

// SaveScreenShotParam
class SaveScreenShotParamPrivate;

class BUILDSYS_SERVICE_DECL SaveScreenShotParam
{
public:
	SaveScreenShotParam();
	SaveScreenShotParam(const SaveScreenShotParam& rhs);
	~SaveScreenShotParam();

	SaveScreenShotParam& operator=(const SaveScreenShotParam& rhs);

	QString meetCDRId() const;
	void setMeetCDRId(const QString& Id);

	QString photo() const;
	void setPhoto(const QString& photo);

	bool operator==(const SaveScreenShotParam& rhs) const;
	bool operator!=(const SaveScreenShotParam& rhs) const;

private:
	QSharedDataPointer<SaveScreenShotParamPrivate> d;
};

// SaveScreenShotResult
class SaveScreenShotResultPrivate;

class BUILDSYS_SERVICE_DECL SaveScreenShotResult
{
public:
	SaveScreenShotResult();
	SaveScreenShotResult(const SaveScreenShotResult& rhs);
	~SaveScreenShotResult();

	SaveScreenShotResult& operator=(const SaveScreenShotResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

private:
	QSharedDataPointer<SaveScreenShotResultPrivate> d;
};

// ForgetPassword
class ForgetPasswordParamPrivate;

class BUILDSYS_SERVICE_DECL ForgetPasswordParam
{
public:
	ForgetPasswordParam();
	ForgetPasswordParam(const ForgetPasswordParam& rhs);
	~ForgetPasswordParam();

	ForgetPasswordParam& operator=(const ForgetPasswordParam& rhs);

	QString userName() const;
	void setUserName(const QString& userName);

	bool operator==(const ForgetPasswordParam& rhs) const;
	bool operator!=(const ForgetPasswordParam& rhs) const;

private:
	QSharedDataPointer<ForgetPasswordParamPrivate> d;
};

// CreateAccount
class CreateAccountParamPrivate;

class BUILDSYS_SERVICE_DECL CreateAccountParam
{
public:
	CreateAccountParam();
	CreateAccountParam(const CreateAccountParam& rhs);
	~CreateAccountParam();

	CreateAccountParam& operator=(const CreateAccountParam& rhs);

	bool operator==(const CreateAccountParam& rhs) const;
	bool operator!=(const CreateAccountParam& rhs) const;

private:
	QSharedDataPointer<CreateAccountParamPrivate> d;
};

// ChangeStatusParam
class ChangeStatusParamPrivate;

class BUILDSYS_SERVICE_DECL ChangeStatusParam
{
public:
	ChangeStatusParam();
	ChangeStatusParam(const ChangeStatusParam& rhs);
	~ChangeStatusParam();

	ChangeStatusParam& operator=(const ChangeStatusParam& rhs);

	QString onlineVisibleStatus() const;
	void setOnlineVisibleStatus(const QString& status);

	bool operator==(const ChangeStatusParam& rhs) const;
	bool operator!=(const ChangeStatusParam& rhs) const;

private:
	QSharedDataPointer<ChangeStatusParamPrivate> d;
};

//ChangeStatusResult
class ChangeStatusResultPrivate;

class BUILDSYS_SERVICE_DECL ChangeStatusResult
{
public:
	ChangeStatusResult();
	ChangeStatusResult(const ChangeStatusResult& rhs);
	~ChangeStatusResult();

	ChangeStatusResult& operator=(const ChangeStatusResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

private:
	QSharedDataPointer<ChangeStatusResultPrivate> d;
};

// GetBlacklistParam
class GetBlacklistParamPrivate;

class BUILDSYS_SERVICE_DECL GetBlacklistParam
{
public:
	GetBlacklistParam();
	GetBlacklistParam(const GetBlacklistParam& rhs);
	~GetBlacklistParam();

	GetBlacklistParam& operator=(const GetBlacklistParam& rhs);

	QString id() const;
	void setId(const QString& id);

	bool operator==(const GetBlacklistParam& rhs) const;
	bool operator!=(const GetBlacklistParam& rhs) const;

private:
	QSharedDataPointer<GetBlacklistParamPrivate> d;
};

// GetBlacklistResult
class GetBlacklistResultPrivate;

class BUILDSYS_SERVICE_DECL GetBlacklistResult
{
public:
	GetBlacklistResult();
	GetBlacklistResult(const GetBlacklistResult& rhs);
	~GetBlacklistResult();

	GetBlacklistResult& operator=(const GetBlacklistResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString id() const;
	void setId(const QString& id);

	QString blacklistId() const;
	void setBlacklistId(const QString& id);

	QString parterSysId() const;
	void setPartnerSysId(const QString& id);

	QString VESysId() const;
	void setVESysId(const QString& id);

	QString displayName() const;
	void setDisplayName(const QString& name);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

private:
	QSharedDataPointer<GetBlacklistResultPrivate> d;
};

// AddBlacklistParam
class AddBlacklistParamPrivate;

class BUILDSYS_SERVICE_DECL AddBlacklistParam
{
public:
	AddBlacklistParam();
	AddBlacklistParam(const AddBlacklistParam& rhs);
	~AddBlacklistParam();

	AddBlacklistParam& operator=(const AddBlacklistParam& rhs);

	QString VESysId() const;
	void setVESysId(const QString& id);

	bool operator==(const AddBlacklistParam& rhs) const;
	bool operator!=(const AddBlacklistParam& rhs) const;

private:
	QSharedDataPointer<AddBlacklistParamPrivate> d;
};

// AddBlacklistResult
class AddBlacklistResultPrivate;

class BUILDSYS_SERVICE_DECL AddBlacklistResult
{
public:
	AddBlacklistResult();
	AddBlacklistResult(const AddBlacklistResult& rhs);
	~AddBlacklistResult();

	AddBlacklistResult& operator=(const AddBlacklistResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString id() const;
	void setId(const QString& id);

	QString blacklistId() const;
	void setBlacklistId(const QString& id);

	QString parterSysId() const;
	void setPartnerSysId(const QString& id);

	QString VESysId() const;
	void setVESysId(const QString& id);

	QString displayName() const;
	void setDisplayName(const QString& name);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

private:
	QSharedDataPointer<AddBlacklistResultPrivate> d;
};

// DelBlacklistParam
class DelBlacklistParamPrivate;

class BUILDSYS_SERVICE_DECL DelBlacklistParam
{
public:
	DelBlacklistParam();
	DelBlacklistParam(const DelBlacklistParam& rhs);
	~DelBlacklistParam();

	DelBlacklistParam& operator=(const DelBlacklistParam& rhs);

	QString VESysId() const;
	void setVESysId(const QString& id);

	bool operator==(const DelBlacklistParam& rhs) const;
	bool operator!=(const DelBlacklistParam& rhs) const;

private:
	QSharedDataPointer<DelBlacklistParamPrivate> d;
};

// DelBlacklistResult
class DelBlacklistResultPrivate;

class BUILDSYS_SERVICE_DECL DelBlacklistResult
{
public:
	DelBlacklistResult();
	DelBlacklistResult(const DelBlacklistResult& rhs);
	~DelBlacklistResult();

	DelBlacklistResult& operator=(const DelBlacklistResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

private:
	QSharedDataPointer<DelBlacklistResultPrivate> d;
};

// GetMemberInfoParam
class GetMemberInfoParamPrivate;

class BUILDSYS_SERVICE_DECL GetMemberInfoParam
{
public:
	GetMemberInfoParam();
	GetMemberInfoParam(const GetMemberInfoParam& rhs);
	~GetMemberInfoParam();

	GetMemberInfoParam& operator=(const GetMemberInfoParam& rhs);

	QString VESysId() const;
	void setVESysId(const QString& id);

	bool operator==(const GetMemberInfoParam& rhs) const;
	bool operator!=(const GetMemberInfoParam& rhs) const;

private:
	QSharedDataPointer<GetMemberInfoParamPrivate> d;
};

// GetMemberInfoResult
class GetMemberInfoResultPrivate;

class BUILDSYS_SERVICE_DECL GetMemberInfoResult
{
public:
	GetMemberInfoResult();
	GetMemberInfoResult(const GetMemberInfoResult& rhs);
	~GetMemberInfoResult();

	GetMemberInfoResult& operator=(const GetMemberInfoResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString VESysId() const;
	void setVESysId(const QString& id);

	QString VELoginId() const;
	void setVELoginId(const QString& id);

	QString callerId() const;
	void setCallerId(const QString& id);

	QString displayName() const;
	void setDisplayName(const QString& name);

	QString nativeLanguage() const;
	void setNativeLanguage(const QString& lang);

	QString country() const;
	void setCountry(const QString& country);

	QString major() const;
	void setMajor(const QString& major);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

private:
	QSharedDataPointer<GetMemberInfoResultPrivate> d;
};

//GetUploadPhotoParam
class GetUploadPhotoParamPrivate;

class Q_DECL_EXPORT GetUploadPhotoParam
{
public:
	GetUploadPhotoParam();
	GetUploadPhotoParam(const GetUploadPhotoParam& rhs);
	~GetUploadPhotoParam();

	GetUploadPhotoParam& operator=(const GetUploadPhotoParam& rhs);

	QString callerId() const;
	void setCallerId(const QString& id);

	QString photoId() const;
	void setPhotoId(const QString& id);

	bool operator==(const GetUploadPhotoParam& rhs) const;
	bool operator!=(const GetUploadPhotoParam& rhs) const;

private:
	QSharedDataPointer<GetUploadPhotoParamPrivate> d;
};

// GetUploadPhotoResult
class GetUploadPhotoResultPrivate;

class BUILDSYS_SERVICE_DECL GetUploadPhotoResult
{
public:
	GetUploadPhotoResult();
	GetUploadPhotoResult(const GetUploadPhotoResult& rhs);
	~GetUploadPhotoResult();

	GetUploadPhotoResult& operator=(const GetUploadPhotoResult& rhs);

	CommonResult common() const;
	void setCommon(const CommonResult& result);

	QString expire() const;
	void setExpire(const QString& expire);

	QString callerId() const;
	void setCallerId(const QString& id);

	QString photoId() const;
	void setPhotoId(const QString& id);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString largePhoto() const;
	void setLargePhoto(const QString& photo);


private:
	QSharedDataPointer<GetUploadPhotoResultPrivate> d;
};

//VersionParam
class VersionParamPrivate;

class BUILDSYS_SERVICE_DECL  VersionParam
{
public:
	VersionParam();
	VersionParam(const VersionParam& rhs);
	~VersionParam();

	VersionParam& operator=(const VersionParam& rhs);

private:
	QSharedDataPointer<VersionParamPrivate> d;
};

//VersionResult
class VersionResultPrivate;

class BUILDSYS_SERVICE_DECL VersionResult
{
public:
	VersionResult();
	VersionResult(const VersionResult& rhs);
	~VersionResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	QString number() const;
	void setNumber(const QString& number);

	QString link() const;
	void setLink(const QString& link);

	VersionResult& operator=(const VersionResult& rhs);

private:
	QSharedDataPointer<VersionResultPrivate> d;
};

//PhoneLoadConfigParam
class PhoneLoadConfigParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneLoadConfigParam
{
public:
	PhoneLoadConfigParam();
	PhoneLoadConfigParam(const PhoneLoadConfigParam& rhs);
	~PhoneLoadConfigParam();

	PhoneLoadConfigParam& operator=(const PhoneLoadConfigParam& rhs);

	QString lastTime() const;
	void setLastTime(const QString& lastTime);

	int reloadConfig() const;
	void setReloadConfig(int reload);

	bool operator==(const PhoneLoadConfigParam& rhs) const;
	bool operator!=(const PhoneLoadConfigParam& rhs) const;

private:
	QSharedDataPointer<PhoneLoadConfigParamPrivate> d;
};

//PhoneLoadConfigResult
class PhoneLoadConfigResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneLoadConfigResult
{
public:
	PhoneLoadConfigResult();
	PhoneLoadConfigResult(const PhoneLoadConfigResult& rhs);
	~PhoneLoadConfigResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	QVariant serviceLanguage() const;
	void setServiceLanguage(const QVariant& lang);

	QVariant majorArray() const;
	void setMajorArray(const QVariant& array);

	QVariant useApp() const;
	void setUseApp(const QVariant& useApp);

	QVariant callMode() const;
	void setCallMode(const QVariant& callMode);

	QString version() const;
	void setVersion(const QString& version);

	QString downloadUrl() const;
	void setDownloadUrl(const QString& url);

	QString forceUpdate() const;
	void setForceUpdate(const QString& force);

	QString verifyCode() const;
	void setVerifyCode(const QString& code);

	QString versionName() const;
	void setVersionName(const QString& name);

	QStringList PSTNSupported() const;
	void setPSTNSupported(const QStringList& supported);

	QString createAccountUrl() const;
	void setCreateAccountUrl(const QString& url);

	QString changePasswordUrl() const;
	void setChangePasswordUrl(const QString& url);

	QString editUserInfoUrl() const;
	void setEditUserInfoUrl(const QString& url);

	PhoneLoadConfigResult& operator=(const PhoneLoadConfigResult& rhs);

private:
	QSharedDataPointer<PhoneLoadConfigResultPrivate> d;
};

//PhoneRegisterParam
class PhoneRegisterParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneRegisterParam
{
public:
	PhoneRegisterParam();
	PhoneRegisterParam(const PhoneRegisterParam& rhs);
	~PhoneRegisterParam();

	PhoneRegisterParam& operator=(const PhoneRegisterParam& rhs);

	QString nativeLanguage() const;
	void setNativeLanguage(const QString& language);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	QString email() const;
	void setEmail(const QString& email);

	QString password() const;
	void setPassword(const QString& password);

	QString photo() const;
	void setPhoto(const QString& photo);

	bool operator==(const PhoneRegisterParam& rhs) const;
	bool operator!=(const PhoneRegisterParam& rhs) const;

private:
	QSharedDataPointer<PhoneRegisterParamPrivate> d;
};

//PhoneRegisterResult
class PhoneRegisterResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneRegisterResult
{
public:
	PhoneRegisterResult();
	PhoneRegisterResult(const PhoneRegisterResult& rhs);
	~PhoneRegisterResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	QString SIPCallerID() const;
	void setSIPCallerID(const QString& id);

	QString nickname() const;
	void setNickname(const QString& nickname);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	QString nativeLanguage() const;
	void setNativeLanguage(const QString& language);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString largePhoto() const;
	void setLargePhoto(const QString& photo);

	PhoneRegisterResult& operator=(const PhoneRegisterResult& rhs);

private:
	QSharedDataPointer<PhoneRegisterResultPrivate> d;
};

//PhoneLoginParam
class PhoneLoginParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneLoginParam
{
public:
	PhoneLoginParam();
	PhoneLoginParam(const PhoneLoginParam& rhs);
	~PhoneLoginParam();

	PhoneLoginParam& operator=(const PhoneLoginParam& rhs);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	QString password() const;
	void setPassword(const QString& password);

	bool operator==(const PhoneLoginParam& rhs) const;
	bool operator!=(const PhoneLoginParam& rhs) const;

private:
	QSharedDataPointer<PhoneLoginParamPrivate> d;
};

//PhoneLoginResult
class PhoneLoginResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneLoginResult
{
public:
	PhoneLoginResult();
	PhoneLoginResult(const PhoneLoginResult& rhs);
	~PhoneLoginResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	QString SIPCallerID() const;
	void setSIPCallerID(const QString& id);

	QString nativeLanguage() const;
	void setNativeLanguage(const QString& language);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString largePhoto() const;
	void setLargePhoto(const QString& photo);

	PhoneLoginResult& operator=(const PhoneLoginResult& rhs);

private:
	QSharedDataPointer<PhoneLoginResultPrivate> d;
};

//PhoneForgetPasswordParam
class PhoneForgetPasswordParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneForgetPasswordParam
{
public:
	PhoneForgetPasswordParam();
	PhoneForgetPasswordParam(const PhoneForgetPasswordParam& rhs);
	~PhoneForgetPasswordParam();

	PhoneForgetPasswordParam& operator=(const PhoneForgetPasswordParam& rhs);

	QString email() const;
	void setEmail(const QString& email);

	bool operator==(const PhoneForgetPasswordParam& rhs) const;
	bool operator!=(const PhoneForgetPasswordParam& rhs) const;

private:
	QSharedDataPointer<PhoneForgetPasswordParamPrivate> d;
};

//PhoneForgetPasswordResult
class PhoneForgetPasswordResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneForgetPasswordResult
{
public:
	PhoneForgetPasswordResult();
	PhoneForgetPasswordResult(const PhoneForgetPasswordResult& rhs);
	~PhoneForgetPasswordResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	PhoneForgetPasswordResult& operator=(const PhoneForgetPasswordResult& rhs);

private:
	QSharedDataPointer<PhoneForgetPasswordResultPrivate> d;
};

//PhoneContactListParam
class PhoneContactListParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneContactListParam
{
public:
	PhoneContactListParam();
	PhoneContactListParam(const PhoneContactListParam& rhs);
	~PhoneContactListParam();

	PhoneContactListParam& operator=(const PhoneContactListParam& rhs);

	QString searchType() const;
	void setSearchType(const QString& type);

	bool operator==(const PhoneContactListParam& rhs) const;
	bool operator!=(const PhoneContactListParam& rhs) const;

private:
	QSharedDataPointer<PhoneContactListParamPrivate> d;
};

//PhoneContactListResult
class PhoneContactListResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneContactListResult
{
public:
	PhoneContactListResult();
	PhoneContactListResult(const PhoneContactListResult& rhs);
	~PhoneContactListResult();

	QString ID() const;
	void setID(const QString& id);

	QString addBookID() const;
	void setAddBookID(const QString& id);

	QString contactName() const;
	void setContactName(const QString& name);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString largePhoto() const;
	void setLargePhoto(const QString& photo);

	QVariant phone() const;
	void setPhone(const QVariant& phone);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	QString SIPCallerID() const;
	void setSIPCallerID(const QString& id);

	QString sourceLanguage() const;
	void setSourceLanguage(const QString& language);

	QString targetLanguage() const;
	void setTargetLanguage(const QString& language);

	QString onlineStatus() const;
	void setOnlineStatus(const QString& status);

	QString systemUser() const;
	void setSystemUser(const QString& system);

	PhoneContactListResult& operator=(const PhoneContactListResult& rhs);

private:
	QSharedDataPointer<PhoneContactListResultPrivate> d;
};

//PhoneAddContactParam
class PhoneAddContactParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneAddContactParam
{
public:
	PhoneAddContactParam();
	PhoneAddContactParam(const PhoneAddContactParam& rhs);
	~PhoneAddContactParam();

	PhoneAddContactParam& operator=(const PhoneAddContactParam& rhs);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	QString name() const;
	void setName(const QString& name);

	QString phone() const;
	void setPhone(const QString& phone);

	QString phoneCountry() const;
	void setPhoneCountry(const QString& country);

	QString sourceLanguage() const;
	void setSourceLanguage(const QString& language);

	QString targetLanguage() const;
	void setTargetLanguage(const QString& language);

	bool operator==(const PhoneAddContactParam& rhs) const;
	bool operator!=(const PhoneAddContactParam& rhs) const;

private:
	QSharedDataPointer<PhoneAddContactParamPrivate> d;
};

//PhoneAddContactResult
class PhoneAddContactResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneAddContactResult
{
public:
	PhoneAddContactResult();
	PhoneAddContactResult(const PhoneAddContactResult& rhs);
	~PhoneAddContactResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	QString addBookID() const;
	void setAddBookID(const QString& id);

	QString ID() const;
	void setID(const QString& id);

	PhoneAddContactResult& operator=(const PhoneAddContactResult& rhs);

private:
	QSharedDataPointer<PhoneAddContactResultPrivate> d;
};

//PhoneUpdateContactParam
class PhoneUpdateContactParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneUpdateContactParam
{
public:
	PhoneUpdateContactParam();
	PhoneUpdateContactParam(const PhoneUpdateContactParam& rhs);
	~PhoneUpdateContactParam();

	PhoneUpdateContactParam& operator=(const PhoneUpdateContactParam& rhs);

	QString addBookID() const;
	void setAddBookID(const QString& id);

	QString updateType() const;
	void setUpdateType(const QString& type);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	QString name() const;
	void setName(const QString& name);

	QString phone() const;
	void setPhone(const QString& phone);

	QString oldPhone() const;
	void setOldPhone(const QString& phone);

	QString phoneCountry() const;
	void setPhoneCountry(const QString& country);

	QString deletePhone() const;
	void setDeletePhone(const QString& phone);

	QString sourceLanguage() const;
	void setSourceLanguage(const QString& language);

	QString targetLanguage() const;
	void setTargetLanguage(const QString& language);

	bool operator==(const PhoneUpdateContactParam& rhs) const;
	bool operator!=(const PhoneUpdateContactParam& rhs) const;

private:
	QSharedDataPointer<PhoneUpdateContactParamPrivate> d;
};

//PhoneUpdateContactResult
class PhoneUpdateContactResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneUpdateContactResult
{
public:
	PhoneUpdateContactResult();
	PhoneUpdateContactResult(const PhoneUpdateContactResult& rhs);
	~PhoneUpdateContactResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	PhoneUpdateContactResult& operator=(const PhoneUpdateContactResult& rhs);

private:
	QSharedDataPointer<PhoneUpdateContactResultPrivate> d;
};

//PhoneDeleteContactParam
class PhoneDeleteContactParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneDeleteContactParam
{
public:
	PhoneDeleteContactParam();
	PhoneDeleteContactParam(const PhoneDeleteContactParam& rhs);
	~PhoneDeleteContactParam();

	PhoneDeleteContactParam& operator=(const PhoneDeleteContactParam& rhs);

	QString addBookID() const;
	void setAddBookID(const QString& id);

	bool operator==(const PhoneDeleteContactParam& rhs) const;
	bool operator!=(const PhoneDeleteContactParam& rhs) const;

private:
	QSharedDataPointer<PhoneDeleteContactParamPrivate> d;
};

//PhoneDeleteContactResult
class PhoneDeleteContactResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneDeleteContactResult
{
public:
	PhoneDeleteContactResult();
	PhoneDeleteContactResult(const PhoneDeleteContactResult& rhs);
	~PhoneDeleteContactResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	PhoneDeleteContactResult& operator=(const PhoneDeleteContactResult& rhs);

private:
	QSharedDataPointer<PhoneDeleteContactResultPrivate> d;
};

//PhoneDetailContactParam
class PhoneDetailContactParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneDetailContactParam
{
public:
	PhoneDetailContactParam();
	PhoneDetailContactParam(const PhoneDetailContactParam& rhs);
	~PhoneDetailContactParam();

	PhoneDetailContactParam& operator=(const PhoneDetailContactParam& rhs);

	QString addBookID() const;
	void setAddBookID(const QString& id);

	bool operator==(const PhoneDetailContactParam& rhs) const;
	bool operator!=(const PhoneDetailContactParam& rhs) const;

private:
	QSharedDataPointer<PhoneDetailContactParamPrivate> d;
};

//PhoneDetailContactResult
class PhoneDetailContactResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneDetailContactResult
{
public:
	PhoneDetailContactResult();
	PhoneDetailContactResult(const PhoneDetailContactResult& rhs);
	~PhoneDetailContactResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	QString nativeLanguage() const;
	void setNativeLanguage(const QString& language);

	QString nickname() const;
	void setNickname(const QString& nickname);

	QString country() const;
	void setCountry(const QString& country);

	QString major() const;
	void setMajor(const QString& major);

	PhoneDetailContactResult& operator=(const PhoneDetailContactResult& rhs);

private:
	QSharedDataPointer<PhoneDetailContactResultPrivate> d;
};

//PhoneGetHistoryCDRParam
class PhoneGetHistoryCDRParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneGetHistoryCDRParam
{
public:
	PhoneGetHistoryCDRParam();
	PhoneGetHistoryCDRParam(const PhoneGetHistoryCDRParam& rhs);
	~PhoneGetHistoryCDRParam();

	PhoneGetHistoryCDRParam& operator=(const PhoneGetHistoryCDRParam& rhs);

	QString historyType() const;
	void setHistoryType(const QString& type);

	bool operator==(const PhoneGetHistoryCDRParam& rhs) const;
	bool operator!=(const PhoneGetHistoryCDRParam& rhs) const;

private:
	QSharedDataPointer<PhoneGetHistoryCDRParamPrivate> d;
};

//PhoneGetHistoryCDRResult
class PhoneGetHistoryCDRResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneGetHistoryCDRResult
{
public:
	PhoneGetHistoryCDRResult();
	PhoneGetHistoryCDRResult(const PhoneGetHistoryCDRResult& rhs);
	~PhoneGetHistoryCDRResult();

	QString ID() const;
	void setID(const QString& id);

	QString meetCDRID() const;
	void setMeetCDRID(const QString& id);

	QString calleeID() const;
	void setCalleeID(const QString& id);

	QString sourceLanguage() const;
	void setSourceLanguage(const QString& language);

	QString targetLanguage() const;
	void setTargetLanguage(const QString& language);

	QString callingTime() const;
	void setCallingTime(const QString& callingTime);

	QString cost() const;
	void setCost(const QString& cost);

	QString isSuccess() const;
	void setIsSuccess(const QString& success);

	QString isCallout() const;
	void setIsCallout(const QString& callout);

	PhoneGetHistoryCDRResult& operator=(const PhoneGetHistoryCDRResult& rhs);

private:
	QSharedDataPointer<PhoneGetHistoryCDRResultPrivate> d;
};

//PhoneGetUserInfoParam
class PhoneGetUserInfoParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneGetUserInfoParam
{
public:
	PhoneGetUserInfoParam();
	PhoneGetUserInfoParam(const PhoneGetUserInfoParam& rhs);
	~PhoneGetUserInfoParam();

	PhoneGetUserInfoParam& operator=(const PhoneGetUserInfoParam& rhs);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	bool operator==(const PhoneGetUserInfoParam& rhs) const;
	bool operator!=(const PhoneGetUserInfoParam& rhs) const;

private:
	QSharedDataPointer<PhoneGetUserInfoParamPrivate> d;
};

//PhoneGetUserInfoResult
class PhoneGetUserInfoResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneGetUserInfoResult
{
public:
	PhoneGetUserInfoResult();
	PhoneGetUserInfoResult(const PhoneGetUserInfoResult& rhs);
	~PhoneGetUserInfoResult();

	QString email() const;
	void setEmail(const QString& email);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	QString SIPCallerID() const;
	void setSIPCallerID(const QString& callerID);

	QString balance() const;
	void setBalance(const QString& balance);

	QString nickname() const;
	void setNickname(const QString& nickname);

	QString nativeLanguage() const;
	void setNativeLanguage(const QString& language);

	QString major() const;
	void setMajor(const QString& major);

	QString country() const;
	void setCountry(const QString& country);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString largePhoto() const;
	void setLargePhoto(const QString& photo);

	QString onlineStatus() const;
	void setOnlineStatus(const QString& status);

	PhoneGetUserInfoResult& operator=(const PhoneGetUserInfoResult& rhs);

private:
	QSharedDataPointer<PhoneGetUserInfoResultPrivate> d;
};

//PhoneEditUserInfoParam
class PhoneEditUserInfoParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneEditUserInfoParam
{
public:
	PhoneEditUserInfoParam();
	PhoneEditUserInfoParam(const PhoneEditUserInfoParam& rhs);
	~PhoneEditUserInfoParam();

	PhoneEditUserInfoParam& operator=(const PhoneEditUserInfoParam& rhs);

	QString nickname() const;
	void setNickname(const QString& nickname);

	QString nativeLanguage() const;
	void setNativeLanguage(const QString& language);

	QString nationlity() const;
	void setNationlity(const QString& nation);

	QString major() const;
	void setMajor(const QString& major);

	QString onlineStatus() const;
	void setOnlineStatus(const QString& status);

	QString photo() const;
	void setPhoto(const QString& photo);

	QString updateType() const;
	void setUpdateType(const QString& type);

	bool operator==(const PhoneEditUserInfoParam& rhs) const;
	bool operator!=(const PhoneEditUserInfoParam& rhs) const;

private:
	QSharedDataPointer<PhoneEditUserInfoParamPrivate> d;
};

//PhoneEditUserInfoResult
class PhoneEditUserInfoResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneEditUserInfoResult
{
public:
	PhoneEditUserInfoResult();
	PhoneEditUserInfoResult(const PhoneEditUserInfoResult& rhs);
	~PhoneEditUserInfoResult();

	PhoneEditUserInfoResult& operator=(const PhoneEditUserInfoResult& rhs);

private:
	QSharedDataPointer<PhoneEditUserInfoResultPrivate> d;
};

//PhoneChangePasswordParam
class PhoneChangePasswordParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneChangePasswordParam
{
public:
	PhoneChangePasswordParam();
	PhoneChangePasswordParam(const PhoneChangePasswordParam& rhs);
	~PhoneChangePasswordParam();

	PhoneChangePasswordParam& operator=(const PhoneChangePasswordParam& rhs);

	QString authCode() const;
	void setAuthCode(const QString& code);

	QString email() const;
	void setEmail(const QString& email);

	bool operator==(const PhoneChangePasswordParam& rhs) const;
	bool operator!=(const PhoneChangePasswordParam& rhs) const;

private:
	QSharedDataPointer<PhoneChangePasswordParamPrivate> d;
};

//PhoneChangePasswordResult
class PhoneChangePasswordResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneChangePasswordResult
{
public:
	PhoneChangePasswordResult();
	PhoneChangePasswordResult(const PhoneChangePasswordResult& rhs);
	~PhoneChangePasswordResult();

	PhoneChangePasswordResult& operator=(const PhoneChangePasswordResult& rhs);

private:
	QSharedDataPointer<PhoneChangePasswordResultPrivate> d;
};

//PhoneNewPasswordParam
class PhoneNewPasswordParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneNewPasswordParam
{
public:
	PhoneNewPasswordParam();
	PhoneNewPasswordParam(const PhoneNewPasswordParam& rhs);
	~PhoneNewPasswordParam();

	PhoneNewPasswordParam& operator=(const PhoneNewPasswordParam& rhs);

	QString newPassword1() const;
	void setNewPassword1(const QString& password);

	QString newPassword2() const;
	void setNewPassword2(const QString& password);

	QString authCode() const;
	void setAuthCode(const QString& code);

	QString email() const;
	void setEmail(const QString& email);

	bool operator==(const PhoneNewPasswordParam& rhs) const;
	bool operator!=(const PhoneNewPasswordParam& rhs) const;

private:
	QSharedDataPointer<PhoneNewPasswordParamPrivate> d;
};

//PhoneNewPasswordResult
class PhoneNewPasswordResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneNewPasswordResult
{
public:
	PhoneNewPasswordResult();
	PhoneNewPasswordResult(const PhoneNewPasswordResult& rhs);
	~PhoneNewPasswordResult();

	PhoneNewPasswordResult& operator=(const PhoneNewPasswordResult& rhs);

private:
	QSharedDataPointer<PhoneNewPasswordResultPrivate> d;
};

//PhoneGetCDRIDParam
class PhoneGetCDRIDParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneGetCDRIDParam
{
public:
	PhoneGetCDRIDParam();
	PhoneGetCDRIDParam(const PhoneGetCDRIDParam& rhs);
	~PhoneGetCDRIDParam();

	PhoneGetCDRIDParam& operator=(const PhoneGetCDRIDParam& rhs);

	QString CDRID() const;
	void setCDRID(const QString& id);

	bool operator==(const PhoneGetCDRIDParam& rhs) const;
	bool operator!=(const PhoneGetCDRIDParam& rhs) const;

private:
	QSharedDataPointer<PhoneGetCDRIDParamPrivate> d;
};

//PhoneGetCDRIDResult
class PhoneGetCDRIDResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneGetCDRIDResult
{
public:
	PhoneGetCDRIDResult();
	PhoneGetCDRIDResult(const PhoneGetCDRIDResult& rhs);
	~PhoneGetCDRIDResult();

	QString ID() const;
	void setID(const QString& id);

	QString callerID() const;
	void setCallerID(const QString& callerID);

	QString deductionTime() const;
	void setDeductionTime(const QString& deductionTime);

	QString meetCDRID() const;
	void setMeetCDRID(const QString& id);
	
	QString callTime() const;
	void setCallTime(const QString& callTime);

	QString contactName() const;
	void setContactName(const QString& name);

	QString smallPhoto() const;
	void setSmallPhoto(const QString& photo);

	QString largePhoto() const;
	void setLargePhoto(const QString& photo);

	QString callStatus() const;
	void setCallStatus(const QString& status);

	QString callMode() const;
	void setCallMode(const QString& mode);

	QString deductionCost() const;
	void setDeductionCost(const QString& cost);

	QString sourceLanguage() const;
	void setSourceLanguage(const QString& language);

	QString targetLanguage() const;
	void setTargetLanguage(const QString& language);

	QString PSTNCountry() const;
	void setPSTNCountry(const QString& country);

	QString voiceraID() const;
	void setVoiceraID(const QString& id);

	PhoneGetCDRIDResult& operator=(const PhoneGetCDRIDResult& rhs);

private:
	QSharedDataPointer<PhoneGetCDRIDResultPrivate> d;
};

//PhoneDeleteCDRParam
class PhoneDeleteCDRParamPrivate;

class BUILDSYS_SERVICE_DECL PhoneDeleteCDRParam
{
public:
	PhoneDeleteCDRParam();
	PhoneDeleteCDRParam(const PhoneDeleteCDRParam& rhs);
	~PhoneDeleteCDRParam();

	PhoneDeleteCDRParam& operator=(const PhoneDeleteCDRParam& rhs);

	QString meetCDRID() const;
	void setMeetCDRID(const QString& id);

	bool operator==(const PhoneDeleteCDRParam& rhs) const;
	bool operator!=(const PhoneDeleteCDRParam& rhs) const;

private:
	QSharedDataPointer<PhoneDeleteCDRParamPrivate> d;
};

//PhoneDeleteCDRResult
class PhoneDeleteCDRResultPrivate;

class BUILDSYS_SERVICE_DECL PhoneDeleteCDRResult
{
public:
	PhoneDeleteCDRResult();
	PhoneDeleteCDRResult(const PhoneDeleteCDRResult& rhs);
	~PhoneDeleteCDRResult();

	QString error() const;
	void setError(const QString& err);

	QString errorMsg() const;
	void setErrorMsg(const QString& errMsg);

	PhoneDeleteCDRResult& operator=(const PhoneDeleteCDRResult& rhs);

private:
	QSharedDataPointer<PhoneDeleteCDRResultPrivate> d;
};


} // namespace ns::Command
} // namespace ns

Q_DECLARE_METATYPE(ns::Command::CommonResult)

Q_DECLARE_METATYPE(ns::Command::LoadConfigParam)
Q_DECLARE_METATYPE(ns::Command::LoadConfigResult)
Q_DECLARE_METATYPE(ns::Command::GetServerParam)
Q_DECLARE_METATYPE(ns::Command::GetServerResult)
Q_DECLARE_METATYPE(ns::Command::LoginParam)
Q_DECLARE_METATYPE(ns::Command::LoginResult)
Q_DECLARE_METATYPE(ns::Command::SetClientInfoParam)
Q_DECLARE_METATYPE(ns::Command::TodayStatParam)
Q_DECLARE_METATYPE(ns::Command::TodayStatResult)
Q_DECLARE_METATYPE(ns::Command::GetNoReportParam)
Q_DECLARE_METATYPE(ns::Command::GetNoReportResult)
Q_DECLARE_METATYPE(ns::Command::GetCDRListParam)
Q_DECLARE_METATYPE(ns::Command::GetCDRListResult)
Q_DECLARE_METATYPE(ns::Command::GetCDRInfoParam)
Q_DECLARE_METATYPE(ns::Command::GetCDRInfoResult)
Q_DECLARE_METATYPE(ns::Command::ForgetPasswordParam)
Q_DECLARE_METATYPE(ns::Command::CreateAccountParam)
Q_DECLARE_METATYPE(ns::Command::ChangeStatusParam)
Q_DECLARE_METATYPE(ns::Command::ChangeStatusResult)
Q_DECLARE_METATYPE(ns::Command::GetBlacklistParam)
Q_DECLARE_METATYPE(ns::Command::GetBlacklistResult)
Q_DECLARE_METATYPE(ns::Command::AddBlacklistParam)
Q_DECLARE_METATYPE(ns::Command::AddBlacklistResult)
Q_DECLARE_METATYPE(ns::Command::DelBlacklistParam)
Q_DECLARE_METATYPE(ns::Command::DelBlacklistResult)
Q_DECLARE_METATYPE(ns::Command::GetMemberInfoParam)
Q_DECLARE_METATYPE(ns::Command::GetMemberInfoResult)
Q_DECLARE_METATYPE(ns::Command::GetUploadPhotoParam)
Q_DECLARE_METATYPE(ns::Command::GetUploadPhotoResult)
Q_DECLARE_METATYPE(ns::Command::VersionParam)
Q_DECLARE_METATYPE(ns::Command::VersionResult)

Q_DECLARE_METATYPE(ns::Command::PhoneLoadConfigParam)
Q_DECLARE_METATYPE(ns::Command::PhoneLoadConfigResult)
Q_DECLARE_METATYPE(ns::Command::PhoneRegisterParam)
Q_DECLARE_METATYPE(ns::Command::PhoneRegisterResult)
Q_DECLARE_METATYPE(ns::Command::PhoneLoginParam)
Q_DECLARE_METATYPE(ns::Command::PhoneLoginResult)
Q_DECLARE_METATYPE(ns::Command::PhoneForgetPasswordParam)
Q_DECLARE_METATYPE(ns::Command::PhoneForgetPasswordResult)
Q_DECLARE_METATYPE(ns::Command::PhoneContactListParam)
Q_DECLARE_METATYPE(ns::Command::PhoneContactListResult)
Q_DECLARE_METATYPE(ns::Command::PhoneAddContactParam)
Q_DECLARE_METATYPE(ns::Command::PhoneAddContactResult)
Q_DECLARE_METATYPE(ns::Command::PhoneUpdateContactParam)
Q_DECLARE_METATYPE(ns::Command::PhoneUpdateContactResult)
Q_DECLARE_METATYPE(ns::Command::PhoneDeleteContactParam)
Q_DECLARE_METATYPE(ns::Command::PhoneDeleteContactResult)
Q_DECLARE_METATYPE(ns::Command::PhoneDetailContactParam)
Q_DECLARE_METATYPE(ns::Command::PhoneDetailContactResult)
Q_DECLARE_METATYPE(ns::Command::PhoneGetHistoryCDRParam)
Q_DECLARE_METATYPE(ns::Command::PhoneGetHistoryCDRResult)
Q_DECLARE_METATYPE(ns::Command::PhoneGetUserInfoParam)
Q_DECLARE_METATYPE(ns::Command::PhoneGetUserInfoResult)
Q_DECLARE_METATYPE(ns::Command::PhoneEditUserInfoParam)
Q_DECLARE_METATYPE(ns::Command::PhoneEditUserInfoResult)
Q_DECLARE_METATYPE(ns::Command::PhoneChangePasswordParam)
Q_DECLARE_METATYPE(ns::Command::PhoneChangePasswordResult)
Q_DECLARE_METATYPE(ns::Command::PhoneNewPasswordParam)
Q_DECLARE_METATYPE(ns::Command::PhoneNewPasswordResult)
Q_DECLARE_METATYPE(ns::Command::PhoneGetCDRIDParam)
Q_DECLARE_METATYPE(ns::Command::PhoneGetCDRIDResult)
Q_DECLARE_METATYPE(ns::Command::PhoneDeleteCDRParam)
Q_DECLARE_METATYPE(ns::Command::PhoneDeleteCDRResult)

#endif // COMMAND_H
