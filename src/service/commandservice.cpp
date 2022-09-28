#include <QCryptographicHash>
#include <QDateTime>
#include <QUrl>
#include <QVariant>
#include <QVariantMap>
#include <QDebug>
#include <QFile>
#include <QDesktopServices>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QHttpMultiPart>
#include "commandservice.h"
#include "commandservice_p.h"
#include "userservice.h"
#include "servicemanager.h"
#include "qjson/parser.h"
#include "qjson/serializer.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "CommandService:"
#endif // DEBUG_PREFIX

namespace ns {

const QString CommandServiceOptions::common_ = QLatin1String("common");
const QString CommandServiceOptions::common_query_session_id_ = QLatin1String("common.query.session_id");
const QString CommandServiceOptions::common_query_sign_ = QLatin1String("common.query.sign");
const QString CommandServiceOptions::common_query_timestamp_ = QLatin1String("common.query.timestamp");
const QString CommandServiceOptions::common_query_client_ = QLatin1String("common.query.client");
const QString CommandServiceOptions::common_query_version_ = QLatin1String("common.query.version");
const QString CommandServiceOptions::common_query_language_ = QLatin1String("common.query.language");
const QString CommandServiceOptions::common_query_device_id_ = QLatin1String("common.query.device_id");
const QString CommandServiceOptions::common_query_device_model_ = QLatin1String("common.query.device_model");
const QString CommandServiceOptions::common_query_token_ = QLatin1String("common.query.token");
const QString CommandServiceOptions::common_key_error_code_ = QLatin1String("common.key.error_code");
const QString CommandServiceOptions::common_key_error_message_ = QLatin1String("common.key.error_message");
const QString CommandServiceOptions::common_key_error_number_ = QLatin1String("common.key.error_number");
const QString CommandServiceOptions::common_key_error_tips_ = QLatin1String("common.key.error_tips");
const QString CommandServiceOptions::common_key_result_ = QLatin1String("common.key.result");
const QString CommandServiceOptions::loadconfig_ = QLatin1String("loadconfig");
const QString CommandServiceOptions::loadconfig_url_ = QLatin1String("loadconfig.url");
const QString CommandServiceOptions::loadconfig_query_last_time_ = QLatin1String("loadconfig.query.last_time");
const QString CommandServiceOptions::loadconfig_query_reload_ = QLatin1String("loadconfig.query.reload");
const QString CommandServiceOptions::loadconfig_key_config_ = QLatin1String("loadconfig.key.config");
const QString CommandServiceOptions::loadconfig_key_client_ = QLatin1String("loadconfig.key.client");
const QString CommandServiceOptions::getserver_ = QLatin1String("getserver");
const QString CommandServiceOptions::getserver_url_ = QLatin1String("getserver.url");
const QString CommandServiceOptions::getserver_key_domain_status_ = QLatin1String("getserver.key.domain_status");
const QString CommandServiceOptions::getserver_key_domain_call_= QLatin1String("getserver.key.domain_call");
const QString CommandServiceOptions::getserver_key_domain_im_= QLatin1String("getserver.key.domain_im");
const QString CommandServiceOptions::login_ = QLatin1String("login");
const QString CommandServiceOptions::login_url_ = QLatin1String("login.url");
const QString CommandServiceOptions::login_encrypt_key_ = QLatin1String("login.encrypt_key");
const QString CommandServiceOptions::login_query_user_name_ = QLatin1String("login.query.user_name");
const QString CommandServiceOptions::login_query_password_ = QLatin1String("login.query.password");
const QString CommandServiceOptions::login_key_user_ = QLatin1String("login.key.partner_user");
const QString CommandServiceOptions::login_key_user_system_id_ = QLatin1String("login.key.partner_user.partner_sys_id");
const QString CommandServiceOptions::login_key_user_caller_id_ = QLatin1String("login.key.partner_user.partner_caller_id");
const QString CommandServiceOptions::login_key_user_login_id_= QLatin1String("login.key.partner_user.partner_login_id");
const QString CommandServiceOptions::login_key_user_secret_ = QLatin1String("login.key.partner_user.partner_secret");
const QString CommandServiceOptions::login_key_user_display_name_ = QLatin1String("login.key.partner_user.partner_display_name");
const QString CommandServiceOptions::login_key_user_skill_ = QLatin1String("login.key.partner_user.partner_skill");
const QString CommandServiceOptions::login_key_user_skill_language_1_ = QLatin1String("login.key.partner_user.partner_skill.language_1");
const QString CommandServiceOptions::login_key_user_skill_language_2_ = QLatin1String("login.key.partner_user.partner_skill.language_2");
const QString CommandServiceOptions::login_key_user_skill_is_free_ = QLatin1String("login.key.partner_user.partner_skill.is_free");
const QString CommandServiceOptions::login_key_user_skill_level_ = QLatin1String("login.key.partner_user.partner_skill.partner_level");
const QString CommandServiceOptions::login_key_user_local_ = QLatin1String("login.key.partner_user.partner_local");
const QString CommandServiceOptions::login_key_user_local_country_ = QLatin1String("login.key.partner_user.partner_local.country");
const QString CommandServiceOptions::login_key_user_local_language_ = QLatin1String("login.key.partner_user.partner_local.language");
const QString CommandServiceOptions::login_key_user_small_photo_ = QLatin1String("login.key.partner_user.partner_small_photo");
const QString CommandServiceOptions::login_key_user_large_photo_ = QLatin1String("login.key.partner_user.partner_large_photo");
const QString CommandServiceOptions::login_key_user_token_ = QLatin1String("login.key.partner_user.partner_token");
const QString CommandServiceOptions::login_key_server_ip_ = QLatin1String("login.key.partner_server_ip");
const QString CommandServiceOptions::login_key_server_ip_domain_status_ = QLatin1String("login.key.partner_server_ip.domain_status");
const QString CommandServiceOptions::login_key_server_ip_domain_call_ = QLatin1String("login.key.partner_server_ip.domain_call");
const QString CommandServiceOptions::login_key_server_ip_domain_im_ = QLatin1String("login.key.partner_server_ip.domain_im");
const QString CommandServiceOptions::login_key_servers_ = QLatin1String("login.key.partner_server_list");
const QString CommandServiceOptions::login_key_servers_domain_call_ = QLatin1String("login.key.partner_server_list.domain_call");
const QString CommandServiceOptions::login_key_servers_domain_status_ = QLatin1String("login.key.partner_server_list.domain_status");
const QString CommandServiceOptions::login_key_servers_domain_im_ = QLatin1String("login.key.partner_server_list.domain_im");
const QString CommandServiceOptions::setclientinfo_ = QLatin1String("setclientinfo");
const QString CommandServiceOptions::setclientinfo_url_ = QLatin1String("setclientinfo.url");
const QString CommandServiceOptions::setclientinfo_query_last_domain_call_ = QLatin1String("setclientinfo.query.last_domain_call");
const QString CommandServiceOptions::todaystat_ = QLatin1String("todaystat");
const QString CommandServiceOptions::todaystat_url_ = QLatin1String("todaystat.url");
const QString CommandServiceOptions::todaystat_key_answer_ = QLatin1String("todaystat.key.answer");
const QString CommandServiceOptions::todaystat_key_success_ = QLatin1String("todaystat.key.success");
const QString CommandServiceOptions::todaystat_key_noanswer_ = QLatin1String("todaystat.key.noanswer");
const QString CommandServiceOptions::getnoreportinfo_ = QLatin1String("getnoreportinfo");
const QString CommandServiceOptions::getnoreportinfo_url_ = QLatin1String("getnoreportinfo.url");
const QString CommandServiceOptions::getnoreportinfo_key_no_report_num_ = QLatin1String("getnoreportinfo.key.no_report_num");
const QString CommandServiceOptions::getcdrlist_ = QLatin1String("getcdrlist");
const QString CommandServiceOptions::getcdrlist_url_ = QLatin1String("getcdrlist.url");
const QString CommandServiceOptions::getcdrlist_query_id_ = QLatin1String("getcdrlist.query.id");
const QString CommandServiceOptions::getcdrlist_query_type_ = QLatin1String("getcdrlist.query.type");
const QString CommandServiceOptions::getcdrlist_key_id_ = QLatin1String("getcdrlist.key.id");
const QString CommandServiceOptions::getcdrlist_key_ve_sys_id_ = QLatin1String("getcdrlist.key.ve_sys_id");
const QString CommandServiceOptions::getcdrlist_key_meet_cdr_id_ = QLatin1String("getcdrlist.key.meet_cdr_id");
const QString CommandServiceOptions::getcdrlist_key_small_photo_ = QLatin1String("getcdrlist.key.small_photo");
const QString CommandServiceOptions::getcdrlist_key_displayname_ = QLatin1String("getcdrlist.key.display_name");
const QString CommandServiceOptions::getcdrlist_key_start_call_time_ = QLatin1String("getcdrlist.key.start_call_time");
const QString CommandServiceOptions::getcdrlist_key_call_time_ = QLatin1String("getcdrlist.key.call_time");
const QString CommandServiceOptions::getcdrlist_key_is_answered_ = QLatin1String("getcdrlist.key.is_answered");
const QString CommandServiceOptions::getcdrlist_key_call_mode_ = QLatin1String("getcdrlist.key.call_mode");
const QString CommandServiceOptions::getcdrlist_key_is_success_ = QLatin1String("getcdrlist.key.is_success");
const QString CommandServiceOptions::getcdrinfo_ = QLatin1String("getcdrinfo");
const QString CommandServiceOptions::getcdrinfo_url_ = QLatin1String("getcdrinfo.url");
const QString CommandServiceOptions::getcdrinfo_query_meet_cdr_id_ = QLatin1String("getcdrinfo.query.meet_cdr_id");
const QString CommandServiceOptions::getcdrinfo_key_cdr_type_ = QLatin1String("getcdrinfo.key.cdr_type");
const QString CommandServiceOptions::getcdrinfo_key_caller_ve_sys_id_ = QLatin1String("getcdrinfo.key.caller_ve_sys_id");
const QString CommandServiceOptions::getcdrinfo_key_caller_id_ = QLatin1String("getcdrinfo.key.caller_id");
const QString CommandServiceOptions::getcdrinfo_key_caller_ve_login_id_ = QLatin1String("getcdrinfo.key.caller_ve_login_id");
const QString CommandServiceOptions::getcdrinfo_key_caller_display_name_ = QLatin1String("getcdrinfo.key.caller_display_name");
const QString CommandServiceOptions::getcdrinfo_key_caller_mobile_ = QLatin1String("getcdrinfo.key.caller_mobile");
const QString CommandServiceOptions::getcdrinfo_key_caller_small_photo_ = QLatin1String("getcdrinfo.key.caller_small_photo");
const QString CommandServiceOptions::getcdrinfo_key_caller_large_photo_ = QLatin1String("getcdrinfo.key.caller_large_photo");
const QString CommandServiceOptions::getcdrinfo_key_is_pstn_call_ = QLatin1String("getcdrinfo.key.is_pstn_call");
const QString CommandServiceOptions::getcdrinfo_key_call_country_ = QLatin1String("getcdrinfo.key.call_country");
const QString CommandServiceOptions::getcdrinfo_key_callee_ve_sys_id_ = QLatin1String("getcdrinfo.key.callee_ve_sys_id");
const QString CommandServiceOptions::getcdrinfo_key_callee_id_ = QLatin1String("getcdrinfo.key.callee_id");
const QString CommandServiceOptions::getcdrinfo_key_callee_ve_login_id_ = QLatin1String("getcdrinfo.key.callee_ve_login_id");
const QString CommandServiceOptions::getcdrinfo_key_callee_display_name_ = QLatin1String("getcdrinfo.key.callee_display_name");
const QString CommandServiceOptions::getcdrinfo_key_callee_mobile_ = QLatin1String("getcdrinfo.key.callee_mobile");
const QString CommandServiceOptions::getcdrinfo_key_callee_small_photo_ = QLatin1String("getcdrinfo.key.callee_small_photo");
const QString CommandServiceOptions::getcdrinfo_key_callee_large_photo_ = QLatin1String("getcdrinfo.key.callee_large_photo");
const QString CommandServiceOptions::getcdrinfo_key_partner_ve_sys_id_ = QLatin1String("getcdrinfo.key.partner_sys_id");
const QString CommandServiceOptions::getcdrinfo_key_partner_id_ = QLatin1String("getcdrinfo.key.partner_caller_id");
const QString CommandServiceOptions::getcdrinfo_key_partner_login_id_ = QLatin1String("getcdrinfo.key.partner_login_id");
const QString CommandServiceOptions::getcdrinfo_key_partner_display_name_ = QLatin1String("getcdrinfo.key.partner_display_name");
const QString CommandServiceOptions::getcdrinfo_key_partner_small_photo_ = QLatin1String("getcdrinfo.key.partner_small_photo");
const QString CommandServiceOptions::getcdrinfo_key_partner_large_photo_ = QLatin1String("getcdrinfo.key.partner_large_photo");
const QString CommandServiceOptions::getcdrinfo_key_is_free_ = QLatin1String("getcdrinfo.key.is_free");
const QString CommandServiceOptions::getcdrinfo_key_partner_level_ = QLatin1String("getcdrinfo.key.partner_level");
const QString CommandServiceOptions::getcdrinfo_key_language_1_ = QLatin1String("getcdrinfo.key.language_1");
const QString CommandServiceOptions::getcdrinfo_key_language_2_ = QLatin1String("getcdrinfo.key.language_2");
const QString CommandServiceOptions::getcdrinfo_key_call_time_ = QLatin1String("getcdrinfo.key.call_time");
const QString CommandServiceOptions::getcdrinfo_key_start_call_time_ = QLatin1String("getcdrinfo.key.start_call_time");
const QString CommandServiceOptions::getuserinfo_ = QLatin1String("getuserinfo");
const QString CommandServiceOptions::getuserinfo_url_ = QLatin1String("getuserinfo.url");
const QString CommandServiceOptions::getuserinfo_key_system_id_ = QLatin1String("getuserinfo.key.partner_sys_id");
const QString CommandServiceOptions::getuserinfo_key_caller_id_ = QLatin1String("getuserinfo.key.partner_caller_id");
const QString CommandServiceOptions::getuserinfo_key_login_id_ = QLatin1String("getuserinfo.key.partner_login_id");
const QString CommandServiceOptions::getuserinfo_key_secret_ = QLatin1String("getuserinfo.key.partner_secret");
const QString CommandServiceOptions::getuserinfo_key_display_name_ = QLatin1String("getuserinfo.key.partner_display_name");
const QString CommandServiceOptions::getuserinfo_key_gender_ = QLatin1String("getuserinfo.key.gender");
const QString CommandServiceOptions::getuserinfo_key_birthday_ = QLatin1String("getuserinfo.key.birthday");
const QString CommandServiceOptions::getuserinfo_key_nationality_ = QLatin1String("getuserinfo.key.nationality");
const QString CommandServiceOptions::getuserinfo_key_education_ = QLatin1String("getuserinfo.key.education");
const QString CommandServiceOptions::getuserinfo_key_major_ = QLatin1String("getuserinfo.key.major");
const QString CommandServiceOptions::getuserinfo_key_intro_ = QLatin1String("getuserinfo.key.intro_info");
const QString CommandServiceOptions::getuserinfo_key_skill_ = QLatin1String("getuserinfo.key.partner_skill");
const QString CommandServiceOptions::getuserinfo_key_skill_language_1_ = QLatin1String("getuserinfo.key.partner_skill.language_1");
const QString CommandServiceOptions::getuserinfo_key_skill_language_2_ = QLatin1String("getuserinfo.key.partner_skill.language_2");
const QString CommandServiceOptions::getuserinfo_key_skill_is_free_ = QLatin1String("getuserinfo.key.partner_skill.is_free");
const QString CommandServiceOptions::getuserinfo_key_skill_level_ = QLatin1String("getuserinfo.key.partner_skill.partner_level");
const QString CommandServiceOptions::getuserinfo_key_small_photo_ = QLatin1String("getuserinfo.key.partner_small_photo");
const QString CommandServiceOptions::getuserinfo_key_large_photo_ = QLatin1String("getuserinfo.key.partner_small_photo");
const QString CommandServiceOptions::getcalleruser_ = QLatin1String("getcalleruser");
const QString CommandServiceOptions::getcalleruser_url_ = QLatin1String("getcalleruser.url");
const QString CommandServiceOptions::getcalleruser_query_caller_id_ = QLatin1String("getcalleruser.query.caller_id");
const QString CommandServiceOptions::getcalleruser_key_small_photo_ = QLatin1String("getcalleruser.key.small_photo");
const QString CommandServiceOptions::getcalleruser_key_large_photo_ = QLatin1String("getcalleruser.key.large_photo");
const QString CommandServiceOptions::getcalleruser_key_display_name_ = QLatin1String("getcalleruser.key.display_name");
const QString CommandServiceOptions::getcalleruser_key_mobile_ = QLatin1String("getcalleruser.key.mobile");

const QString CommandServiceOptions::savescreenshot_ = QLatin1String("savescreenshot");
const QString CommandServiceOptions::savescreenshot_url_ = QLatin1String("savescreenshot.url");
const QString CommandServiceOptions::savescreenshot_query_meet_cdr_id_ = QLatin1String("savescreenshot.query.meet_cdr_id");
const QString CommandServiceOptions::savescreenshot_query_photo_ = QLatin1String("savescreenshot.query.photo");

const QString CommandServiceOptions::forgetpassword_ = QLatin1String("forgetpassword");
const QString CommandServiceOptions::forgetpassword_url_ = QLatin1String("forgetpassword.url");
const QString CommandServiceOptions::forgetpassword_query_user_name_ = QLatin1String("forgetpassword.query.user_name");
const QString CommandServiceOptions::createaccount_ = QLatin1String("createaccount");
const QString CommandServiceOptions::createaccount_url_ = QLatin1String("createaccount.url");
const QString CommandServiceOptions::changestatus_ = QLatin1String("changestatus");
const QString CommandServiceOptions::changestatus_url_ = QLatin1String("changestatus.url");
const QString CommandServiceOptions::changestatus_query_online_visible_status_ = QLatin1String("changestatus.query.online_visible_status");

const QString CommandServiceOptions::getblacklist_= QLatin1String("getblacklist");
const QString CommandServiceOptions::getblacklist_url_= QLatin1String("getblacklist.url");
const QString CommandServiceOptions::getblacklist_query_id_= QLatin1String("getblacklist.query.id");
const QString CommandServiceOptions::getblacklist_key_id_= QLatin1String("getblacklist.key.id");
const QString CommandServiceOptions::getblacklist_key_blacklist_id_= QLatin1String("getblacklist.key.black_list_id");
const QString CommandServiceOptions::getblacklist_key_partner_sys_id_= QLatin1String("getblacklist.key.partner_sys_id");
const QString CommandServiceOptions::getblacklist_key_ve_sys_id_= QLatin1String("getblacklist.key.ve_sys_id");
const QString CommandServiceOptions::getblacklist_key_display_name_= QLatin1String("getblacklist.key.display_name");
const QString CommandServiceOptions::getblacklist_key_small_photo_= QLatin1String("getblacklist.key.small_photo");
const QString CommandServiceOptions::addblacklist_= QLatin1String("addblacklist");
const QString CommandServiceOptions::addblacklist_url_= QLatin1String("addblacklist.url");
const QString CommandServiceOptions::addblacklist_query_ve_sys_id_= QLatin1String("addblacklist.query.ve_sys_id");
const QString CommandServiceOptions::addblacklist_key_id_= QLatin1String("getblacklist.key.id");
const QString CommandServiceOptions::addblacklist_key_blacklist_id_= QLatin1String("getblacklist.key.black_list_id");
const QString CommandServiceOptions::addblacklist_key_partner_sys_id_= QLatin1String("getblacklist.key.partner_sys_id");
const QString CommandServiceOptions::addblacklist_key_ve_sys_id_= QLatin1String("getblacklist.key.ve_sys_id");
const QString CommandServiceOptions::addblacklist_key_display_name_= QLatin1String("getblacklist.key.display_name");
const QString CommandServiceOptions::addblacklist_key_small_photo_= QLatin1String("getblacklist.key.small_photo");
const QString CommandServiceOptions::delblacklist_= QLatin1String("delblacklist");
const QString CommandServiceOptions::delblacklist_url_= QLatin1String("delblacklist.url");
const QString CommandServiceOptions::delblacklist_query_ve_sys_id_= QLatin1String("delblacklist.query.ve_sys_id");

const QString CommandServiceOptions::getmemberinfo_ = QLatin1String("getmemberinfo");
const QString CommandServiceOptions::getmemberinfo_url_ = QLatin1String("getmemberinfo.url");
const QString CommandServiceOptions::getmemberinfo_query_ve_sys_id_ = QLatin1String("getmemberinfo.query.ve_sys_id");
const QString CommandServiceOptions::getmemberinfo_key_ve_sys_id_ = QLatin1String("getmemberinfo.key.ve_sys_id");
const QString CommandServiceOptions::getmemberinfo_key_ve_login_id_ = QLatin1String("getmemberinfo.key.ve_login_id");
const QString CommandServiceOptions::getmemberinfo_key_caller_id_ = QLatin1String("getmemberinfo.key.caller_id");
const QString CommandServiceOptions::getmemberinfo_key_display_name_ = QLatin1String("getmemberinfo.key.display_name");
const QString CommandServiceOptions::getmemberinfo_key_native_language_ = QLatin1String("getmemberinfo.key.native_language");
const QString CommandServiceOptions::getmemberinfo_key_country_ = QLatin1String("getmemberinfo.key.country");
const QString CommandServiceOptions::getmemberinfo_key_major_ = QLatin1String("getmemberinfo.key.major");
const QString CommandServiceOptions::getmemberinfo_key_small_photo_ = QLatin1String("getmemberinfo.key.small_photo");

const QString CommandServiceOptions::getuploadphoto_ = QLatin1String("getuploadphoto");
const QString CommandServiceOptions::getuploadphoto_url_ = QLatin1String("getuploadphoto.url");
const QString CommandServiceOptions::getuploadphoto_query_caller_id_ = QLatin1String("getuploadphoto.query.caller_id");
const QString CommandServiceOptions::getuploadphoto_query_photo_id = QLatin1String("getuploadphoto.query.photo_id");
const QString CommandServiceOptions::getuploadphoto_key_photo_id = QLatin1String("getuploadphoto.key.photo_id");
const QString CommandServiceOptions::getuploadphoto_key_expire = QLatin1String("getuploadphoto.key.expire");
const QString CommandServiceOptions::getuploadphoto_key_small_photo_ = QLatin1String("getuploadphoto.key.small_photo");
const QString CommandServiceOptions::getuploadphoto_key_large_photo_ = QLatin1String("getuploadphoto.key.large_photo");

const QString CommandServiceOptions::version_ = QLatin1String("version");
const QString CommandServiceOptions::version_url_ = QLatin1String("version.url");
const QString CommandServiceOptions::version_key_number_ = QLatin1String("version.key.version_number");
const QString CommandServiceOptions::version_key_link_ = QLatin1String("version.key.download_link");
const QString CommandServiceOptions::phoneloadconfig_ = QLatin1String("loadconfig");
const QString CommandServiceOptions::phoneloadconfig_url_ = QLatin1String("loadconfig.url");
const QString CommandServiceOptions::phoneloadconfig_query_last_time_ = QLatin1String("loadconfig.query.last_time");
const QString CommandServiceOptions::phoneloadconfig_query_reload_ = QLatin1String("loadconfig.query.reload");
const QString CommandServiceOptions::phoneloadconfig_key_info_ = QLatin1String("loadconfig.key.info");
const QString CommandServiceOptions::phoneloadconfig_key_info_service_ = QLatin1String("loadconfig.key.info.service");
const QString CommandServiceOptions::phoneloadconfig_key_info_major_ = QLatin1String("loadconfig.key.info.major");
const QString CommandServiceOptions::phoneloadconfig_key_info_useapp_ = QLatin1String("loadconfig.key.info.useapp");
const QString CommandServiceOptions::phoneloadconfig_key_info_mode_ = QLatin1String("loadconfig.key.info.mode");
const QString CommandServiceOptions::phoneloadconfig_key_version_ = QLatin1String("loadconfig.key.version");
const QString CommandServiceOptions::phoneloadconfig_key_download_ = QLatin1String("loadconfig.key.download");
const QString CommandServiceOptions::phoneloadconfig_key_force_update_ = QLatin1String("loadconfig.key.force_update");
const QString CommandServiceOptions::phoneloadconfig_key_verify_code_ = QLatin1String("loadconfig.key.verify_code");
const QString CommandServiceOptions::phoneloadconfig_key_version_name_ = QLatin1String("loadconfig.key.version_name");
const QString CommandServiceOptions::phoneloadconfig_key_pstn_supported_ = QLatin1String("loadconfig.key.pstn_supported");
const QString CommandServiceOptions::phoneloadconfig_key_href_info_ = QLatin1String("loadconfig.key.pc_href_info");
const QString CommandServiceOptions::phoneloadconfig_key_href_info_create_account_ = QLatin1String("loadconfig.key.pc_href_info.create_account");
const QString CommandServiceOptions::phoneloadconfig_key_href_info_change_password_ = QLatin1String("loadconfig.key.pc_href_info.change_password");
const QString CommandServiceOptions::phoneloadconfig_key_href_info_edit_userinfo_ = QLatin1String("loadconfig.key.pc_href_info.edit_userinfo");
const QString CommandServiceOptions::phoneregister_ = QLatin1String("register");
const QString CommandServiceOptions::phoneregister_url_ = QLatin1String("register.url");
const QString CommandServiceOptions::phoneregister_query_native_language_ = QLatin1String("register.query.native_language");
const QString CommandServiceOptions::phoneregister_query_voicera_id_ = QLatin1String("register.query.voicera_id");
const QString CommandServiceOptions::phoneregister_query_email_ = QLatin1String("register.query.email");
const QString CommandServiceOptions::phoneregister_query_password_ = QLatin1String("register.query.password");
const QString CommandServiceOptions::phoneregister_query_photo_ = QLatin1String("register.query.photo");
const QString CommandServiceOptions::phoneregister_key_result_ = QLatin1String("register.key.result");
const QString CommandServiceOptions::phoneregister_key_result_sip_caller_id_ = QLatin1String("register.key.result.sip_caller_id");
const QString CommandServiceOptions::phoneregister_key_result_nickname_ = QLatin1String("register.key.result.nickname");
const QString CommandServiceOptions::phoneregister_key_result_voicera_id_ = QLatin1String("register.key.result.voiceraid");
const QString CommandServiceOptions::phoneregister_key_result_native_language_ = QLatin1String("register.key.result.native_language");
const QString CommandServiceOptions::phoneregister_key_result_small_photo_ = QLatin1String("register.key.result.small_photo");
const QString CommandServiceOptions::phoneregister_key_result_large_photo_ = QLatin1String("register.key.result.large_photo");
const QString CommandServiceOptions::phonelogin_ = QLatin1String("login");
const QString CommandServiceOptions::phonelogin_url_ = QLatin1String("login.url");
const QString CommandServiceOptions::phonelogin_query_voicera_id_ = QLatin1String("login.query.voicera_id");
const QString CommandServiceOptions::phonelogin_query_password_ = QLatin1String("login.query.password");
const QString CommandServiceOptions::phonelogin_key_result_ = QLatin1String("login.key.result");
const QString CommandServiceOptions::phonelogin_key_result_sip_caller_id_ = QLatin1String("login.key.result.sip_caller_id");
const QString CommandServiceOptions::phonelogin_key_result_native_language_ = QLatin1String("login.key.result.native_language");
const QString CommandServiceOptions::phonelogin_key_result_voicera_id_ = QLatin1String("login.key.result.voiceraid");
const QString CommandServiceOptions::phonelogin_key_result_small_photo_ = QLatin1String("login.key.result.small_photo");
const QString CommandServiceOptions::phonelogin_key_result_large_photo_ = QLatin1String("login.key.result.large_photo");
const QString CommandServiceOptions::phoneforgetpassword_ = QLatin1String("forgetpassword");
const QString CommandServiceOptions::phoneforgetpassword_url_ = QLatin1String("forgetpassword.url");
const QString CommandServiceOptions::phoneforgetpassword_query_email_ = QLatin1String("forgetpassword.query.email");
const QString CommandServiceOptions::phonecontactlist_ = QLatin1String("contactlist");
const QString CommandServiceOptions::phonecontactlist_url_ = QLatin1String("contactlist.url");
const QString CommandServiceOptions::phonecontactlist_query_search_type_ = QLatin1String("contactlist.query.search_type");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_ = QLatin1String("contactlist.key.contact_list");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_id_ = QLatin1String("contactlist.key.contact_list.id");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_add_book_id_ = QLatin1String("contactlist.key.contact_list.add_book_id");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_contact_name_ = QLatin1String("contactlist.key.contact_list.contact_name");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_small_photo_ = QLatin1String("contactlist.key.contact_list.small_photo");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_large_photo_ = QLatin1String("contactlist.key.contact_list.large_photo");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_phone_ = QLatin1String("contactlist.key.contact_list.phone");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_voicera_id_ = QLatin1String("contactlist.key.contact_list.voicera_id");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_sip_caller_id_ = QLatin1String("contactlist.key.contact_list.sip_caller_id");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_source_language_ = QLatin1String("contactlist.key.contact_list.source_language");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_target_language_ = QLatin1String("contactlist.key.contact_list.target_language");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_online_status_ = QLatin1String("contactlist.key.contact_list.online_status");
const QString CommandServiceOptions::phonecontactlist_key_contact_list_system_user_ = QLatin1String("contactlist.key.contact_list.system_user");
const QString CommandServiceOptions::phoneaddcontact_ = QLatin1String("addcontact");
const QString CommandServiceOptions::phoneaddcontact_url_ = QLatin1String("addcontact.url");
const QString CommandServiceOptions::phoneaddcontact_query_voicera_id_ = QLatin1String("addcontact.query.voicera_id");
const QString CommandServiceOptions::phoneaddcontact_query_name_ = QLatin1String("addcontact.query.name");
const QString CommandServiceOptions::phoneaddcontact_query_phone_ = QLatin1String("addcontact.query.phone");
const QString CommandServiceOptions::phoneaddcontact_query_phone_country_ = QLatin1String("addcontact.query.phone_country");
const QString CommandServiceOptions::phoneaddcontact_query_source_language_ = QLatin1String("addcontact.query.source_language");
const QString CommandServiceOptions::phoneaddcontact_query_target_language_ = QLatin1String("addcontact.query.target_language");
const QString CommandServiceOptions::phoneaddcontact_key_add_book_id_ = QLatin1String("addcontact.key.add_book_id");
const QString CommandServiceOptions::phoneaddcontact_key_id_ = QLatin1String("addcontact.key.id");
const QString CommandServiceOptions::phoneupdatecontact_ = QLatin1String("updatecontact");
const QString CommandServiceOptions::phoneupdatecontact_url_ = QLatin1String("updatecontact.url");
const QString CommandServiceOptions::phoneupdatecontact_query_add_book_id_ = QLatin1String("updatecontact.query.add_book_id");
const QString CommandServiceOptions::phoneupdatecontact_query_update_type_ = QLatin1String("updatecontact.query.update_type");
const QString CommandServiceOptions::phoneupdatecontact_query_voicera_id_ = QLatin1String("updatecontact.query.voicera_id");
const QString CommandServiceOptions::phoneupdatecontact_query_name_ = QLatin1String("updatecontact.query.name");
const QString CommandServiceOptions::phoneupdatecontact_query_phone_ = QLatin1String("updatecontact.query.phone");
const QString CommandServiceOptions::phoneupdatecontact_query_old_phone_ = QLatin1String("updatecontact.query.old_phone");
const QString CommandServiceOptions::phoneupdatecontact_query_phone_country_ = QLatin1String("updatecontact.query.phone_country");
const QString CommandServiceOptions::phoneupdatecontact_query_delete_phone_ = QLatin1String("updatecontact.query.delete_phone");
const QString CommandServiceOptions::phoneupdatecontact_query_source_language_ = QLatin1String("updatecontact.query.source_language");
const QString CommandServiceOptions::phoneupdatecontact_query_target_language_ = QLatin1String("updatecontact.query.target_language");
const QString CommandServiceOptions::phonedelcontact_ = QLatin1String("delcontact");
const QString CommandServiceOptions::phonedelcontact_url_ = QLatin1String("delcontact.url");
const QString CommandServiceOptions::phonedelcontact_query_add_book_id_ = QLatin1String("delcontact.query.add_book_id");
const QString CommandServiceOptions::phonedetailcontact_ = QLatin1String("detailcontact");
const QString CommandServiceOptions::phonedetailcontact_url_ = QLatin1String("detailcontact.url");
const QString CommandServiceOptions::phonedetailcontact_query_add_book_id_ = QLatin1String("detailcontact.query.add_book_id");
const QString CommandServiceOptions::phonedetailcontact_key_info_ = QLatin1String("detailcontact.key.info");
const QString CommandServiceOptions::phonedetailcontact_key_info_native_language_ = QLatin1String("detailcontact.key.info.native_language");
const QString CommandServiceOptions::phonedetailcontact_key_info_nickname_ = QLatin1String("detailcontact.key.info.nickname");
const QString CommandServiceOptions::phonedetailcontact_key_info_country_ = QLatin1String("detailcontact.key.info.country");
const QString CommandServiceOptions::phonedetailcontact_key_info_major_ = QLatin1String("detailcontact.key.info.major");
const QString CommandServiceOptions::phonegethistorycdr_ = QLatin1String("gethistorycdr");
const QString CommandServiceOptions::phonegethistorycdr_url_ = QLatin1String("gethistorycdr.url");
const QString CommandServiceOptions::phonegethistorycdr_query_history_type_ = QLatin1String("gethistorycdr.query.history_type");
const QString CommandServiceOptions::phonegethistorycdr_key_list_ = QLatin1String("gethistorycdr.key.list");
const QString CommandServiceOptions::phonegethistorycdr_key_list_id_ = QLatin1String("gethistorycdr.key.list.id");
const QString CommandServiceOptions::phonegethistorycdr_key_list_meet_cdr_id_ = QLatin1String("gethistorycdr.key.list.meet_cdr_id");
const QString CommandServiceOptions::phonegethistorycdr_key_list_callee_id_ = QLatin1String("gethistorycdr.key.list.callee_id");
const QString CommandServiceOptions::phonegethistorycdr_key_list_source_language_ = QLatin1String("gethistorycdr.key.list.source_language");
const QString CommandServiceOptions::phonegethistorycdr_key_list_target_language_ = QLatin1String("gethistorycdr.key.list.target_language");
const QString CommandServiceOptions::phonegethistorycdr_key_list_calling_time_ = QLatin1String("gethistorycdr.key.list.calling_time");
const QString CommandServiceOptions::phonegethistorycdr_key_list_cost_ = QLatin1String("gethistorycdr.key.list.cost");
const QString CommandServiceOptions::phonegethistorycdr_key_list_is_success_ = QLatin1String("gethistorycdr.key.list.is_success");
const QString CommandServiceOptions::phonegethistorycdr_key_list_is_callout_ = QLatin1String("gethistorycdr.key.list.is_callout");
const QString CommandServiceOptions::phonegetuserinfo_ = QLatin1String("getuserinfo");
const QString CommandServiceOptions::phonegetuserinfo_url_ = QLatin1String("getuserinfo.url");
const QString CommandServiceOptions::phonegetuserinfo_query_voice_id_ = QLatin1String("getuserinfo.query.voicera_id");
const QString CommandServiceOptions::phonegetuserinfo_key_info_ = QLatin1String("getuserinfo.key.info");
const QString CommandServiceOptions::phonegetuserinfo_key_info_email_ = QLatin1String("getuserinfo.key.info.email");
const QString CommandServiceOptions::phonegetuserinfo_key_info_voicera_id_ = QLatin1String("getuserinfo.key.info.voicera_id");
const QString CommandServiceOptions::phonegetuserinfo_key_info_sip_caller_id_ = QLatin1String("getuserinfo.key.info.sip_caller_id");
const QString CommandServiceOptions::phonegetuserinfo_key_info_account_balance_ = QLatin1String("getuserinfo.key.info.account_balance");
const QString CommandServiceOptions::phonegetuserinfo_key_info_nickname_ = QLatin1String("getuserinfo.key.info.nickname");
const QString CommandServiceOptions::phonegetuserinfo_key_info_native_language_ = QLatin1String("getuserinfo.key.info.native_language");
const QString CommandServiceOptions::phonegetuserinfo_key_info_major_ = QLatin1String("getuserinfo.key.info.major");
const QString CommandServiceOptions::phonegetuserinfo_key_info_country_ = QLatin1String("getuserinfo.key.info.country");
const QString CommandServiceOptions::phonegetuserinfo_key_info_small_photo_ = QLatin1String("getuserinfo.key.info.small_photo");
const QString CommandServiceOptions::phonegetuserinfo_key_info_large_photo_ = QLatin1String("getuserinfo.key.info.large_photo");
const QString CommandServiceOptions::phonegetuserinfo_key_info_online_status_ = QLatin1String("getuserinfo.key.info.online_status");
const QString CommandServiceOptions::phoneedituserinfo_ = QLatin1String("edituserinfo");
const QString CommandServiceOptions::phoneedituserinfo_url_ = QLatin1String("edituserinfo.url");
const QString CommandServiceOptions::phoneedituserinfo_query_nickname_ = QLatin1String("edituserinfo.query.nickname");
const QString CommandServiceOptions::phoneedituserinfo_query_native_language_ = QLatin1String("edituserinfo.query.native_language");
const QString CommandServiceOptions::phoneedituserinfo_query_nationality_ = QLatin1String("edituserinfo.query.nationality");
const QString CommandServiceOptions::phoneedituserinfo_query_major_ = QLatin1String("edituserinfo.query.major");
const QString CommandServiceOptions::phoneedituserinfo_query_online_visible_status_ = QLatin1String("edituserinfo.query.online_visible_status");
const QString CommandServiceOptions::phoneedituserinfo_query_photo_ = QLatin1String("edituserinfo.query.photo");
const QString CommandServiceOptions::phoneedituserinfo_query_update_type_ = QLatin1String("edituserinfo.query.update_type");
const QString CommandServiceOptions::phonechangepassword_ = QLatin1String("changepassword");
const QString CommandServiceOptions::phonechangepassword_url_ = QLatin1String("changepassword.url");
const QString CommandServiceOptions::phonechangepassword_query_auth_code_ = QLatin1String("changepassword.query.auth_code");
const QString CommandServiceOptions::phonechangepassword_query_email_ = QLatin1String("changepassword.query.email");
const QString CommandServiceOptions::phonenewpassword_ = QLatin1String("newpassword");
const QString CommandServiceOptions::phonenewpassword_url_ = QLatin1String("newpassword.url");
const QString CommandServiceOptions::phonenewpassword_query_new_password1_ = QLatin1String("newpassword.query.new_password1");
const QString CommandServiceOptions::phonenewpassword_query_new_password2_ = QLatin1String("newpassword.query.new_password2");
const QString CommandServiceOptions::phonenewpassword_query_auth_code_ = QLatin1String("newpassword.query.auth_code");
const QString CommandServiceOptions::phonenewpassword_query_email_ = QLatin1String("newpassword.query.email");
const QString CommandServiceOptions::phonegetcdrid_ = QLatin1String("getcdrid");
const QString CommandServiceOptions::phonegetcdrid_url_ = QLatin1String("getcdrid.url");
const QString CommandServiceOptions::phonegetcdrid_query_cdr_id_ = QLatin1String("getcdrid.query.cdr_id");
const QString CommandServiceOptions::phonegetcdrid_key_info_ = QLatin1String("getcdrid.key.info");
const QString CommandServiceOptions::phonegetcdrid_key_info_id_ = QLatin1String("getcdrid.key.info.id");
const QString CommandServiceOptions::phonegetcdrid_key_info_caller_id_ = QLatin1String("getcdrid.key.info.caller_id");
const QString CommandServiceOptions::phonegetcdrid_key_info_deduction_time_ = QLatin1String("getcdrid.key.info.deduction_time");
const QString CommandServiceOptions::phonegetcdrid_key_info_meet_cdr_id_ = QLatin1String("getcdrid.key.info.meet_cdr_id");
const QString CommandServiceOptions::phonegetcdrid_key_info_call_time_ = QLatin1String("getcdrid.key.info.call_time");
const QString CommandServiceOptions::phonegetcdrid_key_info_contact_name_ = QLatin1String("getcdrid.key.info.contact_name");
const QString CommandServiceOptions::phonegetcdrid_key_info_small_photo_ = QLatin1String("getcdrid.key.info.small_photo");
const QString CommandServiceOptions::phonegetcdrid_key_info_large_photo_ = QLatin1String("getcdrid.key.info.large_photo");
const QString CommandServiceOptions::phonegetcdrid_key_info_call_status_ = QLatin1String("getcdrid.key.info.call_status");
const QString CommandServiceOptions::phonegetcdrid_key_info_call_mode_ = QLatin1String("getcdrid.key.info.call_mode");
const QString CommandServiceOptions::phonegetcdrid_key_info_deduction_cost_ = QLatin1String("getcdrid.key.info.deduction_cost");
const QString CommandServiceOptions::phonegetcdrid_key_info_source_language_ = QLatin1String("getcdrid.key.info.source_language");
const QString CommandServiceOptions::phonegetcdrid_key_info_target_language_ = QLatin1String("getcdrid.key.info.target_language");
const QString CommandServiceOptions::phonegetcdrid_key_info_pstn_country_ = QLatin1String("getcdrid.key.info.pstn_country");
const QString CommandServiceOptions::phonegetcdrid_key_info_voicera_id_ = QLatin1String("getcdrid.key.info.voicera_id");
const QString CommandServiceOptions::phonedelcdr_ = QLatin1String("delcdr");
const QString CommandServiceOptions::phonedelcdr_url_ = QLatin1String("delcdr.url");
const QString CommandServiceOptions::phonedelcdr_query_meet_cdr_id_ = QLatin1String("delcdr.query.meet_cdr_id");

CommandServiceOptions::CommandServiceOptions()
{
}

const QString CommandServicePrivate::serviceHostPlaceholder = QLatin1String("{host}");
const QString CommandServicePrivate::tokenSeparator = QLatin1String("|");
const QString CommandServicePrivate::loadconfigTokenPrefix = QLatin1String("loadconfig:");
const QString CommandServicePrivate::loginTokenPrefix = QLatin1String("login:");
const QString CommandServicePrivate::serverTokenPrefix = QLatin1String("server:");
const QString CommandServicePrivate::setclientinfoTokenPrefix = QLatin1String("setclientinfo:");
const QString CommandServicePrivate::todaystayTokenPrefix = QLatin1String("todaystat:");
const QString CommandServicePrivate::getnoreportinfoTokenPrefix = QLatin1String("getnoreportinfo:");
const QString CommandServicePrivate::getcdrlistTokenPrefix = QLatin1String("getcdrlist:");
const QString CommandServicePrivate::getcdrinfoTokenPrefix = QLatin1String("getcdrinfo:");
const QString CommandServicePrivate::getuserinfoTokenPrefix = QLatin1String("getuserinfo:");
const QString CommandServicePrivate::getcalleruserTokenPrefix = QLatin1String("getcalleruser:");
const QString CommandServicePrivate::savescreenshotTokenPrefix = QLatin1String("savescreenshot:");
const QString CommandServicePrivate::changestatusTokenPrefix = QLatin1String("changestatus:");
const QString CommandServicePrivate::getblacklistTokenPrefix = QLatin1String("getblacklist:");
const QString CommandServicePrivate::addblacklistTokenPrefix = QLatin1String("addblacklist:");
const QString CommandServicePrivate::delblacklistTokenPrefix = QLatin1String("delblacklist:");
const QString CommandServicePrivate::getmemberinfoTokenPrefix = QLatin1String("getmemberinfo:");
const QString CommandServicePrivate::getuploadphotoTokenPrefix = QLatin1String("getuploadphoto:");
const QString CommandServicePrivate::versionTokenPrefix = QLatin1String("version:");

const QString CommandServicePrivate::phoneloadconfigTokenPrefix = QLatin1String("phoneloadconfig:");
const QString CommandServicePrivate::phoneregisterTokenPrefix = QLatin1String("phoneregister:");
const QString CommandServicePrivate::phoneloginTokenPrefix = QLatin1String("phonelogin:");
const QString CommandServicePrivate::phoneforgetpasswordTokenPrefix = QLatin1String("phoneforgetpassword:");
const QString CommandServicePrivate::phonecontactlistTokenPrefix = QLatin1String("phonecontactlist:");
const QString CommandServicePrivate::phoneaddcontactTokenPrefix = QLatin1String("phoneaddcontact:");
const QString CommandServicePrivate::phoneupdatecontactTokenPrefix = QLatin1String("phoneupdatecontact:");
const QString CommandServicePrivate::phonedelcontactTokenPrefix = QLatin1String("phonedelcontact:");
const QString CommandServicePrivate::phonedetailcontactTokenPrefix = QLatin1String("phonedetailcontact:");
const QString CommandServicePrivate::phonegethistorycdrTokenPrefix = QLatin1String("phonegethistorycdr:");
const QString CommandServicePrivate::phonegetuserinfoTokenPrefix = QLatin1String("phonegetuserinfo:");
const QString CommandServicePrivate::phoneedituserinfoTokenPrefix = QLatin1String("phoneedituserinfo:");
const QString CommandServicePrivate::phonechangepasswordTokenPrefix = QLatin1String("phonechangepassword:");
const QString CommandServicePrivate::phonenewpasswordTokenPrefix = QLatin1String("phonenewpassword:");
const QString CommandServicePrivate::phonegetcdridTokenPrefix = QLatin1String("phonegetcdrid:");
const QString CommandServicePrivate::phonedelcdrTokenPrefix = QLatin1String("phonedelcdr:");

CommandServicePrivate::CommandServicePrivate(ServiceManager* manager, const QString& name) :
    AbstractServicePrivate(manager, name),
    m_commandQueue(0)
{
}

CommandServicePrivate::~CommandServicePrivate()
{
    cleanup();
}

void CommandServicePrivate::init()
{
    Q_Q(CommandService);
    Q_ASSERT(q);
    Q_ASSERT(m_manager);

    m_commandQueue = new DownloadQueue(q);

    QNetworkAccessManager* networkAccessManager = m_manager->networkAccessManager();
    m_commandQueue->setNetworkAccessManager(networkAccessManager);

    QObject::connect(m_commandQueue, SIGNAL(finished(QString, DownloadResult)),
        q, SLOT(_q_commandFinished(QString, DownloadResult)));
}

void CommandServicePrivate::startup()
{
    qsrand(QDateTime::currentDateTime().toTime_t());
}

void CommandServicePrivate::cleanup()
{
    m_commandQueue->clear();

    AbstractServicePrivate::cleanup();
}

QList<QNetworkCookie> CommandServicePrivate::cookies() const
{
	return m_cookies;
}

void CommandServicePrivate::setCookies(const QList<QNetworkCookie>& cookies)
{
	m_cookies = cookies;
	qDebug() << "setCookies" << m_cookies;
}

QString CommandServicePrivate::token() const 
{
	return m_token;
}

void CommandServicePrivate::setToken(const QString& token)
{
	m_token = token;
}

void CommandServicePrivate::loadconfig(const Command::LoadConfigParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	qDebug() << m_userService->userServiceHost() << m_opt.loadconfig.url;

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.loadconfig.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.loadconfig.query.lastTime, param.lastTime());
	//addQueryItem(&url, m_opt.loadconfig.query.reload, QString::number(param.reloadConfig()));
	url.addQueryItem(m_opt.loadconfig.query.lastTime, param.lastTime());
	url.addQueryItem(m_opt.loadconfig.query.reload, QString::number(param.reloadConfig()));

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()));

	request.setUrl(url);

	m_commandQueue->setCookies(url);

	const QString token = makeToken(loadconfigTokenPrefix, param.lastTime(), QString::number(param.reloadConfig()));
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::getserver(const Command::GetServerParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	qDebug() << m_userService->userServiceHost() << m_opt.getserver.url;

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.getserver.url));
	addCommonQueryItem(&url);

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	qDebug() << m_cookies;

	//const QString token = makeToken(serverTokenPrefix, param.userName(), param.password());
	//	m_commandQueue->setCookies(m_commandQueue->cookies(), url);
	m_commandQueue->setCookies(url);
	m_commandQueue->add(serverTokenPrefix, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << serverTokenPrefix;
}

