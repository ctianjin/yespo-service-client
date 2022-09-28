#ifndef COMMANDSERVICE_H
#define COMMANDSERVICE_H

#include "abstractservice.h"
#include "command.h"
#include "serviceglobal.h"

QT_BEGIN_NAMESPACE
class QNetworkCookie;
QT_END_NAMESPACE

namespace ns {
class UserService;
class CommandServicePrivate;

class BUILDSYS_SERVICE_DECL CommandService : public AbstractService
{
    Q_OBJECT
    Q_PROPERTY(UserService* userService READ userService WRITE setUserService)

public:
    explicit CommandService(ServiceManager* manager, QObject* parent = 0);
    CommandService(ServiceManager* manager, const QString& name, QObject* parent = 0);
    virtual ~CommandService();

    UserService* userService() const;
    void setUserService(UserService* userService);

	void setCommonItem(const QString& client, const QString& version, const QString& language, const QString& deviceId, const QString& deviceModel);

	QList<QNetworkCookie> cookies() const;
	void setCookies(const QList<QNetworkCookie>& cookies);

	QString token() const;
	void setToken(const QString& token);

	QString deviceId() const;

	void loadconfig(const Command::LoadConfigParam& param);
	void getserver(const Command::GetServerParam& param);
    void login(const Command::LoginParam& param);
	void login2(const Command::LoginParam& param);
	void setclientinfo(const Command::SetClientInfoParam& param);
	void todayStat(const Command::TodayStatParam& param);
	void getnoreportinfo(const Command::GetNoReportParam& param);
	void getcdrlist(const Command::GetCDRListParam& param);
	void getcdrinfo(const Command::GetCDRInfoParam& param);
	void getuserinfo(const Command::GetUserInfoParam& param);
	void getcalleruser(const Command::GetCallerUserParam& param);
	void savescreenshot(const Command::SaveScreenShotParam& param);
    void forgetpassword(const Command::ForgetPasswordParam& param);
	void createaccount(const Command::CreateAccountParam& param);
    void changestatus(const Command::ChangeStatusParam& param);
	void getblacklist(const Command::GetBlacklistParam& param);
	void addblacklist(const Command::AddBlacklistParam& param);
	void delblacklist(const Command::DelBlacklistParam& param);
	void getmemberinfo(const Command::GetMemberInfoParam& param);
	void getuploadphoto(const Command::GetUploadPhotoParam& param);
	void version(const Command::VersionParam& param);

	void  phoneloadconfig(const Command::PhoneLoadConfigParam& param);
	void  phoneregister(const Command::PhoneRegisterParam& param);
	void  phonelogin(const Command::PhoneLoginParam& param);
	void  phoneforgetpassword(const Command::PhoneForgetPasswordParam& param);
	void  phonecontactlist(const Command::PhoneContactListParam& param);
	void  phoneaddcontact(const Command::PhoneAddContactParam& param);
	void  phoneupdatecontact(const Command::PhoneUpdateContactParam& param);
	void  phonedelcontact(const Command::PhoneDeleteContactParam& param);
	void  phonedetailcontact(const Command::PhoneDetailContactParam& param);
	void  phonegethistorycdr(const Command::PhoneGetHistoryCDRParam& param);
	void  phonegetuserinfo(const Command::PhoneGetUserInfoParam& param);
	void  phoneedituserinfo(const Command::PhoneEditUserInfoParam& param);
	void  phonechangepassword(const Command::PhoneChangePasswordParam& param);
	void  phonenewpassword(const Command::PhoneNewPasswordParam& param);
	void  phonegetcdrid(const Command::PhoneGetCDRIDParam& param);
	void  phonedelcdr(const Command::PhoneDeleteCDRParam& param);

Q_SIGNALS:
	void loadconfigFinished(const Command::LoadConfigParam& param, const Command::LoadConfigResult& result);
	void getserverFinished(const Command::GetServerParam& param, const Command::GetServerResult& result);
    void loginFinished(const Command::LoginParam& param, const Command::LoginResult& result);
	void todayStatFinished(const Command::TodayStatParam& param, const Command::TodayStatResult& result);
	void getnoreportinfoFinished(const Command::GetNoReportParam& param, const Command::GetNoReportResult& result);
	void setclientinfoFinished(const Command::SetClientInfoParam& param, const Command::SetClientInfoResult& result);
	void getcdrlistFinished(const Command::GetCDRListParam& param, const Command::GetCDRListResult& result);
	void getcdrinfoFinished(const Command::GetCDRInfoParam& param, const Command::GetCDRInfoResult& result);
	void getuserinfoFinished(const Command::GetUserInfoParam& param, const Command::GetUserInfoResult& result);
	void getcalleruserFinished(const Command::GetCallerUserParam& param, const Command::GetCallerUserResult& result);
	void savescreenshotFinished(const Command::SaveScreenShotParam& param, const Command::SaveScreenShotResult& result);
    void changestatusFinished(const Command::ChangeStatusParam& param, const Command::ChangeStatusResult& result);
	void getblacklistFinished(const Command::GetBlacklistParam& param, const Command::GetBlacklistResult& result);
	void addblacklistFinished(const Command::AddBlacklistParam& param, const Command::AddBlacklistResult& result);
	void delblacklistFinished(const Command::DelBlacklistParam& param, const Command::DelBlacklistResult& result);
	void getmemberinfoFinished(const Command::GetMemberInfoParam& param, const Command::GetMemberInfoResult& result);
	void getuploadphotoFinished(const Command::GetUploadPhotoParam& param, const Command::GetUploadPhotoResult& result);
	void versionFinished(const Command::VersionParam& param, const Command::VersionResult& result);