void CommandServicePrivate::login(const Command::LoginParam& param)
{
    Q_ASSERT(isActive());

    setError(Service::NoError);

//     const QByteArray key = m_opt.login.encryptKey.toUtf8();
//     const QByteArray userName = encryptUserName(param.userName().toUtf8(), key);
//     const QByteArray password = param.password().toUtf8();//encryptPassword(param.password().toUtf8(), key);

	qDebug() << m_userService->userServiceHost() << m_opt.login.url;

    QUrl url;
	url.setUrl(makeFullUrl(m_opt.login.url));
	addCommonQueryItem(&url);
	url.addQueryItem(m_opt.login.query.userName, param.userName());
	url.addQueryItem(m_opt.login.query.password, param.password());
	//addQueryItem(&url, m_opt.login.query.userName, param.userName());
	//addQueryItem(&url, m_opt.login.query.password, param.password());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()));
    request.setUrl(url);

	qDebug() << m_cookies;

    const QString token = makeToken(loginTokenPrefix, param.userName(), param.password());
//	m_commandQueue->setCookies(m_commandQueue->cookies(), url);
	m_commandQueue->setCookies(url);
    m_commandQueue->add(token, request);

    qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::login2(const Command::LoginParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QString loginUrl = m_opt.login.url /*+ QLatin1String("2")*/;
	qDebug() << m_userService->userServiceHost() << loginUrl;

	QUrl url;
	url.setUrl(makeFullUrl(loginUrl));
	addCommonQueryItem(&url);
	url.addQueryItem(m_opt.login.query.userName, param.userName());
	url.addQueryItem(m_opt.login.query.password, param.password());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService) {
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()));
		m_commandQueue->setSystemId(m_userService->systemId());
	}
	request.setUrl(url);

	qDebug() << m_cookies;

	const QString token = makeToken(loginTokenPrefix, param.userName(), param.password());
	//	m_commandQueue->setCookies(m_commandQueue->cookies(), url);
	m_commandQueue->setToken(m_token);
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::setclientinfo(const Command::SetClientInfoParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QString setclientinfoUrl = m_opt.setclientinfo.url;
	qDebug() << m_userService->userServiceHost() << setclientinfoUrl;

	QUrl url;
	url.setUrl(makeFullUrl(setclientinfoUrl));
	addCommonQueryItem(&url);
	url.addQueryItem(m_opt.setclientinfo.query.lastDomainCall, param.lastDomainCall());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(setclientinfoTokenPrefix, param.lastDomainCall());
	//	m_commandQueue->setCookies(m_commandQueue->cookies(), url);
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::todaystat(const Command::TodayStatParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.todaystat.url));
	addCommonQueryItem(&url);

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	m_commandQueue->setCookies(url);
	m_commandQueue->add(todaystayTokenPrefix, request);
}

void CommandServicePrivate::getnoreportinfo(const Command::GetNoReportParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.getnoreportinfo.url));
	addCommonQueryItem(&url);

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	m_commandQueue->setCookies(url);
	m_commandQueue->add(getnoreportinfoTokenPrefix, request);
}

void CommandServicePrivate::getcdrlist(const Command::GetCDRListParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.getcdrlist.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.getcdrlist.query.id, param.Id());
	//addQueryItem(&url, m_opt.getcdrlist.query.type, param.type());
	url.addQueryItem(m_opt.getcdrlist.query.id, param.Id());
	url.addQueryItem(m_opt.getcdrlist.query.type, param.type());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(getcdrlistTokenPrefix, param.Id(), param.type());
//	m_commandQueue->setCookies(m_cookies, url);
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::getcdrinfo(const Command::GetCDRInfoParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.getcdrinfo.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.getcdrinfo.query.meetCDRId, param.meetCDRId());
	url.addQueryItem(m_opt.getcdrinfo.query.meetCDRId, param.meetCDRId());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(getcdrinfoTokenPrefix, param.meetCDRId());
	//m_commandQueue->setCookies(m_cookies, url);
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::getuserinfo(const Command::GetUserInfoParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.getuserinfo.url));
	addCommonQueryItem(&url);

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(getuserinfoTokenPrefix, QString());
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::getcalleruser(const Command::GetCallerUserParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.getcalleruser.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.getcalleruser.query.callerId, param.callerId());
	url.addQueryItem(m_opt.getcalleruser.query.callerId, param.callerId());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(getcalleruserTokenPrefix, param.callerId());
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::savescreenshot(const Command::SaveScreenShotParam& param)
{
	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.savescreenshot.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.savescreenshot.query.meetCDRId, param.meetCDRId());
	url.addQueryItem(m_opt.savescreenshot.query.meetCDRId, param.meetCDRId());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(savescreenshotTokenPrefix, param.meetCDRId(), param.photo());
	m_commandQueue->setCookies(url);

	if (!param.photo().isEmpty()) {
		QFile *photoFile = new QFile(param.photo());
		if (photoFile->open(QFile::ReadOnly)) {

			QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
			QHttpPart imagePart;
			imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QLatin1String("image/jpeg")));
			imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"photo\"; filename=\"photo.jpg\"")));
			imagePart.setBodyDevice(photoFile);
			multiPart->append(imagePart);
			photoFile->setParent(multiPart);

			m_commandQueue->add(token, request, multiPart);

		} else {
			setError(Service::InvalidArgumentError);
		}
	} else {
		m_commandQueue->add(token, request);
	}

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::forgetpassword(const Command::ForgetPasswordParam& param)
{
//    Q_ASSERT(isActive());

    setError(Service::NoError);

    QUrl url;
	url.setUrl(makeFullUrl(m_opt.forgetpassword.url));
	addCommonQueryItem(&url);
	url.addQueryItem(m_opt.forgetpassword.query.userName, param.userName());

	QDesktopServices::openUrl(url);
}

void CommandServicePrivate::createaccount(const Command::CreateAccountParam& param)
{
//	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.createaccount.url));
	addCommonQueryItem(&url);

	QDesktopServices::openUrl(url);
}