	void  phoneloadconfigFinished(const Command::PhoneLoadConfigParam& param, const Command::PhoneLoadConfigResult& result);
	void  phoneregisterFinished(const Command::PhoneRegisterParam& param, const Command::PhoneRegisterResult& result);
	void  phoneloginFinished(const Command::PhoneLoginParam& param, const Command::PhoneLoginResult& result);
	void  phoneforgetpasswordFinished(const Command::PhoneForgetPasswordParam& param, const Command::PhoneForgetPasswordResult& result);
	void  phonecontactlistFinished(const Command::PhoneContactListParam& param, const Command::PhoneContactListResult& result);
	void  phoneaddcontactFinished(const Command::PhoneAddContactParam& param, const Command::PhoneAddContactResult& result);
	void  phoneupdatecontactFinished(const Command::PhoneUpdateContactParam& param, const Command::PhoneUpdateContactResult& result);
	void  phonedelcontactFinished(const Command::PhoneDeleteContactParam& param, const Command::PhoneDeleteContactResult& result);
	void  phonedetailcontactFinished(const Command::PhoneDetailContactParam& param, const Command::PhoneDetailContactResult& result);
	void  phonegethistorycdrFinished(const Command::PhoneGetHistoryCDRParam& param, const Command::PhoneGetHistoryCDRResult& result);
	void  phonegetuserinfoFinished(const Command::PhoneGetUserInfoParam& param, const Command::PhoneGetUserInfoResult& result);
	void  phoneedituserinfoFinished(const Command::PhoneEditUserInfoParam& param, const Command::PhoneEditUserInfoResult& result);
	void  phonechangepasswordFinished(const Command::PhoneChangePasswordParam& param, const Command::PhoneChangePasswordResult& result);
	void  phonenewpasswordFinished(const Command::PhoneNewPasswordParam& param, const Command::PhoneNewPasswordResult& result);
	void  phonegetcdridFinished(const Command::PhoneGetCDRIDParam& param, const Command::PhoneGetCDRIDResult& result);
	void  phonedelcdrFinished(const Command::PhoneDeleteCDRParam& param, const Command::PhoneDeleteCDRResult& result);
	
protected:
    CommandService(CommandServicePrivate& dd, QObject* parent = 0);

    virtual void applyPreference(const QSet<QString>& paths, bool force);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_commandFinished(const QString&, const DownloadResult&))

    Q_DECLARE_PRIVATE(CommandService)
    Q_DISABLE_COPY(CommandService)
};

} // namespace ns

#endif // COMMANDSERVICE_H