void CommandServicePrivate::changestatus(const Command::ChangeStatusParam& param)
{
    Q_ASSERT(isActive());

    setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.changestatus.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.changestatus.query.onlineVisibleStatus, param.onlineVisibleStatus());
	url.addQueryItem(m_opt.changestatus.query.onlineVisibleStatus, param.onlineVisibleStatus());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
    request.setUrl(url);

    const QString token = makeToken(changestatusTokenPrefix, param.onlineVisibleStatus());
	m_commandQueue->setCookies(url);
    m_commandQueue->add(token, request);

    qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::getblacklist(const Command::GetBlacklistParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.getblacklist.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.getblacklist.query.id, param.id());
	url.addQueryItem(m_opt.getblacklist.query.id, param.id());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	//request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");

	request.setUrl(url);

	const QString token = makeToken(getblacklistTokenPrefix, param.id());
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::addblacklist(const Command::AddBlacklistParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.addblacklist.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.addblacklist.query.VESysId, param.VESysId());
	url.addQueryItem(m_opt.addblacklist.query.VESysId, param.VESysId());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(addblacklistTokenPrefix, param.VESysId());
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::delblacklist(const Command::DelBlacklistParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.delblacklist.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.delblacklist.query.VESysId, param.VESysId());
	url.addQueryItem(m_opt.delblacklist.query.VESysId, param.VESysId());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(delblacklistTokenPrefix, param.VESysId());
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::getmemberinfo(const Command::GetMemberInfoParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.getmemberinfo.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.getmemberinfo.query.VESysId, param.VESysId());
	url.addQueryItem(m_opt.getmemberinfo.query.VESysId, param.VESysId());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(getmemberinfoTokenPrefix, param.VESysId());
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::getuploadphoto(const Command::GetUploadPhotoParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.getuploadphoto.url));
	addCommonQueryItem(&url);
	//addQueryItem(&url, m_opt.getuploadphoto.query.callerId, param.callerId());
	//addQueryItem(&url, m_opt.getuploadphoto.query.photoId, param.photoId());
	url.addQueryItem(m_opt.getuploadphoto.query.callerId, param.callerId());
	url.addQueryItem(m_opt.getuploadphoto.query.photoId, param.photoId());

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(getuploadphotoTokenPrefix, param.callerId(), param.photoId());
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::version(const Command::VersionParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.version.url));
	addCommonQueryItem(&url);
	addSessionIdQueryItem(&url);

	QNetworkRequest request;
	addCommonRequestHeaders(request);
	if (m_userService)
		request.setRawHeader("Authorization","Basic " + QByteArray(QString(QLatin1String("%1:%2")).arg(m_userService->systemId()).arg(m_token).toAscii()).toBase64());
	request.setUrl(url);

	const QString token = makeToken(versionTokenPrefix, QLatin1String(" "));
	m_commandQueue->setCookies(url);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phoneloadconfig(const Command::PhoneLoadConfigParam& param)
{
//	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phoneloadconfig.url));
	addCommonQueryItem(&url);
	addQueryItem(&url, m_opt.phoneloadconfig.query.lastTime, param.lastTime());
	addQueryItem(&url, m_opt.phoneloadconfig.query.reload, QString::number(param.reloadConfig()));
	// 	url.addQueryItem(m_opt.loadconfig.query.lastTime, param.lastTime());
	// 	url.addQueryItem(m_opt.loadconfig.query.reload, QString::number(param.reloadConfig()));

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phoneloadconfigTokenPrefix, param.lastTime(), QString::number(param.reloadConfig()));
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phoneregister(const Command::PhoneRegisterParam& param)
{
//	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phoneregister.url));
	addQueryItem(&url, m_opt.phoneregister.query.nativeLanguage, param.nativeLanguage());
	addQueryItem(&url, m_opt.phoneregister.query.voiceraID, param.voiceraID());
	addQueryItem(&url, m_opt.phoneregister.query.email, param.email());
	addQueryItem(&url, m_opt.phoneregister.query.password, param.password());
//	addQueryItem(&url, m_opt.phoneregister.query.photo, param.photo());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phoneregisterTokenPrefix, param.nativeLanguage(), param.voiceraID(), param.email());

	if (!param.photo().isEmpty()) {
		QFile *photoFile = new QFile(param.photo());
		if (photoFile->open(QFile::ReadOnly)) {

			QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
			QHttpPart imagePart;
			imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QLatin1String("image/jpeg")));
			imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QLatin1String("form-data; name=\"photo\"; filename=\"photo.jpg\"")));
			imagePart.setBodyDevice(photoFile);
			multiPart->append(imagePart);
			photoFile->setParent(multiPart);

			m_commandQueue->add(token, request, multiPart);

		} else {
			setError(Service::InvalidArgumentError);
		}
	} else {
		m_commandQueue->add(token, request);
	}

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonelogin(const Command::PhoneLoginParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonelogin.url));
	addQueryItem(&url, m_opt.phonelogin.query.voiceraID, param.voiceraID());
	addQueryItem(&url, m_opt.phonelogin.query.password, param.password());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phoneloginTokenPrefix, param.voiceraID(), param.password());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phoneforgetpassword(const Command::PhoneForgetPasswordParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phoneforgetpassword.url));
	addQueryItem(&url, m_opt.phoneforgetpassword.query.email, param.email());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phoneforgetpasswordTokenPrefix, param.email());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonecontactlist(const Command::PhoneContactListParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonecontactlist.url));
	addQueryItem(&url, m_opt.phonecontactlist.query.searchType, param.searchType());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phonecontactlistTokenPrefix, param.searchType());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phoneaddcontact(const Command::PhoneAddContactParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phoneaddcontact.url));
	addQueryItem(&url, m_opt.phoneaddcontact.query.voiceraID, param.voiceraID());
	addQueryItem(&url, m_opt.phoneaddcontact.query.name, param.name());
	addQueryItem(&url, m_opt.phoneaddcontact.query.phone, param.phone());
	addQueryItem(&url, m_opt.phoneaddcontact.query.phoneCountry, param.phoneCountry());
	addQueryItem(&url, m_opt.phoneaddcontact.query.sourceLanguage, param.sourceLanguage());
	addQueryItem(&url, m_opt.phoneaddcontact.query.targetLanguage, param.targetLanguage());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phoneaddcontactTokenPrefix, param.voiceraID(), param.name(), param.phone());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phoneupdatecontact(const Command::PhoneUpdateContactParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phoneupdatecontact.url));
	addQueryItem(&url, m_opt.phoneupdatecontact.query.addBookID, param.addBookID());
	addQueryItem(&url, m_opt.phoneupdatecontact.query.updateType, param.updateType());
	addQueryItem(&url, m_opt.phoneupdatecontact.query.voiceraID, param.voiceraID());
	addQueryItem(&url, m_opt.phoneupdatecontact.query.name, param.name());
	addQueryItem(&url, m_opt.phoneupdatecontact.query.phone, param.phone());
	addQueryItem(&url, m_opt.phoneupdatecontact.query.oldPhone, param.oldPhone());
	addQueryItem(&url, m_opt.phoneupdatecontact.query.phoneCountry, param.phoneCountry());
	addQueryItem(&url, m_opt.phoneupdatecontact.query.deletePhone, param.deletePhone());
	addQueryItem(&url, m_opt.phoneupdatecontact.query.sourceLanguage, param.sourceLanguage());
	addQueryItem(&url, m_opt.phoneupdatecontact.query.targetLanguage, param.targetLanguage());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	QStringList args;
	args << param.addBookID() << param.updateType() 
		<< param.oldPhone() << param.phoneCountry() << param.phone() 
		<< param.deletePhone() 
		<< param.voiceraID() 
		<< param.name() 
		<< param.sourceLanguage() << param.targetLanguage();

	const QString token = makeToken(phoneupdatecontactTokenPrefix, args);
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonedelcontact(const Command::PhoneDeleteContactParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonedelcontact.url));
	addQueryItem(&url, m_opt.phonedelcontact.query.addBookID, param.addBookID());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phonedelcontactTokenPrefix, param.addBookID());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonedetailcontact(const Command::PhoneDetailContactParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonedetailcontact.url));
	addQueryItem(&url, m_opt.phonedetailcontact.query.addBookID, param.addBookID());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phonedetailcontactTokenPrefix, param.addBookID());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonegethistorycdr(const Command::PhoneGetHistoryCDRParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonegethistorycdr.url));
	addQueryItem(&url, m_opt.phonegethistorycdr.query.historyType, param.historyType());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phonegethistorycdrTokenPrefix, param.historyType());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonegetuserinfo(const Command::PhoneGetUserInfoParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonegetuserinfo.url));
	addQueryItem(&url, m_opt.phonegetuserinfo.query.voiceraID, param.voiceraID());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phonegetuserinfoTokenPrefix, param.voiceraID());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phoneedituserinfo(const Command::PhoneEditUserInfoParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phoneedituserinfo.url));
	addQueryItem(&url, m_opt.phoneedituserinfo.query.nickname, param.nickname());
	addQueryItem(&url, m_opt.phoneedituserinfo.query.nativeLanguage, param.nativeLanguage());
	addQueryItem(&url, m_opt.phoneedituserinfo.query.nationality, param.nationlity());
	addQueryItem(&url, m_opt.phoneedituserinfo.query.major, param.major());
	addQueryItem(&url, m_opt.phoneedituserinfo.query.onlineVisibleStatus, param.onlineStatus());
	addQueryItem(&url, m_opt.phoneedituserinfo.query.photo, param.photo());
	addQueryItem(&url, m_opt.phoneedituserinfo.query.updateType, param.updateType());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phoneedituserinfoTokenPrefix, param.updateType(), param.nickname(), param.nativeLanguage());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonechangepassword(const Command::PhoneChangePasswordParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonechangepassword.url));
	addQueryItem(&url, m_opt.phonechangepassword.query.authCode, param.authCode());
	addQueryItem(&url, m_opt.phonechangepassword.query.email, param.email());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phonechangepasswordTokenPrefix, param.authCode(), param.email());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonenewpassword(const Command::PhoneNewPasswordParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonenewpassword.url));
	addQueryItem(&url, m_opt.phonenewpassword.query.newPassword1, param.newPassword1());
	addQueryItem(&url, m_opt.phonenewpassword.query.newPassword2, param.newPassword2());
	addQueryItem(&url, m_opt.phonenewpassword.query.authCode, param.authCode());
	addQueryItem(&url, m_opt.phonenewpassword.query.email, param.email());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phonenewpasswordTokenPrefix, param.newPassword1(), param.newPassword2(), param.authCode());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonegetcdrid(const Command::PhoneGetCDRIDParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonegetcdrid.url));
	addQueryItem(&url, m_opt.phonegetcdrid.query.CDRID, param.CDRID());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phonegetcdridTokenPrefix, param.CDRID());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void  CommandServicePrivate::phonedelcdr(const Command::PhoneDeleteCDRParam& param)
{
	Q_ASSERT(isActive());

	setError(Service::NoError);

	QUrl url;
	url.setUrl(makeFullUrl(m_opt.phonedelcdr.url));
	addQueryItem(&url, m_opt.phonedelcdr.query.meetCDRID, param.meetCDRID());

	QNetworkRequest request;
	request.setRawHeader("Authorization", "Basic dGVzdDo1MTc5");
	request.setUrl(url);

	const QString token = makeToken(phonedelcdrTokenPrefix, param.meetCDRID());
	m_commandQueue->add(token, request);

	qDebug() << DEBUG_PREFIX << "Command submitted" << token;
}

void CommandServicePrivate::_q_commandFinished(const QString& token, const DownloadResult& result)
{
    Q_Q(CommandService);

    if (!isActive())
        return;

    QVariantMap record;
	Command::CommonResult commonResult;
    do {
       if (result.error) {
//             setError(result.error, result.errorReason);
//             break;
		   setHttpError(result.error, token, result.errorReason);
		   return;
        }

		qDebug() << DEBUG_PREFIX << "Command Response" << result.data;

		QJson::Parser parser;
        bool ok = false;
        const QVariant data = parser.parse(result.data, &ok);

        if (!ok) {
            qCritical() << DEBUG_PREFIX << "Failed to parse result" << parser.errorString();
            setError(Service::ResponseSerializationFailedError, parser.errorString());
            break;
        }

        if (!data.isValid() || !data.canConvert<QVariantMap>()) {
            qCritical() << DEBUG_PREFIX << "Failed to parse result to map";
            setError(Service::ResponseSerializationFailedError);
            break;
        }

        const QVariantMap dataMap = data.toMap();

		commonResult.setError(dataMap.value(m_opt.common.key.errorCode).toString());
		commonResult.setErrorMsg(dataMap.value(m_opt.common.key.errorMessage).toString());
		commonResult.setErrorNum(dataMap.value(m_opt.common.key.errorNumber).toString());
		commonResult.setErrorTips(dataMap.value(m_opt.common.key.errorTips).toString());

        record = dataMap;

    } while (false);

	//loadconfig
	if (token.startsWith(loadconfigTokenPrefix)) {
		
		const QStringList args = extractToken(token, loadconfigTokenPrefix);
		if (args.size() == 2) {
			Command::LoadConfigParam param;
			param.setLastTime(args[0]);
			param.setReloadConfig(args[1].toInt());

			Command::LoadConfigResult result;
			result.setCommon(commonResult);

			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				record = data.toMap();

				result.setConfig(record.value(m_opt.loadconfig.key.config));
				result.setClient(record.value(m_opt.loadconfig.key.client));
			}
			
			Q_EMIT q->loadconfigFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}

	}
	//server
	else if (token.startsWith(serverTokenPrefix)) {
		
		Command::GetServerParam param;

		Command::GetServerResult result;
		result.setCommon(commonResult);

		QVariant data = record.value(m_opt.common.key.result);
		if (data.isValid() && data.canConvert<QVariantMap>()) {
			record = data.toMap();
			result.setDomainStatus(record.value(m_opt.getserver.key.domain_status).toString());
			result.setDomainCall(record.value(m_opt.getserver.key.domain_call).toString());
			result.setDomainIM(record.value(m_opt.getserver.key.domain_im).toString());

			QStringList serverList;
			QVariantMap map = data.toMap();
			QVariant serverVariantList = map.value(QLatin1String("server_list"));
			if (serverVariantList.isValid() && serverVariantList.canConvert<QVariantList>()) {
				foreach(const QVariant& s, serverVariantList.toList()) {
					if(s.isValid() && s.canConvert<QVariantMap>()) {
						QVariantMap sMap = s.toMap();
						serverList.append(sMap.value(m_opt.login.key.serverIpInfo.domainCall).toString());
					}
				}
				result.setSipServerList(serverList);
			}

			Q_EMIT q->getserverFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}

	}
	//todaystat
	else if (token.startsWith(todaystayTokenPrefix)){
		Command::TodayStatParam param;

		Command::TodayStatResult result;
		result.setCommon(commonResult);

		QVariant data = record.value(m_opt.common.key.result);
		if (data.isValid() && data.canConvert<QVariantMap>()) {
			record = data.toMap();
			result.setAnswer(record.value(m_opt.todaystat.key.answer).toString());
			result.setSuccess(record.value(m_opt.todaystat.key.success).toString());
			result.setNoAnswer(record.value(m_opt.todaystat.key.noanswer).toString());

			Q_EMIT q->todayStatFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	//setclientinfo
	else if(token.startsWith(setclientinfoTokenPrefix)) {
		Command::SetClientInfoParam param;
		const QStringList args = extractToken(token, setclientinfoTokenPrefix);
		if (args.size() == 1) {
			param.setLastDomainCall(args[0]);
		}

		Command::SetClientInfoResult result;
		result.setCommon(commonResult);

		Q_EMIT q->setclientinfoFinished(param, result);
		return;
	}
    // login
    else if (token.startsWith(loginTokenPrefix)) {

		m_cookies = m_commandQueue->cookies();
        const QStringList args = extractToken(token, loginTokenPrefix);
        if (args.size() == 2) {
            Command::LoginParam param;
            param.setUserName(args[0]);
            param.setPassword(args[1]);

            Command::LoginResult result;	
			result.setCommon(commonResult);
			
			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				QVariantMap serverMap = data.toMap();
				QVariant serverData = serverMap.value(m_opt.login.key.serverIp);
				if (serverData.isValid() && serverData.canConvert<QVariantMap>()) {
					QVariantMap serverMap = serverData.toMap();

// 					result.setStatusServiceHost(serverMap.value(m_opt.login.key.serverIpInfo.domainStatus).toString());
// 					result.setSipServiceHost(serverMap.value(m_opt.login.key.serverIpInfo.domainCall).toString());
					result.setImServiceHost(serverMap.value(m_opt.login.key.serverIpInfo.domainIm).toString());

				}
// 				QStringList serverList;
// 				QVariantMap map = data.toMap();
// 				QVariant serverVariantList = map.value(QLatin1String("server_list"));
// 				if (serverVariantList.isValid() && serverVariantList.canConvert<QVariantList>()) {
// 					foreach(const QVariant& s, serverVariantList.toList()) {
// 						if(s.isValid() && s.canConvert<QVariantMap>()) {
// 							QVariantMap sMap = s.toMap();
// 							serverList.append(sMap.value(m_opt.login.key.serverIpInfo.domainCall).toString());
// 						}
// 					}
// 					result.setSipServerList(serverList);
// 				}
				QVariantMap dataMap = data.toMap();
				QVariant user = dataMap.value(m_opt.login.key.user);
				if (user.isValid() && user.canConvert<QVariantMap>()) {
					record = user.toMap();

					result.setSystemId(record.value(m_opt.login.key.userInfo.systemId).toString());
					result.setCallerId(record.value(m_opt.login.key.userInfo.callerId).toString());
					result.setLoginId(record.value(m_opt.login.key.userInfo.loginId).toString());
					result.setSecret(record.value(m_opt.login.key.userInfo.secret).toString());
					result.setDisplayName(record.value(m_opt.login.key.userInfo.displayName).toString());
					result.setSmallPhoto(record.value(m_opt.login.key.userInfo.smallPhoto).toString());
					result.setLargePhoto(record.value(m_opt.login.key.userInfo.largePhoto).toString());
					result.setToken(record.value(m_opt.login.key.userInfo.token).toString());

					QStringList skill;
					QVariant skillData = record.value(m_opt.login.key.userInfo.skill);
					if (skillData.isValid() && skillData.canConvert<QVariantList>()) {
						foreach(const QVariant& skl, skillData.toList()) {
							if (skl.isValid() && skl.canConvert<QVariantMap>()) {
								QVariantMap sklMap = skl.toMap();
								skill.append(QString::fromLatin1("%1|%2|%3|%4").arg(
									sklMap.value(m_opt.login.key.userInfo.skillInfo.language1).toString(),
									sklMap.value(m_opt.login.key.userInfo.skillInfo.language2).toString(),
									sklMap.value(m_opt.login.key.userInfo.skillInfo.isFree).toString(),
									sklMap.value(m_opt.login.key.userInfo.skillInfo.level).toString()
									)); 
							}
						}
					}
					result.setSkill(skill);

					QStringList localList;
					QVariant localData = record.value(m_opt.login.key.userInfo.local);
					if(localData.isValid() && localData.canConvert<QVariantList>()) {
						foreach(const QVariant& local, localData.toList()) {
							if(local.isValid() && local.canConvert<QVariantMap>()) {
								QVariantMap localMap = local.toMap();
								localList.append(QString::fromLatin1("%1|%2").arg(
									localMap.value(m_opt.login.key.userInfo.localInfo.country).toString(),
									localMap.value(m_opt.login.key.userInfo.localInfo.language).toString()
									));
							}
						}
					}
					result.setLocal(localList);
				}								
				Q_EMIT q->loginFinished(param, result);
				return;
			}
        } else {
            Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
        }
    }
	//getnoreportinfo
	else if (token.startsWith(getnoreportinfoTokenPrefix)) {
		Command::GetNoReportParam param;

		Command::GetNoReportResult result;
		result.setCommon(commonResult);

		QVariant data = record.value(m_opt.common.key.result);
		if (data.isValid() && data.canConvert<QVariantMap>()) {
			record = data.toMap();
			result.setNum(record.value(m_opt.getnoreportinfo.key.num).toString());

			Q_EMIT q->getnoreportinfoFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// getcdrlist
	else if (token.startsWith(getcdrlistTokenPrefix)) {
		const QStringList args = extractToken(token, getcdrlistTokenPrefix);
		if (args.size() == 2) {
			Command::GetCDRListParam param;
			param.setId(args[0]);
			param.setType(args[1]);

			Command::GetCDRListResult result;	
			result.setCommon(commonResult);

			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				record = data.toMap();

				QVariant cdrList = record.value(QLatin1String("list"));
				if (cdrList.isValid() && cdrList.canConvert<QVariantList>()) {
					foreach(const QVariant& cdr, cdrList.toList()) {
						if (cdr.isValid() && cdr.canConvert<QVariantMap>()) {
							QVariantMap cdrMap = cdr.toMap();

							result.setId(cdrMap.value(m_opt.getcdrlist.key.id).toString());
							result.setVESysId(cdrMap.value(m_opt.getcdrlist.key.VESysId).toString());
							result.setMeetCDRId(cdrMap.value(m_opt.getcdrlist.key.meetCDRId).toString());
							result.setSmallPhoto(cdrMap.value(m_opt.getcdrlist.key.smallPhoto).toString());
							result.setDisplayName(cdrMap.value(m_opt.getcdrlist.key.displayName).toString());
							result.setStartCallTime(cdrMap.value(m_opt.getcdrlist.key.startCallTime).toString());
							result.setCallTime(cdrMap.value(m_opt.getcdrlist.key.callTime).toString());
							result.setIsAnswered(cdrMap.value(m_opt.getcdrlist.key.isAnswered).toString());
							result.setCallMode(cdrMap.value(m_opt.getcdrlist.key.callMode).toString());
							result.setIsSuccess(cdrMap.value(m_opt.getcdrlist.key.isSuccess).toString());

							Q_EMIT q->getcdrlistFinished(param, result);
						}
					}
				} else {
					Q_EMIT q->getcdrlistFinished(param, result); // Emit finished when the cdr list is empty
				}
			}
			
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// getcdrinfo
	else if (token.startsWith(getcdrinfoTokenPrefix)) {
		const QStringList args = extractToken(token, getcdrinfoTokenPrefix);
		if (args.size() == 1) {
			Command::GetCDRInfoParam param;
			param.setMeetCDRId(args[0]);

			Command::GetCDRInfoResult result;	
			result.setCommon(commonResult);

			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				record = data.toMap();

				result.setCDRType(record.value(m_opt.getcdrinfo.key.CDRType).toString());
				result.setCallerVESysId(record.value(m_opt.getcdrinfo.key.callerVESysId).toString());
				result.setCallerId(record.value(m_opt.getcdrinfo.key.callerId).toString());
				result.setCallerVELoginId(record.value(m_opt.getcdrinfo.key.callerVELoginId).toString());
				result.setCallerDisplayName(record.value(m_opt.getcdrinfo.key.callerDisplayName).toString());
				result.setCallerMobile(record.value(m_opt.getcdrinfo.key.callerMobile).toString());
				result.setCallerSmallPhoto(record.value(m_opt.getcdrinfo.key.callerSmallPhoto).toString());
				result.setCallerLargePhoto(record.value(m_opt.getcdrinfo.key.callerLargePhoto).toString());
				result.setIsPSTNCall(record.value(m_opt.getcdrinfo.key.isPSTNCall).toString());
				result.setCallCountry(record.value(m_opt.getcdrinfo.key.callCountry).toString());

				result.setCalleeVESysId(record.value(m_opt.getcdrinfo.key.calleeVESysId).toString());
				result.setCalleeId(record.value(m_opt.getcdrinfo.key.calleeId).toString());
				result.setCalleeVELoginId(record.value(m_opt.getcdrinfo.key.calleeVELoginId).toString());
				result.setCalleeDisplayName(record.value(m_opt.getcdrinfo.key.calleeDisplayName).toString());
				result.setCalleeMobile(record.value(m_opt.getcdrinfo.key.calleeMobile).toString());
				result.setCalleeSmallPhoto(record.value(m_opt.getcdrinfo.key.calleeSmallPhoto).toString());
				result.setCalleeLargePhoto(record.value(m_opt.getcdrinfo.key.calleeLargePhoto).toString());

				result.setPartnerVESysId(record.value(m_opt.getcdrinfo.key.partnerVESysId).toString());
				result.setPartnerId(record.value(m_opt.getcdrinfo.key.partnerId).toString());
				result.setPartnerLoginId(record.value(m_opt.getcdrinfo.key.partnerLoginId).toString());
				result.setPartnerDisplayName(record.value(m_opt.getcdrinfo.key.partnerDisplayName).toString());
				result.setPartnerSmallPhoto(record.value(m_opt.getcdrinfo.key.partnerSmallPhoto).toString());
				result.setPartnerLargePhoto(record.value(m_opt.getcdrinfo.key.partnerLargePhoto).toString());

				result.setIsFree(record.value(m_opt.getcdrinfo.key.isFree).toString());
				result.setPartnerLevel(record.value(m_opt.getcdrinfo.key.partnerLevel).toString());
				result.setLanguage1(record.value(m_opt.getcdrinfo.key.language1).toString());
				result.setLanguage2(record.value(m_opt.getcdrinfo.key.language2).toString());
				result.setCallTime(record.value(m_opt.getcdrinfo.key.callTime).toString());
				result.setStartCallTime(record.value(m_opt.getcdrinfo.key.startCallTime).toString());
			}

			Q_EMIT q->getcdrinfoFinished(param, result);

			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// getuserinfo
	else if (token.startsWith(getuserinfoTokenPrefix)) {
		const QStringList args = extractToken(token, getuserinfoTokenPrefix);
		if (args.size() == 1) {
			Command::GetUserInfoParam param;

			Command::GetUserInfoResult result;	
			result.setCommon(commonResult);

			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				record = data.toMap();

				result.setSystemId(record.value(m_opt.getuserinfo.key.systemId).toString());
				result.setCallerId(record.value(m_opt.getuserinfo.key.callerId).toString());
				result.setLoginId(record.value(m_opt.getuserinfo.key.loginId).toString());
				result.setSecret(record.value(m_opt.getuserinfo.key.secret).toString());
				result.setDisplayName(record.value(m_opt.getuserinfo.key.displayName).toString());
				result.setGender(record.value(m_opt.getuserinfo.key.gender).toString());
				result.setBirthday(record.value(m_opt.getuserinfo.key.birthday).toString());
				result.setEducation(record.value(m_opt.getuserinfo.key.education).toString());
				result.setNationality(record.value(m_opt.getuserinfo.key.nationality).toString());
				result.setMajor(record.value(m_opt.getuserinfo.key.major).toString());
				result.setIntroduction(record.value(m_opt.getuserinfo.key.intro).toString());
				result.setSmallPhoto(record.value(m_opt.getuserinfo.key.smallPhoto).toString());
				result.setLargePhoto(record.value(m_opt.getuserinfo.key.largePhoto).toString());
				QStringList skill;
				qDebug() << m_opt.getuserinfo.key.skill;
				qDebug() << record.value(m_opt.getuserinfo.key.skill);
				QVariant skillData = record.value(m_opt.getuserinfo.key.skill);
				if (skillData.isValid() && skillData.canConvert<QVariantList>()) {
					foreach(const QVariant& skl, skillData.toList()) {
						if (skl.isValid() && skl.canConvert<QVariantMap>()) {
							QVariantMap sklMap = skl.toMap();
							skill.append(QString::fromLatin1("%1|%2|%3|%4").arg(
								sklMap.value(m_opt.getuserinfo.key.skillInfo.language1).toString(),
								sklMap.value(m_opt.getuserinfo.key.skillInfo.language2).toString(),
								sklMap.value(m_opt.getuserinfo.key.skillInfo.isFree).toString(),
								sklMap.value(m_opt.getuserinfo.key.skillInfo.level).toString()
								)); 
						}
					}
				}
				result.setSkill(skill);
			}
			Q_EMIT q->getuserinfoFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// getcalleruser
	else if (token.startsWith(getcalleruserTokenPrefix)) {
		const QStringList args = extractToken(token, getcalleruserTokenPrefix);
		if (args.size() == 1) {
			Command::GetCallerUserParam param;
			param.setCallerId(args[0]);

			Command::GetCallerUserResult result;	
			result.setCommon(commonResult);

			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				record = data.toMap();

				result.setSmallPhoto(record.value(m_opt.getcalleruser.key.smallPhoto).toString());
				result.setLargePhoto(record.value(m_opt.getcalleruser.key.largePhoto).toString());
				result.setDisplayName(record.value(m_opt.getcalleruser.key.displayName).toString());
				result.setMobile(record.value(m_opt.getcalleruser.key.mobile).toString());
				
			}
			Q_EMIT q->getcalleruserFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// savescreenshot
	else if (token.startsWith(savescreenshotTokenPrefix)) {
		const QStringList args = extractToken(token, savescreenshotTokenPrefix);
		if (args.size() == 2) {
			Command::SaveScreenShotParam param;
			param.setMeetCDRId(args[0]);
			param.setPhoto(args[1]);

			Command::SaveScreenShotResult result;	
			result.setCommon(commonResult);

			Q_EMIT q->savescreenshotFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
    // changestatus
    else if (token.startsWith(changestatusTokenPrefix)) {
        const QStringList args = extractToken(token, changestatusTokenPrefix);
        if (args.size() == 1) {
            Command::ChangeStatusParam param;
            param.setOnlineVisibleStatus(args[0]);

			Command::ChangeStatusResult result;
			result.setCommon(commonResult);

			Q_EMIT q->changestatusFinished(param, result);
            return;
        } else {
            Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
        }
    }
	// getblacklist
	else if (token.startsWith(getblacklistTokenPrefix)) {
		const QStringList args = extractToken(token, getblacklistTokenPrefix);
		if (args.size() == 1) {
			Command::GetBlacklistParam param;
			param.setId(args[0]);

			Command::GetBlacklistResult result;	
			result.setCommon(commonResult);

			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				record = data.toMap();

				QVariant blacklist = record.value(QLatin1String("list"));
				if (blacklist.isValid() && blacklist.canConvert<QVariantList>()) {
					foreach(const QVariant& bl, blacklist.toList()) {
						if (bl.isValid() && bl.canConvert<QVariantMap>()) {
							QVariantMap blMap = bl.toMap();

							result.setId(blMap.value(m_opt.getblacklist.key.id).toString());
							result.setBlacklistId(blMap.value(m_opt.getblacklist.key.blacklistId).toString());
							result.setPartnerSysId(blMap.value(m_opt.getblacklist.key.partnerSysId).toString());
							result.setVESysId(blMap.value(m_opt.getblacklist.key.VESysId).toString());
							result.setDisplayName(blMap.value(m_opt.getblacklist.key.displayName).toString());
							result.setSmallPhoto(blMap.value(m_opt.getblacklist.key.smallPhoto).toString());

							Q_EMIT q->getblacklistFinished(param, result);
						}
					}
				} else {
					Q_EMIT q->getblacklistFinished(param, result); // Emit finished when the cdr list is empty
				}
			}

			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// addblacklist
	else if (token.startsWith(addblacklistTokenPrefix)) {
		const QStringList args = extractToken(token, addblacklistTokenPrefix);
		if (args.size() == 1) {
			Command::AddBlacklistParam param;
			param.setVESysId(args[0]);

			Command::AddBlacklistResult result;
			result.setCommon(commonResult);

			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				record = data.toMap();

				result.setId(record.value(m_opt.addblacklist.key.id).toString());
				result.setBlacklistId(record.value(m_opt.addblacklist.key.blacklistId).toString());
				result.setPartnerSysId(record.value(m_opt.addblacklist.key.partnerSysId).toString());
				result.setVESysId(record.value(m_opt.addblacklist.key.VESysId).toString());
				result.setDisplayName(record.value(m_opt.addblacklist.key.displayName).toString());
				result.setSmallPhoto(record.value(m_opt.addblacklist.key.smallPhoto).toString());
			}

			Q_EMIT q->addblacklistFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// delblacklist
	else if (token.startsWith(delblacklistTokenPrefix)) {
		const QStringList args = extractToken(token, delblacklistTokenPrefix);
		if (args.size() == 1) {
			Command::DelBlacklistParam param;
			param.setVESysId(args[0]);

			Command::DelBlacklistResult result;
			result.setCommon(commonResult);

			Q_EMIT q->delblacklistFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// getmemberinfo
	else if (token.startsWith(getmemberinfoTokenPrefix)) {
		const QStringList args = extractToken(token, getmemberinfoTokenPrefix);
		if (args.size() == 1) {
			Command::GetMemberInfoParam param;
			param.setVESysId(args[0]);

			Command::GetMemberInfoResult result;
			result.setCommon(commonResult);

			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				record = data.toMap();

				result.setVESysId(record.value(m_opt.getmemberinfo.key.VESysId).toString());
				result.setVELoginId(record.value(m_opt.getmemberinfo.key.VELoginId).toString());
				result.setCallerId(record.value(m_opt.getmemberinfo.key.callerId).toString());
				result.setDisplayName(record.value(m_opt.getmemberinfo.key.displayName).toString());
				result.setNativeLanguage(record.value(m_opt.getmemberinfo.key.nativeLanguage).toString());
				result.setCountry(record.value(m_opt.getmemberinfo.key.country).toString());
				result.setMajor(record.value(m_opt.getmemberinfo.key.major).toString());
				result.setSmallPhoto(record.value(m_opt.getmemberinfo.key.smallPhoto).toString());

			}

			Q_EMIT q->getmemberinfoFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	//getuploadphoto
	else if (token.startsWith(getuploadphotoTokenPrefix)) {
		const QStringList args = extractUploadPhotoToken(token, getuploadphotoTokenPrefix);
		if (args.size() == 2) {
			Command::GetUploadPhotoParam param;
			param.setCallerId(args[0]);
			param.setPhotoId(args[1]);

			Command::GetUploadPhotoResult result;
			result.setCommon(commonResult);

			QVariant data = record.value(m_opt.common.key.result);
			if (data.isValid() && data.canConvert<QVariantMap>()) {
				record = data.toMap();

				QVariant uploadphoto = record.value(QLatin1String("list"));
				if (uploadphoto.isValid() && uploadphoto.canConvert<QVariantList>()) {
					foreach(const QVariant& up, uploadphoto.toList()) {
						if(up.isValid() && up.canConvert<QVariantMap>()) {
							QVariantMap upMap = up.toMap();

							result.setPhotoId(upMap.value(m_opt.getuploadphoto.key.photoId).toString());
							result.setExpire(upMap.value(m_opt.getuploadphoto.key.expire).toString());
							result.setSmallPhoto(upMap.value(m_opt.getuploadphoto.key.smallPhoto).toString());
							result.setLargePhoto(upMap.value(m_opt.getuploadphoto.key.largePhoto).toString());

							Q_EMIT q->getuploadphotoFinished(param, result);
						}				
					}			
				} else {
					Q_EMIT q->getuploadphotoFinished(param, result); // Emit finished when the photo list is empty
				}
			}

			return;			
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// version
	else if (token.startsWith(versionTokenPrefix)) {
		const QStringList args = extractToken(token, versionTokenPrefix);
		if (args.size() == 1) {
			Command::VersionParam param;

			Command::VersionResult result;
			result.setError(record.value(m_opt.common.key.errorCode).toString());
			result.setErrorMsg(record.value(m_opt.common.key.errorMessage).toString());

			QVariant info = record.value(QLatin1String("result"));
			if (info.isValid() && info.canConvert<QVariantMap>()) {
				record = info.toMap();
				result.setNumber(record.value(m_opt.version.key.number).toString());
				result.setLink(record.value(m_opt.version.key.link).toString());
			}			

			Q_EMIT q->versionFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phoneloadconfig
	else if (token.startsWith(phoneloadconfigTokenPrefix)) {
		const QStringList args = extractToken(token, phoneloadconfigTokenPrefix);
		if (args.size() == 2) {
			Command::PhoneLoadConfigParam param;
			param.setLastTime(args[0]);
			param.setReloadConfig(args[1].toInt());

			Command::PhoneLoadConfigResult result;
			result.setError(record.value(m_opt.common.key.errorCode).toString());
			result.setErrorMsg(record.value(m_opt.common.key.errorMessage).toString());

			QVariant info = record.value(m_opt.phoneloadconfig.key.configinfo);
			if (info.isValid() && info.canConvert<QVariantMap>()) {
				QVariantMap infoMap = info.toMap();
				result.setCallMode(infoMap.value(m_opt.phoneloadconfig.key.info.mode));
				result.setServiceLanguage(infoMap.value(m_opt.phoneloadconfig.key.info.service));
				result.setMajorArray(infoMap.value(m_opt.phoneloadconfig.key.info.major));
				result.setUseApp(infoMap.value(m_opt.phoneloadconfig.key.info.useapp));
			}
			result.setVersion(record.value(m_opt.phoneloadconfig.key.version).toString());
			result.setDownloadUrl(record.value(m_opt.phoneloadconfig.key.download).toString());
			result.setForceUpdate(record.value(m_opt.phoneloadconfig.key.forceUpdate).toString());
			result.setVerifyCode(record.value(m_opt.phoneloadconfig.key.verifyCode).toString());
			result.setVersionName(record.value(m_opt.phoneloadconfig.key.versionName).toString());
			result.setPSTNSupported(record.value(m_opt.phoneloadconfig.key.PSTNSupported).toStringList());
			QVariant hinfo = record.value(m_opt.phoneloadconfig.key.hrefinfo);
			if (hinfo.isValid() && hinfo.canConvert<QVariantMap>()) {
				QVariantMap infoMap = hinfo.toMap();
				result.setCreateAccountUrl(infoMap.value(m_opt.phoneloadconfig.key.hinfo.createAccount).toString());
				result.setChangePasswordUrl(infoMap.value(m_opt.phoneloadconfig.key.hinfo.changePassword).toString());
				result.setEditUserInfoUrl(infoMap.value(m_opt.phoneloadconfig.key.hinfo.editUserInfo).toString());
			}

			Q_EMIT q->phoneloadconfigFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phoneregister
	else if (token.startsWith(phoneregisterTokenPrefix)) {
		const QStringList args = extractToken(token, phoneregisterTokenPrefix);
		if (args.size() == 3) {
			Command::PhoneRegisterParam param;
			param.setNativeLanguage(args[0]);
			param.setVoiceraID(args[1]);
			param.setEmail(args[2]);

			Command::PhoneRegisterResult result;
			result.setError(record.value(m_opt.common.key.errorCode).toString());
			result.setErrorMsg(record.value(m_opt.common.key.errorMessage).toString());
			QVariant info = record.value(m_opt.phoneregister.key.result);
			if (info.isValid() && info.canConvert<QVariantMap>()) {
				QVariantMap infoMap = info.toMap();
				result.setSIPCallerID(infoMap.value(m_opt.phoneregister.key.resultinfo.SIPCallerID).toString());
				result.setNickname(infoMap.value(m_opt.phoneregister.key.resultinfo.nickname).toString());
				result.setVoiceraID(infoMap.value(m_opt.phoneregister.key.resultinfo.voiceraID).toString());
				result.setNativeLanguage(infoMap.value(m_opt.phoneregister.key.resultinfo.nativeLanguage).toString());
				result.setSmallPhoto(infoMap.value(m_opt.phoneregister.key.resultinfo.smallPhoto).toString());
				result.setLargePhoto(infoMap.value(m_opt.phoneregister.key.resultinfo.largePhoto).toString());
			}

			Q_EMIT q->phoneregisterFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonelogin
	else if (token.startsWith(phoneloginTokenPrefix)) {
		const QStringList args = extractToken(token, phoneloginTokenPrefix);
		if (args.size() == 2) {
			Command::PhoneLoginParam param;
			param.setVoiceraID(args[0]);
			param.setPassword(args[1]);

			Command::PhoneLoginResult result;
			result.setError(record.value(m_opt.common.key.errorCode).toString());
			result.setErrorMsg(record.value(m_opt.common.key.errorMessage).toString());
			QVariant info = record.value(m_opt.phonelogin.key.result);
			if (info.isValid() && info.canConvert<QVariantMap>()) {
				QVariantMap infoMap = info.toMap();
				result.setSIPCallerID(infoMap.value(m_opt.phonelogin.key.resultinfo.SIPCallerID).toString());
				result.setNativeLanguage(infoMap.value(m_opt.phonelogin.key.resultinfo.nativeLanguage).toString());
				result.setVoiceraID(infoMap.value(m_opt.phonelogin.key.resultinfo.voiceraID).toString());
				result.setSmallPhoto(infoMap.value(m_opt.phonelogin.key.resultinfo.smallPhoto).toString());
				result.setLargePhoto(infoMap.value(m_opt.phonelogin.key.resultinfo.largePhoto).toString());
			}

			Q_EMIT q->phoneloginFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phoneforgetpassword
	else if (token.startsWith(phoneforgetpasswordTokenPrefix)) {
		const QStringList args = extractToken(token, phoneforgetpasswordTokenPrefix);
		if (args.size() == 1) {
			Command::PhoneForgetPasswordParam param;
			param.setEmail(args[0]);

			Command::PhoneForgetPasswordResult result;	
			result.setError(record.value(m_opt.common.key.errorCode).toString());
			result.setErrorMsg(record.value(m_opt.common.key.errorMessage).toString());

			Q_EMIT q->phoneforgetpasswordFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonecontactlist
	else if (token.startsWith(phonecontactlistTokenPrefix)) {
		const QStringList args = extractToken(token, phonecontactlistTokenPrefix);
		if (args.size() == 1) {
			Command::PhoneContactListParam param;
			param.setSearchType(args[0]);

			const QVariant list = record.value(m_opt.phonecontactlist.key.list);
			if (list.isValid() && list.canConvert<QVariantList>()) {
				foreach (const QVariant& m, list.toList()) {
					QVariantMap details = m.toMap();
					Command::PhoneContactListResult result;
					result.setID(details.value(m_opt.phonecontactlist.key.contactList.ID).toString());
					result.setAddBookID(details.value(m_opt.phonecontactlist.key.contactList.addBookID).toString());
					result.setContactName(details.value(m_opt.phonecontactlist.key.contactList.contactName).toString());
					result.setPhone(details.value(m_opt.phonecontactlist.key.contactList.phone));
					result.setVoiceraID(details.value(m_opt.phonecontactlist.key.contactList.voiceraID).toString());
					result.setSmallPhoto(details.value(m_opt.phonecontactlist.key.contactList.smallPhoto).toString());
					result.setLargePhoto(details.value(m_opt.phonecontactlist.key.contactList.largePhoto).toString());
					result.setSIPCallerID(details.value(m_opt.phonecontactlist.key.contactList.SIPCallerID).toString());
					result.setSourceLanguage(details.value(m_opt.phonecontactlist.key.contactList.sourceLanguage).toString());
					result.setTargetLanguage(details.value(m_opt.phonecontactlist.key.contactList.targetLanguage).toString());
					result.setOnlineStatus(details.value(m_opt.phonecontactlist.key.contactList.onlineStatus).toString());
					result.setSystemUser(details.value(m_opt.phonecontactlist.key.contactList.systemUser).toString());
					Q_EMIT q->phonecontactlistFinished(param, result);
				}				
			}      

			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phoneaddcontact
	else if (token.startsWith(phoneaddcontactTokenPrefix)) {
		const QStringList args = extractToken(token, phoneaddcontactTokenPrefix);
		if (args.size() == 3) {
			Command::PhoneAddContactParam param;
			param.setVoiceraID(args[0]);
			param.setName(args[1]);
			param.setPhone(args[2]);

			Command::PhoneAddContactResult result;	
			result.setError(record.value(m_opt.common.key.errorCode).toString());
			result.setErrorMsg(record.value(m_opt.common.key.errorMessage).toString());
			result.setAddBookID(record.value(m_opt.phoneaddcontact.key.addBookID).toString());
			result.setID(record.value(m_opt.phoneaddcontact.key.ID).toString());

			Q_EMIT q->phoneaddcontactFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phoneupdatecontact
	else if (token.startsWith(phoneupdatecontactTokenPrefix)) {
		const QStringList args = extractToken(token, phoneupdatecontactTokenPrefix);
		if (args.size() == 10) {
			Command::PhoneUpdateContactParam param;
			param.setAddBookID(args[0]);
			param.setUpdateType(args[1]);
			param.setOldPhone(args[2]);
			param.setPhoneCountry(args[3]);
			param.setPhone(args[4]);
			param.setDeletePhone(args[5]);
			param.setVoiceraID(args[6]);
			param.setName(args[7]);
			param.setSourceLanguage(args[8]);
			param.setTargetLanguage(args[9]);

			Command::PhoneUpdateContactResult result;
			result.setError(record.value(m_opt.common.key.errorCode).toString());
			result.setErrorMsg(record.value(m_opt.common.key.errorMessage).toString());

			Q_EMIT q->phoneupdatecontactFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonedelcontact
	else if (token.startsWith(phonedelcontactTokenPrefix)) {
		const QStringList args = extractToken(token, phonedelcontactTokenPrefix);
		if (args.size() == 1) {
			Command::PhoneDeleteContactParam param;
			param.setAddBookID(args[0]);

			Command::PhoneDeleteContactResult result;	
			result.setError(record.value(m_opt.common.key.errorCode).toString());
			result.setErrorMsg(record.value(m_opt.common.key.errorMessage).toString());

			Q_EMIT q->phonedelcontactFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonedetailcontact
	else if (token.startsWith(phonedetailcontactTokenPrefix)) {
		const QStringList args = extractToken(token, phonedetailcontactTokenPrefix);
		if (args.size() == 1) {
			Command::PhoneDetailContactParam param;
			param.setAddBookID(args[0]);

			Command::PhoneDetailContactResult result;
			const QVariant info = record.value(m_opt.phonedetailcontact.key.detailInfo);
			if (info.isValid() && info.canConvert<QVariantMap>()) {
				QVariantMap details = info.toMap();
				result.setNativeLanguage(details.value(m_opt.phonedetailcontact.key.info.nativeLanguage).toString());
				result.setNickname(details.value(m_opt.phonedetailcontact.key.info.nickname).toString());
				result.setCountry(details.value(m_opt.phonedetailcontact.key.info.country).toString());
				result.setMajor(details.value(m_opt.phonedetailcontact.key.info.major).toString());
			}      

			Q_EMIT q->phonedetailcontactFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonegethistorycdr
	else if (token.startsWith(phonegethistorycdrTokenPrefix)) {
		const QStringList args = extractToken(token, phonegethistorycdrTokenPrefix);
		if (args.size() == 1) {
			Command::PhoneGetHistoryCDRParam param;
			param.setHistoryType(args[0]);

			const QVariant list = record.value(m_opt.phonegethistorycdr.key.cdrlist);
			if (list.isValid() && list.canConvert<QVariantList>()) {
				foreach (const QVariant& m, list.toList()) {
					QVariantMap details = m.toMap();
					Command::PhoneGetHistoryCDRResult result;

					result.setID(details.value(m_opt.phonegethistorycdr.key.list.ID).toString());
					result.setMeetCDRID(details.value(m_opt.phonegethistorycdr.key.list.meetCDRID).toString());
					result.setCalleeID(details.value(m_opt.phonegethistorycdr.key.list.calleeID).toString());
					result.setSourceLanguage(details.value(m_opt.phonegethistorycdr.key.list.sourceLanguage).toString());
					result.setTargetLanguage(details.value(m_opt.phonegethistorycdr.key.list.targetLanguage).toString());
					result.setCallingTime(details.value(m_opt.phonegethistorycdr.key.list.callingTime).toString());
					result.setCost(details.value(m_opt.phonegethistorycdr.key.list.cost).toString());
					result.setIsSuccess(details.value(m_opt.phonegethistorycdr.key.list.isSuccess).toString());
					result.setIsCallout(details.value(m_opt.phonegethistorycdr.key.list.isCallout).toString());

					Q_EMIT q->phonegethistorycdrFinished(param, result);
				}				
			}
			
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonegetuserinfo
	else if (token.startsWith(phonegetuserinfoTokenPrefix)) {
		const QStringList args = extractToken(token, phonegetuserinfoTokenPrefix);
		if (args.size() == 1) {
			Command::PhoneGetUserInfoParam param;
			param.setVoiceraID(args[0]);

			const QVariant list = record.value(QLatin1String("userinfo"));
			if (list.isValid() && list.canConvert<QVariantList>()) {

				QVariantList l = list.toList();
				if (l.size() > 0) {
					foreach (const QVariant& m, l) 
					{
						QVariantMap details = m.toMap();
						Command::PhoneGetUserInfoResult result;
						result.setEmail(details.value(m_opt.phonegetuserinfo.key.info.email).toString());
						result.setBalance(details.value(m_opt.phonegetuserinfo.key.info.accountBalance).toString());
						result.setNickname(details.value(m_opt.phonegetuserinfo.key.info.nickname).toString());
						result.setNativeLanguage(details.value(m_opt.phonegetuserinfo.key.info.nativeLanguage).toString());
						result.setMajor(details.value(m_opt.phonegetuserinfo.key.info.major).toString());
						result.setCountry(details.value(m_opt.phonegetuserinfo.key.info.country).toString());
						result.setOnlineStatus(details.value(m_opt.phonegetuserinfo.key.info.onlineStatus).toString());
						result.setVoiceraID(details.value(m_opt.phonegetuserinfo.key.info.voiceraID).toString());
						result.setSIPCallerID(details.value(m_opt.phonegetuserinfo.key.info.sipCallerID).toString());
						result.setSmallPhoto(details.value(m_opt.phonegetuserinfo.key.info.smallPhoto).toString());
						result.setLargePhoto(details.value(m_opt.phonegetuserinfo.key.info.largePhoto).toString());

						Q_EMIT q->phonegetuserinfoFinished(param, result);
					}
				} else {
					//No user info obtained
					Command::PhoneGetUserInfoResult result;
					Q_EMIT q->phonegetuserinfoFinished(param, result);
				}	
			} 
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phoneedituserinfo
	else if (token.startsWith(phoneedituserinfoTokenPrefix)) {
		const QStringList args = extractToken(token, phoneedituserinfoTokenPrefix);
		if (args.size() == 2) {
			Command::PhoneEditUserInfoParam param;

			Command::PhoneEditUserInfoResult result;	

			Q_EMIT q->phoneedituserinfoFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonechangepassword
	else if (token.startsWith(phonechangepasswordTokenPrefix)) {
		const QStringList args = extractToken(token, phonechangepasswordTokenPrefix);
		if (args.size() == 2) {
			Command::PhoneChangePasswordParam param;

			Command::PhoneChangePasswordResult result;	

			Q_EMIT q->phonechangepasswordFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonenewpassword
	else if (token.startsWith(phonenewpasswordTokenPrefix)) {
		const QStringList args = extractToken(token, phonenewpasswordTokenPrefix);
		if (args.size() == 2) {
			Command::PhoneNewPasswordParam param;

			Command::PhoneNewPasswordResult result;	

			Q_EMIT q->phonenewpasswordFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonegetcdrid
	else if (token.startsWith(phonegetcdridTokenPrefix)) {
		const QStringList args = extractToken(token, phonegetcdridTokenPrefix);
		if (args.size() == 1) {
			Command::PhoneGetCDRIDParam param;
			param.setCDRID(args[0]);

			const QVariant list = record.value(QLatin1String("cdrinfo"));
			if (list.isValid() && list.canConvert<QVariantList>()) {
				foreach (const QVariant& m, list.toList()) {
					QVariantMap details = m.toMap();
					Command::PhoneGetCDRIDResult result;

					result.setID(details.value(m_opt.phonegetcdrid.key.info.ID).toString());
					result.setCallerID(details.value(m_opt.phonegetcdrid.key.info.callerID).toString());
					result.setDeductionTime(details.value(m_opt.phonegetcdrid.key.info.deductionTime).toString());
					result.setMeetCDRID(details.value(m_opt.phonegetcdrid.key.info.meetCDRID).toString());
					result.setCallTime(details.value(m_opt.phonegetcdrid.key.info.callTime).toString());
					result.setContactName(details.value(m_opt.phonegetcdrid.key.info.contactName).toString());
					result.setCallStatus(details.value(m_opt.phonegetcdrid.key.info.callStatus).toString());
					result.setSmallPhoto(details.value(m_opt.phonegetcdrid.key.info.smallPhoto).toString());
					result.setLargePhoto(details.value(m_opt.phonegetcdrid.key.info.largePhoto).toString());
					result.setDeductionCost(details.value(m_opt.phonegetcdrid.key.info.deductionCost).toString());
					result.setSourceLanguage(details.value(m_opt.phonegetcdrid.key.info.sourceLanguage).toString());
					result.setTargetLanguage(details.value(m_opt.phonegetcdrid.key.info.targetLanguage).toString());
					result.setCallMode(details.value(m_opt.phonegetcdrid.key.info.callMode).toString());
					result.setPSTNCountry(details.value(m_opt.phonegetcdrid.key.info.PSTNCountry).toString());
					result.setVoiceraID(details.value(m_opt.phonegetcdrid.key.info.voiceraID).toString());

					Q_EMIT q->phonegetcdridFinished(param, result);
				}				
			}

			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}
	// phonedelcdr
	else if (token.startsWith(phonedelcdrTokenPrefix)) {
		const QStringList args = extractToken(token, phonedelcdrTokenPrefix);
		if (args.size() == 1) {
			Command::PhoneDeleteCDRParam param;
			param.setMeetCDRID(args[0]);

			Command::PhoneDeleteCDRResult result;
			result.setError(record.value(m_opt.common.key.errorCode).toString());
			result.setErrorMsg(record.value(m_opt.common.key.errorMessage).toString());

			Q_EMIT q->phonedelcdrFinished(param, result);
			return;
		} else {
			Q_ASSERT_X(false, "CommandServicePrivate::_q_commandFinished", "Bad token");
		}
	}


    qWarning() << DEBUG_PREFIX << "Token" << token << "not recognized";
}

QString CommandServicePrivate::makeToken(const QString& prefix, const QString& arg1)
{
    QString ret = prefix;
    ret += arg1;
    return ret;
}

QString CommandServicePrivate::makeToken(const QString& prefix, const QString& arg1, const QString& arg2)
{
    QString ret = prefix;
    ret += arg1;
    ret += tokenSeparator;
    ret += arg2;
    return ret;
}

QString CommandServicePrivate::makeToken(const QString& prefix, const QString& arg1, const QString& arg2, const QString& arg3)
{
    QString ret = prefix;
    ret += arg1;
    ret += tokenSeparator;
    ret += arg2;
    ret += tokenSeparator;
    ret += arg3;
    return ret;
}

QString CommandServicePrivate::makeToken(const QString& prefix, const QString& arg1, const QString& arg2, const QString& arg3, const QString& arg4)
{
    QString ret = prefix;
    ret += arg1;
    ret += tokenSeparator;
    ret += arg2;
    ret += tokenSeparator;
    ret += arg3;
    ret += tokenSeparator;
    ret += arg4;
    return ret;
}

QString CommandServicePrivate::makeToken(const QString& prefix, const QStringList& args)
{
    QString ret = prefix;
    for (int i = 0; i < args.size(); ++i) {
        if (i > 0)
            ret += tokenSeparator;
        ret += args[i];
    }
    return ret;
}

QStringList CommandServicePrivate::extractToken(const QString& token, const QString& prefix)
{
    if (!token.startsWith(prefix))
        return QStringList();

    const QString args = token.mid(prefix.length());
    return args.split(tokenSeparator);
}

QStringList CommandServicePrivate::extractUploadPhotoToken(const QString& token, const QString& prefix)
{
	if (!token.startsWith(prefix))
		return QStringList();

	QStringList upToken;

	QString upPrefix = token.left(token.indexOf(tokenSeparator));
	QString callerId = upPrefix.right(upPrefix.length() - upPrefix.indexOf(QLatin1String(":")) - 1);
	QString photoId = token.right(token.length() - token.indexOf(tokenSeparator) - 1);
	upToken.append(callerId);
	upToken.append(photoId);

	return upToken;
}

void CommandServicePrivate::addQueryItem(QUrl* url, const QString &key, const QString &value)
{
	url->addQueryItem(key, value);
// 	if (value.isEmpty())
// 		return;
// 
// 	QString urlString = url->toString();
// 	if (!urlString.endsWith(QLatin1String("/")))
// 		urlString += QLatin1String("/");
// 
// 	urlString += key;
// 	urlString += QLatin1String("/");
// 	urlString += value;
// 	urlString += QLatin1String("/");
// 
// 	url->setUrl(urlString);
}

void CommandServicePrivate::addSessionIdQueryItem(QUrl* url) const
{
	Q_ASSERT(url);

	QString sessionId;
	if (m_userService) {
		sessionId = m_userService->userSessionId();
	}

	if (!sessionId.isEmpty()) {
		url->addQueryItem(m_opt.common.query.sessionId, sessionId);
	}
}

QString CommandServicePrivate::makeFullUrl(const QString& url) const
{
    QString serviceHost;
    if (m_userService) {
        serviceHost = m_userService->userServiceHost();
    }

    QString ret = url;
    ret.replace(serviceHostPlaceholder, serviceHost);

    return ret;
}

void CommandServicePrivate::addCommonQueryItem(QUrl* url)
{
	url->addQueryItem(m_opt.common.query.language, m_language);
	url->addQueryItem(m_opt.common.query.deviceId, m_deviceId);
	url->addQueryItem(m_opt.common.query.deviceModel, m_deviceModel);
}

void CommandServicePrivate::addCommonRequestHeaders(QNetworkRequest& request)
{
	int timestamp = QDateTime::currentDateTime().toTime_t();
	QString strTimestamp = QString::number(timestamp);
	QByteArray appid  = QByteArray("A61201281860668");
	QByteArray appkey = QByteArray("10990ED5D-92AE-4E43-8470-5FCC66A550B3");
	QByteArray secret = QCryptographicHash::hash(appid + appkey + strTimestamp.toLocal8Bit(), QCryptographicHash::Sha1).toHex().constData();

	QString time = strTimestamp.prepend(QLatin1String("."));
	secret.append(QByteArray(time.toAscii()));

	request.setRawHeader("X-YesPo-AppId", appid);
	request.setRawHeader("X-YesPo-AppSecret", secret);
	request.setRawHeader("X-YesPo-AppClient", "partner");
	request.setRawHeader("X-YesPo-AppVer", "v1");
}

QByteArray CommandServicePrivate::encryptUserName(const QByteArray& text, const QByteArray& key)
{
    const QByteArray key1 = QCryptographicHash::hash(
        QByteArray::number(qrand() % 32000), QCryptographicHash::Md5).toHex();

    int n = 0;
    QByteArray ret;

    for (int i = 0; i < text.size(); ++i) {
        if (n >= key1.size())
            n = 0;
        ret += key1[n];
        ret += (text[i] ^ key1[n++]);
    }

    ret = encryptKey(ret, key);
    ret = ret.toBase64();

    return QUrl::toPercentEncoding(QString::fromUtf8(ret.constData(), ret.size()));
}

QByteArray CommandServicePrivate::encryptPassword(const QByteArray& text, const QByteArray& key)
{
    Q_UNUSED(key);

    QByteArray ret = QCryptographicHash::hash(text, QCryptographicHash::Md5).toHex();
    return QUrl::toPercentEncoding(QString::fromUtf8(ret.constData(), ret.size()));
}

QByteArray CommandServicePrivate::encryptKey(const QByteArray& text, const QByteArray& key)
{
    const QByteArray key1 = QCryptographicHash::hash(key, QCryptographicHash::Md5).toHex();

    int n = 0;
    QByteArray ret;

    for (int i = 0; i < text.size(); ++i) {
        if (n >= key1.size())
            n = 0;
        ret += (text[i] ^ key1[n++]);
    }

    return ret;
}

CommandService::CommandService(ServiceManager* manager, QObject* parent) :
    AbstractService(*new CommandServicePrivate(manager, QString()), parent)
{
    Q_D(CommandService);
    d->init();
}

CommandService::CommandService(ServiceManager* manager, const QString& name, QObject* parent) :
    AbstractService(*new CommandServicePrivate(manager, name), parent)
{
    Q_D(CommandService);
    d->init();
}

CommandService::CommandService(CommandServicePrivate& dd, QObject* parent) :
    AbstractService(dd, parent)
{
    Q_D(CommandService);
    d->init();
}

CommandService::~CommandService()
{
}

void CommandService::applyPreference(const QSet<QString>& paths, bool force)
{
    Q_D(CommandService);

    typedef CommandServiceOptions Opt;
    Opt& opt = d->m_opt;
    Preference* pref = preference();

    if (force || paths.contains(Opt::common_)) {
        opt.common.query.sessionId = pref->getString(Opt::common_query_session_id_);
		opt.common.query.sign = pref->getString(Opt::common_query_sign_);
		opt.common.query.timestamp = pref->getString(Opt::common_query_timestamp_);
		opt.common.query.client = pref->getString(Opt::common_query_client_);
		opt.common.query.version = pref->getString(Opt::common_query_version_);
		opt.common.query.language = pref->getString(Opt::common_query_language_);
		opt.common.query.deviceId = pref->getString(Opt::common_query_device_id_);
		opt.common.query.deviceModel = pref->getString(Opt::common_query_device_model_);
		opt.common.query.token = pref->getString(Opt::common_query_token_);
        opt.common.key.errorCode = pref->getString(Opt::common_key_error_code_);
        opt.common.key.errorMessage = pref->getString(Opt::common_key_error_message_);
		opt.common.key.errorNumber = pref->getString(Opt::common_key_error_number_);
		opt.common.key.errorTips = pref->getString(Opt::common_key_error_tips_);
		opt.common.key.result = pref->getString(Opt::common_key_result_);
    }

	if (force || paths.contains(Opt::loadconfig_)) {
		opt.loadconfig.url = pref->getString(Opt::loadconfig_url_);
		opt.loadconfig.query.lastTime = pref->getString(Opt::loadconfig_query_last_time_);
		opt.loadconfig.query.reload = pref->getString(Opt::loadconfig_query_reload_);
		opt.loadconfig.key.config = pref->getString(Opt::loadconfig_key_config_);
		opt.loadconfig.key.client = pref->getString(Opt::loadconfig_key_client_);
	}

	if (force || paths.contains(Opt::getserver_url_)) {
		opt.getserver.url = pref->getString(Opt::getserver_url_);
		opt.getserver.key.domain_status = pref->getString(Opt::getserver_key_domain_status_);
		opt.getserver.key.domain_call = pref->getString(Opt::getserver_key_domain_call_);
		opt.getserver.key.domain_im = pref->getString(Opt::getserver_key_domain_im_);
	}

    if (force || paths.contains(Opt::login_)) {
        opt.login.url = pref->getString(Opt::login_url_);
        opt.login.encryptKey = pref->getString(Opt::login_encrypt_key_);
        opt.login.query.userName = pref->getString(Opt::login_query_user_name_);
        opt.login.query.password = pref->getString(Opt::login_query_password_);
		opt.login.key.user = pref->getString(Opt::login_key_user_);
		opt.login.key.userInfo.systemId = pref->getString(Opt::login_key_user_system_id_);
        opt.login.key.userInfo.callerId = pref->getString(Opt::login_key_user_caller_id_);
        opt.login.key.userInfo.loginId = pref->getString(Opt::login_key_user_login_id_);
        opt.login.key.userInfo.secret = pref->getString(Opt::login_key_user_secret_);
        opt.login.key.userInfo.displayName = pref->getString(Opt::login_key_user_display_name_);
		opt.login.key.userInfo.skill = pref->getString(Opt::login_key_user_skill_);
        opt.login.key.userInfo.skillInfo.language1 = pref->getString(Opt::login_key_user_skill_language_1_);
        opt.login.key.userInfo.skillInfo.language2 = pref->getString(Opt::login_key_user_skill_language_2_);
        opt.login.key.userInfo.skillInfo.isFree = pref->getString(Opt::login_key_user_skill_is_free_);
		opt.login.key.userInfo.skillInfo.level = pref->getString(Opt::login_key_user_skill_level_);
		opt.login.key.userInfo.local= pref->getString(Opt::login_key_user_local_);
		opt.login.key.userInfo.localInfo.country = pref->getString(Opt::login_key_user_local_country_);
		opt.login.key.userInfo.localInfo.language = pref->getString(Opt::login_key_user_local_language_);
		opt.login.key.userInfo.smallPhoto = pref->getString(Opt::login_key_user_small_photo_);
		opt.login.key.userInfo.largePhoto = pref->getString(Opt::login_key_user_large_photo_);
		opt.login.key.userInfo.token = pref->getString(Opt::login_key_user_token_);
		opt.login.key.serverIp = pref->getString(Opt::login_key_server_ip_);
		opt.login.key.serverIpInfo.domainStatus = pref->getString(Opt::login_key_server_ip_domain_status_);
		opt.login.key.serverIpInfo.domainCall = pref->getString(Opt::login_key_server_ip_domain_call_);
		opt.login.key.serverIpInfo.domainIm = pref->getString(Opt::login_key_server_ip_domain_im_);
		opt.login.key.servers = pref->getString(Opt::login_key_servers_);
		opt.login.key.serverListInfo.domainCall = pref->getString(Opt::login_key_servers_domain_call_);
		opt.login.key.serverListInfo.domainIm = pref->getString(Opt::login_key_servers_domain_im_);
		opt.login.key.serverListInfo.domainStatus = pref->getString(Opt::login_key_servers_domain_status_);
    }

	if (force || paths.contains(Opt::setclientinfo_)) {
		opt.setclientinfo.url = pref->getString(Opt::setclientinfo_url_);
		opt.setclientinfo.query.lastDomainCall = pref->getString(Opt::setclientinfo_query_last_domain_call_);
	}

	if (force || paths.contains(Opt::todaystat_)) {
		opt.todaystat.url = pref->getString(Opt::todaystat_url_);
		opt.todaystat.key.answer = pref->getString(Opt::todaystat_key_answer_);
		opt.todaystat.key.success = pref->getString(Opt::todaystat_key_success_);
		opt.todaystat.key.noanswer = pref->getString(Opt::todaystat_key_noanswer_);
	}

	if (force || paths.contains(Opt::getnoreportinfo_)) {
		opt.getnoreportinfo.url = pref->getString(Opt::getnoreportinfo_url_);
		opt.getnoreportinfo.key.num = pref->getString(Opt::getnoreportinfo_key_no_report_num_);
	}

	if (force || paths.contains(Opt::getcdrlist_)) {
		opt.getcdrlist.url = pref->getString(Opt::getcdrlist_url_);
		opt.getcdrlist.query.id = pref->getString(Opt::getcdrlist_query_id_);
		opt.getcdrlist.query.type = pref->getString(Opt::getcdrlist_query_type_);
		opt.getcdrlist.key.id = pref->getString(Opt::getcdrlist_key_id_);
		opt.getcdrlist.key.VESysId = pref->getString(Opt::getcdrlist_key_ve_sys_id_);
		opt.getcdrlist.key.meetCDRId = pref->getString(Opt::getcdrlist_key_meet_cdr_id_);
		opt.getcdrlist.key.smallPhoto = pref->getString(Opt::getcdrlist_key_small_photo_);
		opt.getcdrlist.key.displayName = pref->getString(Opt::getcdrlist_key_displayname_);
		opt.getcdrlist.key.startCallTime = pref->getString(Opt::getcdrlist_key_start_call_time_);
		opt.getcdrlist.key.callTime = pref->getString(Opt::getcdrlist_key_call_time_);
		opt.getcdrlist.key.isAnswered = pref->getString(Opt::getcdrlist_key_is_answered_);
		opt.getcdrlist.key.callMode = pref->getString(Opt::getcdrlist_key_call_mode_);
		opt.getcdrlist.key.isSuccess = pref->getString(Opt::getcdrlist_key_is_success_);
	}

	if (force || paths.contains(Opt::getcdrinfo_)) {
		opt.getcdrinfo.url = pref->getString(Opt::getcdrinfo_url_);
		opt.getcdrinfo.query.meetCDRId = pref->getString(Opt::getcdrinfo_query_meet_cdr_id_);
		opt.getcdrinfo.key.CDRType = pref->getString(Opt::getcdrinfo_key_cdr_type_);
		opt.getcdrinfo.key.callerVESysId = pref->getString(Opt::getcdrinfo_key_caller_ve_sys_id_);
		opt.getcdrinfo.key.callerId = pref->getString(Opt::getcdrinfo_key_caller_id_);

		opt.getcdrinfo.key.callerVELoginId = pref->getString(Opt::getcdrinfo_key_caller_ve_login_id_);

		opt.getcdrinfo.key.callerDisplayName = pref->getString(Opt::getcdrinfo_key_caller_display_name_);
		opt.getcdrinfo.key.callerMobile = pref->getString(Opt::getcdrinfo_key_caller_mobile_);
		opt.getcdrinfo.key.callerSmallPhoto = pref->getString(Opt::getcdrinfo_key_caller_small_photo_);
		opt.getcdrinfo.key.callerLargePhoto = pref->getString(Opt::getcdrinfo_key_caller_large_photo_);
		opt.getcdrinfo.key.isPSTNCall = pref->getString(Opt::getcdrinfo_key_is_pstn_call_);
		opt.getcdrinfo.key.callCountry = pref->getString(Opt::getcdrinfo_key_call_country_);
		opt.getcdrinfo.key.calleeVESysId = pref->getString(Opt::getcdrinfo_key_callee_ve_sys_id_);
		opt.getcdrinfo.key.calleeId = pref->getString(Opt::getcdrinfo_key_callee_id_);

		opt.getcdrinfo.key.calleeVELoginId = pref->getString(Opt::getcdrinfo_key_callee_ve_login_id_);

		opt.getcdrinfo.key.calleeDisplayName = pref->getString(Opt::getcdrinfo_key_callee_display_name_);
		opt.getcdrinfo.key.calleeMobile = pref->getString(Opt::getcdrinfo_key_callee_mobile_);
		opt.getcdrinfo.key.calleeSmallPhoto = pref->getString(Opt::getcdrinfo_key_callee_small_photo_);
		opt.getcdrinfo.key.calleeLargePhoto = pref->getString(Opt::getcdrinfo_key_callee_large_photo_);
		opt.getcdrinfo.key.partnerVESysId = pref->getString(Opt::getcdrinfo_key_partner_ve_sys_id_);
		opt.getcdrinfo.key.partnerId = pref->getString(Opt::getcdrinfo_key_partner_id_);

		opt.getcdrinfo.key.partnerLoginId = pref->getString(Opt::getcdrinfo_key_partner_login_id_);

		opt.getcdrinfo.key.partnerDisplayName = pref->getString(Opt::getcdrinfo_key_partner_display_name_);
		opt.getcdrinfo.key.partnerSmallPhoto = pref->getString(Opt::getcdrinfo_key_partner_small_photo_);
		opt.getcdrinfo.key.partnerLargePhoto = pref->getString(Opt::getcdrinfo_key_partner_large_photo_);
		opt.getcdrinfo.key.isFree = pref->getString(Opt::getcdrinfo_key_is_free_);
		opt.getcdrinfo.key.partnerLevel = pref->getString(Opt::getcdrinfo_key_partner_level_);
		opt.getcdrinfo.key.language1 = pref->getString(Opt::getcdrinfo_key_language_1_);
		opt.getcdrinfo.key.language2 = pref->getString(Opt::getcdrinfo_key_language_2_);
		opt.getcdrinfo.key.callTime = pref->getString(Opt::getcdrinfo_key_call_time_);
		opt.getcdrinfo.key.startCallTime = pref->getString(Opt::getcdrinfo_key_start_call_time_);
	}

	if (force || paths.contains(Opt::getuserinfo_)) {
		opt.getuserinfo.url = pref->getString(Opt::getuserinfo_url_);
		opt.getuserinfo.key.systemId = pref->getString(Opt::getuserinfo_key_system_id_);
		opt.getuserinfo.key.callerId = pref->getString(Opt::getuserinfo_key_caller_id_);
		opt.getuserinfo.key.loginId = pref->getString(Opt::getuserinfo_key_login_id_);
		opt.getuserinfo.key.secret = pref->getString(Opt::getuserinfo_key_secret_);
		opt.getuserinfo.key.displayName = pref->getString(Opt::getuserinfo_key_display_name_);
		opt.getuserinfo.key.gender = pref->getString(Opt::getuserinfo_key_gender_);
		opt.getuserinfo.key.birthday = pref->getString(Opt::getuserinfo_key_birthday_);
		opt.getuserinfo.key.nationality = pref->getString(Opt::getuserinfo_key_nationality_);
		opt.getuserinfo.key.education = pref->getString(Opt::getuserinfo_key_education_);
		opt.getuserinfo.key.major = pref->getString(Opt::getuserinfo_key_major_);
		opt.getuserinfo.key.intro = pref->getString(Opt::getuserinfo_key_intro_);
		opt.getuserinfo.key.skill = pref->getString(Opt::getuserinfo_key_skill_);
		opt.getuserinfo.key.skillInfo.language1 = pref->getString(Opt::getuserinfo_key_skill_language_1_);
		opt.getuserinfo.key.skillInfo.language2 = pref->getString(Opt::getuserinfo_key_skill_language_2_);
		opt.getuserinfo.key.skillInfo.isFree = pref->getString(Opt::getuserinfo_key_skill_is_free_);
		opt.getuserinfo.key.skillInfo.level = pref->getString(Opt::getuserinfo_key_skill_level_);
		opt.getuserinfo.key.smallPhoto = pref->getString(Opt::getuserinfo_key_small_photo_);
		opt.getuserinfo.key.largePhoto = pref->getString(Opt::getuserinfo_key_large_photo_);
	}

	if (force || paths.contains(Opt::getcalleruser_)) {
		opt.getcalleruser.url = pref->getString(Opt::getcalleruser_url_);
		opt.getcalleruser.query.callerId = pref->getString(Opt::getcalleruser_query_caller_id_);
		opt.getcalleruser.key.smallPhoto = pref->getString(Opt::getcalleruser_key_small_photo_);
		opt.getcalleruser.key.largePhoto = pref->getString(Opt::getcalleruser_key_large_photo_);
		opt.getcalleruser.key.displayName = pref->getString(Opt::getcalleruser_key_display_name_);
		opt.getcalleruser.key.mobile = pref->getString(Opt::getcalleruser_key_mobile_);
	}

	if (force || paths.contains(Opt::savescreenshot_)) {
		opt.savescreenshot.url = pref->getString(Opt::savescreenshot_url_);
		opt.savescreenshot.query.meetCDRId = pref->getString(Opt::savescreenshot_query_meet_cdr_id_);
		opt.savescreenshot.query.photo = pref->getString(Opt::savescreenshot_query_photo_);
	}

    if (force || paths.contains(Opt::forgetpassword_)) {
        opt.forgetpassword.url = pref->getString(Opt::forgetpassword_url_);
        opt.forgetpassword.query.userName = pref->getString(Opt::forgetpassword_query_user_name_);
    }

	if (force || paths.contains(Opt::createaccount_)) {
		opt.createaccount.url = pref->getString(Opt::createaccount_url_);
	}

    if (force || paths.contains(Opt::changestatus_)) {
        opt.changestatus.url = pref->getString(Opt::changestatus_url_);
        opt.changestatus.query.onlineVisibleStatus = pref->getString(Opt::changestatus_query_online_visible_status_);
    }

	if (force || paths.contains(Opt::getblacklist_)) {
		opt.getblacklist.url = pref->getString(Opt::getblacklist_url_);
		opt.getblacklist.query.id = pref->getString(Opt::getblacklist_query_id_);
		opt.getblacklist.key.id = pref->getString(Opt::getblacklist_key_id_);
		opt.getblacklist.key.blacklistId = pref->getString(Opt::getblacklist_key_blacklist_id_);
		opt.getblacklist.key.partnerSysId = pref->getString(Opt::getblacklist_key_partner_sys_id_);
		opt.getblacklist.key.VESysId = pref->getString(Opt::getblacklist_key_ve_sys_id_);
		opt.getblacklist.key.displayName = pref->getString(Opt::getblacklist_key_display_name_);
		opt.getblacklist.key.smallPhoto = pref->getString(Opt::getblacklist_key_small_photo_);
	}

	if (force || paths.contains(Opt::addblacklist_)) {
		opt.addblacklist.url = pref->getString(Opt::addblacklist_url_);
		opt.addblacklist.query.VESysId = pref->getString(Opt::addblacklist_query_ve_sys_id_);
		opt.addblacklist.key.id = pref->getString(Opt::addblacklist_key_id_);
		opt.addblacklist.key.blacklistId = pref->getString(Opt::addblacklist_key_blacklist_id_);
		opt.addblacklist.key.partnerSysId = pref->getString(Opt::addblacklist_key_partner_sys_id_);
		opt.addblacklist.key.VESysId = pref->getString(Opt::addblacklist_key_ve_sys_id_);
		opt.addblacklist.key.displayName = pref->getString(Opt::addblacklist_key_display_name_);
		opt.addblacklist.key.smallPhoto = pref->getString(Opt::addblacklist_key_small_photo_);
	}

	if (force || paths.contains(Opt::delblacklist_)) {
		opt.delblacklist.url = pref->getString(Opt::delblacklist_url_);
		opt.delblacklist.query.VESysId = pref->getString(Opt::delblacklist_query_ve_sys_id_);
	}

	if (force || paths.contains(Opt::getmemberinfo_)) {
		opt.getmemberinfo.url = pref->getString(Opt::getmemberinfo_url_);
		opt.getmemberinfo.query.VESysId = pref->getString(Opt::getmemberinfo_query_ve_sys_id_);

		opt.getmemberinfo.key.VESysId = pref->getString(Opt::getmemberinfo_key_ve_sys_id_);
		opt.getmemberinfo.key.VELoginId = pref->getString(Opt::getmemberinfo_key_ve_login_id_);
		opt.getmemberinfo.key.callerId = pref->getString(Opt::getmemberinfo_key_caller_id_);
		opt.getmemberinfo.key.displayName = pref->getString(Opt::getmemberinfo_key_display_name_);
		opt.getmemberinfo.key.nativeLanguage = pref->getString(Opt::getmemberinfo_key_native_language_);
		opt.getmemberinfo.key.country = pref->getString(Opt::getmemberinfo_key_country_);
		opt.getmemberinfo.key.major = pref->getString(Opt::getmemberinfo_key_major_);
		opt.getmemberinfo.key.smallPhoto = pref->getString(Opt::getmemberinfo_key_small_photo_);
	}

	if (force || paths.contains(Opt::getuploadphoto_)) {
		opt.getuploadphoto.url = pref->getString(Opt::getuploadphoto_url_);
		opt.getuploadphoto.query.callerId = pref->getString(Opt::getuploadphoto_query_caller_id_);
		opt.getuploadphoto.query.photoId = pref->getString(Opt::getuploadphoto_query_photo_id);

		opt.getuploadphoto.key.photoId = pref->getString(Opt::getuploadphoto_key_photo_id);
		opt.getuploadphoto.key.expire = pref->getString(Opt::getuploadphoto_key_expire);
		opt.getuploadphoto.key.smallPhoto = pref->getString(Opt::getuploadphoto_key_small_photo_);
		opt.getuploadphoto.key.largePhoto = pref->getString(Opt::getuploadphoto_key_large_photo_);
	}

	if (force || paths.contains(Opt::version_)) {
		opt.version.url = pref->getString(Opt::version_url_);
		opt.version.key.number = pref->getString(Opt::version_key_number_);
		opt.version.key.link = pref->getString(Opt::version_key_link_);
	}

	if(force || paths.contains(Opt::phoneloadconfig_)) {
		opt.phoneloadconfig.url = pref->getString(Opt::phoneloadconfig_url_);
		opt.phoneloadconfig.query.lastTime = pref->getString(Opt::phoneloadconfig_query_last_time_);
		opt.phoneloadconfig.query.reload = pref->getString(Opt::phoneloadconfig_query_reload_);
		opt.phoneloadconfig.key.configinfo = pref->getString(Opt::phoneloadconfig_key_info_);
		opt.phoneloadconfig.key.info.service = pref->getString(Opt::phoneloadconfig_key_info_service_);
		opt.phoneloadconfig.key.info.major = pref->getString(Opt::phoneloadconfig_key_info_major_);
		opt.phoneloadconfig.key.info.useapp = pref->getString(Opt::phoneloadconfig_key_info_useapp_);
		opt.phoneloadconfig.key.info.mode = pref->getString(Opt::phoneloadconfig_key_info_mode_);
		opt.phoneloadconfig.key.version = pref->getString(Opt::phoneloadconfig_key_version_);
		opt.phoneloadconfig.key.download = pref->getString(Opt::phoneloadconfig_key_download_);
		opt.phoneloadconfig.key.forceUpdate = pref->getString(Opt::phoneloadconfig_key_force_update_);
		opt.phoneloadconfig.key.verifyCode = pref->getString(Opt::phoneloadconfig_key_verify_code_);
		opt.phoneloadconfig.key.versionName = pref->getString(Opt::phoneloadconfig_key_version_name_);
		opt.phoneloadconfig.key.PSTNSupported = pref->getString(Opt::phoneloadconfig_key_pstn_supported_);
		opt.phoneloadconfig.key.hrefinfo = pref->getString(Opt::phoneloadconfig_key_href_info_);
		opt.phoneloadconfig.key.hinfo.createAccount = pref->getString(Opt::phoneloadconfig_key_href_info_create_account_);
		opt.phoneloadconfig.key.hinfo.changePassword = pref->getString(Opt::phoneloadconfig_key_href_info_change_password_);
		opt.phoneloadconfig.key.hinfo.editUserInfo = pref->getString(Opt::phoneloadconfig_key_href_info_edit_userinfo_);
	}

	if(force || paths.contains(Opt::phoneregister_)) {
		opt.phoneregister.url = pref->getString(Opt::phoneregister_url_);
		opt.phoneregister.query.nativeLanguage = pref->getString(Opt::phoneregister_query_native_language_);
		opt.phoneregister.query.voiceraID = pref->getString(Opt::phoneregister_query_voicera_id_);
		opt.phoneregister.query.email = pref->getString(Opt::phoneregister_query_email_);
		opt.phoneregister.query.password = pref->getString(Opt::phoneregister_query_password_);
		opt.phoneregister.query.photo = pref->getString(Opt::phoneregister_query_photo_);
		opt.phoneregister.key.result = pref->getString(Opt::phoneregister_key_result_);
		opt.phoneregister.key.resultinfo.SIPCallerID = pref->getString(Opt::phoneregister_key_result_sip_caller_id_);
		opt.phoneregister.key.resultinfo.nickname = pref->getString(Opt::phoneregister_key_result_nickname_);
		opt.phoneregister.key.resultinfo.voiceraID = pref->getString(Opt::phoneregister_key_result_voicera_id_);
		opt.phoneregister.key.resultinfo.nativeLanguage = pref->getString(Opt::phoneregister_key_result_native_language_);
		opt.phoneregister.key.resultinfo.smallPhoto = pref->getString(Opt::phoneregister_key_result_small_photo_);
		opt.phoneregister.key.resultinfo.largePhoto = pref->getString(Opt::phoneregister_key_result_large_photo_);
	}

	if(force || paths.contains(Opt::phonelogin_)) {
		opt.phonelogin.url = pref->getString(Opt::phonelogin_url_);
		opt.phonelogin.query.voiceraID = pref->getString(Opt::phonelogin_query_voicera_id_);
		opt.phonelogin.query.password = pref->getString(Opt::phonelogin_query_password_);
		opt.phonelogin.key.result = pref->getString(Opt::phonelogin_key_result_);
		opt.phonelogin.key.resultinfo.SIPCallerID = pref->getString(Opt::phonelogin_key_result_sip_caller_id_);
		opt.phonelogin.key.resultinfo.nativeLanguage = pref->getString(Opt::phonelogin_key_result_native_language_);
		opt.phonelogin.key.resultinfo.voiceraID = pref->getString(Opt::phonelogin_key_result_voicera_id_);
		opt.phonelogin.key.resultinfo.smallPhoto = pref->getString(Opt::phonelogin_key_result_small_photo_);
		opt.phonelogin.key.resultinfo.largePhoto = pref->getString(Opt::phonelogin_key_result_large_photo_);
	}

	if (force || paths.contains(Opt::phoneforgetpassword_)) {
		opt.phoneforgetpassword.url = pref->getString(Opt::phoneforgetpassword_url_);
		opt.phoneforgetpassword.query.email = pref->getString(Opt::phoneforgetpassword_query_email_);
	}

	if (force || paths.contains(Opt::phonecontactlist_)) {
		opt.phonecontactlist.url = pref->getString(Opt::phonecontactlist_url_);
		opt.phonecontactlist.query.searchType = pref->getString(Opt::phonecontactlist_query_search_type_);
		opt.phonecontactlist.key.list = pref->getString(Opt::phonecontactlist_key_contact_list_);
		opt.phonecontactlist.key.contactList.ID = pref->getString(Opt::phonecontactlist_key_contact_list_id_);
		opt.phonecontactlist.key.contactList.addBookID = pref->getString(Opt::phonecontactlist_key_contact_list_add_book_id_);
		opt.phonecontactlist.key.contactList.contactName = pref->getString(Opt::phonecontactlist_key_contact_list_contact_name_);
		opt.phonecontactlist.key.contactList.smallPhoto = pref->getString(Opt::phonecontactlist_key_contact_list_small_photo_);
		opt.phonecontactlist.key.contactList.largePhoto = pref->getString(Opt::phonecontactlist_key_contact_list_large_photo_);
		opt.phonecontactlist.key.contactList.phone = pref->getString(Opt::phonecontactlist_key_contact_list_phone_);
		opt.phonecontactlist.key.contactList.voiceraID = pref->getString(Opt::phonecontactlist_key_contact_list_voicera_id_);
		opt.phonecontactlist.key.contactList.SIPCallerID = pref->getString(Opt::phonecontactlist_key_contact_list_sip_caller_id_);
		opt.phonecontactlist.key.contactList.sourceLanguage = pref->getString(Opt::phonecontactlist_key_contact_list_source_language_);
		opt.phonecontactlist.key.contactList.targetLanguage = pref->getString(Opt::phonecontactlist_key_contact_list_target_language_);
		opt.phonecontactlist.key.contactList.onlineStatus = pref->getString(Opt::phonecontactlist_key_contact_list_online_status_);
		opt.phonecontactlist.key.contactList.systemUser = pref->getString(Opt::phonecontactlist_key_contact_list_system_user_);
	}

	if (force || paths.contains(Opt::phoneaddcontact_)) {
		opt.phoneaddcontact.url = pref->getString(Opt::phoneaddcontact_url_);
		opt.phoneaddcontact.query.voiceraID = pref->getString(Opt::phoneaddcontact_query_voicera_id_);
		opt.phoneaddcontact.query.name = pref->getString(Opt::phoneaddcontact_query_name_);
		opt.phoneaddcontact.query.phone = pref->getString(Opt::phoneaddcontact_query_phone_);
		opt.phoneaddcontact.query.phoneCountry = pref->getString(Opt::phoneaddcontact_query_phone_country_);
		opt.phoneaddcontact.query.sourceLanguage = pref->getString(Opt::phoneaddcontact_query_source_language_);
		opt.phoneaddcontact.query.targetLanguage = pref->getString(Opt::phoneaddcontact_query_target_language_);
		opt.phoneaddcontact.key.addBookID = pref->getString(Opt::phoneaddcontact_key_add_book_id_);
		opt.phoneaddcontact.key.ID = pref->getString(Opt::phoneaddcontact_key_id_);
	}

	if (force || paths.contains(Opt::phoneupdatecontact_)) {
		opt.phoneupdatecontact.url = pref->getString(Opt::phoneupdatecontact_url_);
		opt.phoneupdatecontact.query.addBookID = pref->getString(Opt::phoneupdatecontact_query_add_book_id_);
		opt.phoneupdatecontact.query.updateType = pref->getString(Opt::phoneupdatecontact_query_update_type_);
		opt.phoneupdatecontact.query.voiceraID = pref->getString(Opt::phoneupdatecontact_query_voicera_id_);
		opt.phoneupdatecontact.query.name = pref->getString(Opt::phoneupdatecontact_query_name_);
		opt.phoneupdatecontact.query.phone = pref->getString(Opt::phoneupdatecontact_query_phone_);
		opt.phoneupdatecontact.query.oldPhone = pref->getString(Opt::phoneupdatecontact_query_old_phone_);
		opt.phoneupdatecontact.query.phoneCountry = pref->getString(Opt::phoneupdatecontact_query_phone_country_);
		opt.phoneupdatecontact.query.deletePhone = pref->getString(Opt::phoneupdatecontact_query_delete_phone_);
		opt.phoneupdatecontact.query.sourceLanguage = pref->getString(Opt::phoneupdatecontact_query_source_language_);
		opt.phoneupdatecontact.query.targetLanguage = pref->getString(Opt::phoneupdatecontact_query_target_language_);
	}

	if (force || paths.contains(Opt::phonedelcontact_)) {
		opt.phonedelcontact.url = pref->getString(Opt::phonedelcontact_url_);
		opt.phonedelcontact.query.addBookID = pref->getString(Opt::phonedelcontact_query_add_book_id_);
	}

	if (force || paths.contains(Opt::phonedetailcontact_)) {
		opt.phonedetailcontact.url = pref->getString(Opt::phonedetailcontact_url_);
		opt.phonedetailcontact.query.addBookID = pref->getString(Opt::phonedetailcontact_query_add_book_id_);
		opt.phonedetailcontact.key.detailInfo = pref->getString(Opt::phonedetailcontact_key_info_);
		opt.phonedetailcontact.key.info.nativeLanguage = pref->getString(Opt::phonedetailcontact_key_info_native_language_);
		opt.phonedetailcontact.key.info.nickname = pref->getString(Opt::phonedetailcontact_key_info_nickname_);
		opt.phonedetailcontact.key.info.country = pref->getString(Opt::phonedetailcontact_key_info_country_);
		opt.phonedetailcontact.key.info.major = pref->getString(Opt::phonedetailcontact_key_info_major_);
	}

	if (force || paths.contains(Opt::phonegethistorycdr_)) {
		opt.phonegethistorycdr.url = pref->getString(Opt::phonegethistorycdr_url_);
		opt.phonegethistorycdr.query.historyType = pref->getString(Opt::phonegethistorycdr_query_history_type_);
		opt.phonegethistorycdr.key.cdrlist = pref->getString(Opt::phonegethistorycdr_key_list_);
		opt.phonegethistorycdr.key.list.ID = pref->getString(Opt::phonegethistorycdr_key_list_id_);
		opt.phonegethistorycdr.key.list.meetCDRID = pref->getString(Opt::phonegethistorycdr_key_list_meet_cdr_id_);
		opt.phonegethistorycdr.key.list.calleeID = pref->getString(Opt::phonegethistorycdr_key_list_callee_id_);
		opt.phonegethistorycdr.key.list.sourceLanguage = pref->getString(Opt::phonegethistorycdr_key_list_source_language_);
		opt.phonegethistorycdr.key.list.targetLanguage = pref->getString(Opt::phonegethistorycdr_key_list_target_language_);
		opt.phonegethistorycdr.key.list.callingTime = pref->getString(Opt::phonegethistorycdr_key_list_calling_time_);
		opt.phonegethistorycdr.key.list.cost = pref->getString(Opt::phonegethistorycdr_key_list_cost_);
		opt.phonegethistorycdr.key.list.isSuccess = pref->getString(Opt::phonegethistorycdr_key_list_is_success_);
		opt.phonegethistorycdr.key.list.isCallout = pref->getString(Opt::phonegethistorycdr_key_list_is_callout_);
	}

	if (force || paths.contains(Opt::phonegetuserinfo_)) {
		opt.phonegetuserinfo.url = pref->getString(Opt::phonegetuserinfo_url_);
		opt.phonegetuserinfo.query.voiceraID = pref->getString(Opt::phonegetuserinfo_query_voice_id_);
		opt.phonegetuserinfo.key.info.email = pref->getString(Opt::phonegetuserinfo_key_info_email_);
		opt.phonegetuserinfo.key.info.voiceraID = pref->getString(Opt::phonegetuserinfo_key_info_voicera_id_);
		opt.phonegetuserinfo.key.info.sipCallerID = pref->getString(Opt::phonegetuserinfo_key_info_sip_caller_id_);
		opt.phonegetuserinfo.key.info.accountBalance = pref->getString(Opt::phonegetuserinfo_key_info_account_balance_);
		opt.phonegetuserinfo.key.info.nickname = pref->getString(Opt::phonegetuserinfo_key_info_nickname_);
		opt.phonegetuserinfo.key.info.nativeLanguage = pref->getString(Opt::phonegetuserinfo_key_info_native_language_);
		opt.phonegetuserinfo.key.info.major = pref->getString(Opt::phonegetuserinfo_key_info_major_);
		opt.phonegetuserinfo.key.info.country = pref->getString(Opt::phonegetuserinfo_key_info_country_);
		opt.phonegetuserinfo.key.info.smallPhoto = pref->getString(Opt::phonegetuserinfo_key_info_small_photo_);
		opt.phonegetuserinfo.key.info.largePhoto = pref->getString(Opt::phonegetuserinfo_key_info_large_photo_);
		opt.phonegetuserinfo.key.info.onlineStatus = pref->getString(Opt::phonegetuserinfo_key_info_online_status_);
	}

	if (force || paths.contains(Opt::phoneedituserinfo_)) {
		opt.phoneedituserinfo.url = pref->getString(Opt::phoneedituserinfo_url_);
		opt.phoneedituserinfo.query.nickname = pref->getString(Opt::phoneedituserinfo_query_nickname_);
		opt.phoneedituserinfo.query.nativeLanguage = pref->getString(Opt::phoneedituserinfo_query_native_language_);
		opt.phoneedituserinfo.query.nationality = pref->getString(Opt::phoneedituserinfo_query_nationality_);
		opt.phoneedituserinfo.query.major = pref->getString(Opt::phoneedituserinfo_query_major_);
		opt.phoneedituserinfo.query.onlineVisibleStatus = pref->getString(Opt::phoneedituserinfo_query_online_visible_status_);
		opt.phoneedituserinfo.query.photo = pref->getString(Opt::phoneedituserinfo_query_photo_);
		opt.phoneedituserinfo.query.updateType = pref->getString(Opt::phoneedituserinfo_query_update_type_);
	}

	if (force || paths.contains(Opt::phonechangepassword_)) {
		opt.phonechangepassword.url = pref->getString(Opt::phonechangepassword_url_);
		opt.phonechangepassword.query.authCode = pref->getString(Opt::phonechangepassword_query_auth_code_);
		opt.phonechangepassword.query.email = pref->getString(Opt::phonechangepassword_query_email_);
	}

	if (force || paths.contains(Opt::phonenewpassword_)) {
		opt.phonenewpassword.url = pref->getString(Opt::phonenewpassword_url_);
		opt.phonenewpassword.query.newPassword1 = pref->getString(Opt::phonenewpassword_query_new_password1_);
		opt.phonenewpassword.query.newPassword2 = pref->getString(Opt::phonenewpassword_query_new_password2_);
		opt.phonenewpassword.query.authCode = pref->getString(Opt::phonenewpassword_query_auth_code_);
		opt.phonenewpassword.query.email = pref->getString(Opt::phonenewpassword_query_email_);
	}

	if (force || paths.contains(Opt::phonegetcdrid_)) {
		opt.phonegetcdrid.url = pref->getString(Opt::phonegetcdrid_url_);
		opt.phonegetcdrid.query.CDRID = pref->getString(Opt::phonegetcdrid_query_cdr_id_);
		opt.phonegetcdrid.key.info.ID = pref->getString(Opt::phonegetcdrid_key_info_id_);
		opt.phonegetcdrid.key.info.callerID = pref->getString(Opt::phonegetcdrid_key_info_caller_id_);
		opt.phonegetcdrid.key.info.deductionTime = pref->getString(Opt::phonegetcdrid_key_info_deduction_time_);
		opt.phonegetcdrid.key.info.meetCDRID = pref->getString(Opt::phonegetcdrid_key_info_meet_cdr_id_);
		opt.phonegetcdrid.key.info.callTime = pref->getString(Opt::phonegetcdrid_key_info_call_time_);
		opt.phonegetcdrid.key.info.contactName = pref->getString(Opt::phonegetcdrid_key_info_contact_name_);
		opt.phonegetcdrid.key.info.smallPhoto = pref->getString(Opt::phonegetcdrid_key_info_small_photo_);
		opt.phonegetcdrid.key.info.largePhoto = pref->getString(Opt::phonegetcdrid_key_info_large_photo_);
		opt.phonegetcdrid.key.info.callStatus = pref->getString(Opt::phonegetcdrid_key_info_call_status_);
		opt.phonegetcdrid.key.info.callMode = pref->getString(Opt::phonegetcdrid_key_info_call_mode_);
		opt.phonegetcdrid.key.info.deductionCost = pref->getString(Opt::phonegetcdrid_key_info_deduction_cost_);
		opt.phonegetcdrid.key.info.sourceLanguage = pref->getString(Opt::phonegetcdrid_key_info_source_language_);
		opt.phonegetcdrid.key.info.targetLanguage = pref->getString(Opt::phonegetcdrid_key_info_target_language_);
		opt.phonegetcdrid.key.info.PSTNCountry = pref->getString(Opt::phonegetcdrid_key_info_pstn_country_);
		opt.phonegetcdrid.key.info.voiceraID = pref->getString(Opt::phonegetcdrid_key_info_voicera_id_);
	}

	if (force || paths.contains(Opt::phonedelcdr_)) {
		opt.phonedelcdr.url = pref->getString(Opt::phonedelcdr_url_);
		opt.phonedelcdr.query.meetCDRID = pref->getString(Opt::phonedelcdr_query_meet_cdr_id_);
	}
}

UserService* CommandService::userService() const
{
    Q_D(const CommandService);
    return d->m_userService;
}

void CommandService::setUserService(UserService* userService)
{
    Q_D(CommandService);

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

void CommandService::setCommonItem(const QString& client, const QString& version, const QString& language, const QString& deviceId, const QString& deviceModel)
{
	Q_D(CommandService);

	d->m_client = client;
	d->m_version = version;
	d->m_language = language;
	d->m_deviceId = deviceId;
	d->m_deviceModel = deviceModel;
}

QList<QNetworkCookie> CommandService::cookies() const 
{
	Q_D(const CommandService);

	return d->cookies();
}

void CommandService::setCookies(const QList<QNetworkCookie>& cookies)
{
	Q_D(CommandService);

	d->setCookies(cookies);
}

QString CommandService::token() const 
{
	Q_D(const CommandService);

	return d->token();
}

void CommandService::setToken(const QString& token)
{
	Q_D(CommandService);

	return d->setToken(token);
}

QString CommandService::deviceId() const 
{
	Q_D(const CommandService);

	return d->m_deviceId;
}

void CommandService::loadconfig(const Command::LoadConfigParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->loadconfig(param);
}

void CommandService::getserver(const Command::GetServerParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->getserver(param);
}

void CommandService::login(const Command::LoginParam& param)
{
    Q_D(CommandService);

    if (!isActive()) {
        qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
        return;
    }

    d->login(param);
}

void CommandService::login2(const Command::LoginParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->login2(param);
}

void CommandService::setclientinfo(const Command::SetClientInfoParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->setclientinfo(param);
}

void CommandService::todayStat(const Command::TodayStatParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->todaystat(param);
}

void CommandService::getnoreportinfo(const Command::GetNoReportParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->getnoreportinfo(param);
}

void CommandService::getcdrlist(const Command::GetCDRListParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->getcdrlist(param);
}

void CommandService::getcdrinfo(const Command::GetCDRInfoParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->getcdrinfo(param);
}

void CommandService::getuserinfo(const Command::GetUserInfoParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->getuserinfo(param);
}

void CommandService::getcalleruser(const Command::GetCallerUserParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->getcalleruser(param);
}

void CommandService::savescreenshot(const Command::SaveScreenShotParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->savescreenshot(param);
}

void CommandService::forgetpassword(const Command::ForgetPasswordParam& param)
{
    Q_D(CommandService);

//     if (!isActive()) {
//         qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
//         return;
//     }

    d->forgetpassword(param);
}

void CommandService::createaccount(const Command::CreateAccountParam& param)
{
	Q_D(CommandService);

// 	if (!isActive()) {
// 		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
// 		return;
// 	}

	d->createaccount(param);
}

void CommandService::changestatus(const Command::ChangeStatusParam& param)
{
    Q_D(CommandService);

    if (!isActive()) {
        qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
        return;
    }

    d->changestatus(param);
}

void CommandService::getblacklist(const Command::GetBlacklistParam& param)
{

	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->getblacklist(param);
}

void CommandService::addblacklist(const Command::AddBlacklistParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->addblacklist(param);
}

void CommandService::delblacklist(const Command::DelBlacklistParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->delblacklist(param);
}

void CommandService::getmemberinfo(const Command::GetMemberInfoParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->getmemberinfo(param);
}

void CommandService::getuploadphoto(const Command::GetUploadPhotoParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->getuploadphoto(param);
}

void CommandService::version(const Command::VersionParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->version(param);
}


void  CommandService::phoneloadconfig(const Command::PhoneLoadConfigParam& param)
{
	Q_D(CommandService);

// 	if (!isActive()) {
// 		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
// 		return;
// 	}

	d->phoneloadconfig(param);
}

void  CommandService::phoneregister(const Command::PhoneRegisterParam& param)
{
	Q_D(CommandService);

// 	if (!isActive()) {
// 		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
// 		return;
// 	}

	d->phoneregister(param);
}

void  CommandService::phonelogin(const Command::PhoneLoginParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonelogin(param);
}

void  CommandService::phoneforgetpassword(const Command::PhoneForgetPasswordParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phoneforgetpassword(param);
}

void  CommandService::phonecontactlist(const Command::PhoneContactListParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonecontactlist(param);
}

void  CommandService::phoneaddcontact(const Command::PhoneAddContactParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phoneaddcontact(param);
}

void  CommandService::phoneupdatecontact(const Command::PhoneUpdateContactParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phoneupdatecontact(param);
}

void  CommandService::phonedelcontact(const Command::PhoneDeleteContactParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonedelcontact(param);
}

void  CommandService::phonedetailcontact(const Command::PhoneDetailContactParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonedetailcontact(param);
}

void  CommandService::phonegethistorycdr(const Command::PhoneGetHistoryCDRParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonegethistorycdr(param);
}

void  CommandService::phonegetuserinfo(const Command::PhoneGetUserInfoParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonegetuserinfo(param);
}

void  CommandService::phoneedituserinfo(const Command::PhoneEditUserInfoParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phoneedituserinfo(param);
}

void  CommandService::phonechangepassword(const Command::PhoneChangePasswordParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonechangepassword(param);
}

void  CommandService::phonenewpassword(const Command::PhoneNewPasswordParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonenewpassword(param);
}

void  CommandService::phonegetcdrid(const Command::PhoneGetCDRIDParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonegetcdrid(param);
}

void  CommandService::phonedelcdr(const Command::PhoneDeleteCDRParam& param)
{
	Q_D(CommandService);

	if (!isActive()) {
		qDebug() << DEBUG_PREFIX << "command rejected because of invalid state";
		return;
	}

	d->phonedelcdr(param);
}

} // namespace ns

#include "moc_commandservice.cpp"
