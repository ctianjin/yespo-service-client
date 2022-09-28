#ifndef COMMANDSERVICE_P_H
#define COMMANDSERVICE_P_H

#include <QByteArray>
#include <QPointer>
#include <QStringList>
#include "commandservice.h"
#include "abstractservice_p.h"
#include "downloadqueue_p.h"

namespace ns {

struct CommandServiceOptions
{
    static const QString common_;
    static const QString common_query_session_id_;
	static const QString common_query_sign_;
	static const QString common_query_timestamp_;
	static const QString common_query_client_;
	static const QString common_query_version_;
	static const QString common_query_language_;
	static const QString common_query_device_id_;
	static const QString common_query_device_model_;
	static const QString common_query_token_;
    static const QString common_key_error_code_;
    static const QString common_key_error_message_;
	static const QString common_key_error_number_;
	static const QString common_key_error_tips_;
	static const QString common_key_result_;

	static const QString loadconfig_;
	static const QString loadconfig_url_;
	static const QString loadconfig_query_last_time_;
	static const QString loadconfig_query_reload_;
	static const QString loadconfig_key_config_;
	static const QString loadconfig_key_client_;

	static const QString getserver_;
	static const QString getserver_url_;
	static const QString getserver_key_domain_status_;
	static const QString getserver_key_domain_call_;
	static const QString getserver_key_domain_im_;

    static const QString login_;
    static const QString login_url_;
    static const QString login_encrypt_key_;
    static const QString login_query_user_name_;
    static const QString login_query_password_;
	static const QString login_key_user_;
	static const QString login_key_user_system_id_;
	static const QString login_key_user_caller_id_;
	static const QString login_key_user_login_id_;
	static const QString login_key_user_secret_;
	static const QString login_key_user_display_name_;
	static const QString login_key_user_skill_;
	static const QString login_key_user_skill_language_1_;
	static const QString login_key_user_skill_language_2_;
	static const QString login_key_user_skill_is_free_;
	static const QString login_key_user_skill_level_;
	static const QString login_key_user_local_;
	static const QString login_key_user_local_country_;
	static const QString login_key_user_local_language_;
	static const QString login_key_user_small_photo_;
	static const QString login_key_user_large_photo_;
	static const QString login_key_user_token_;
	static const QString login_key_server_ip_;
	static const QString login_key_server_ip_domain_status_;
	static const QString login_key_server_ip_domain_call_;
	static const QString login_key_server_ip_domain_im_;
	static const QString login_key_servers_;
	static const QString login_key_servers_domain_status_;
	static const QString login_key_servers_domain_call_;
	static const QString login_key_servers_domain_im_;

	static const QString setclientinfo_;
	static const QString setclientinfo_url_;
	static const QString setclientinfo_query_last_domain_call_;

	static const QString todaystat_;
	static const QString todaystat_url_;
	static const QString todaystat_key_answer_;
	static const QString todaystat_key_success_;
	static const QString todaystat_key_noanswer_;

	static const QString getnoreportinfo_;
	static const QString getnoreportinfo_url_;
	static const QString getnoreportinfo_key_no_report_num_;

	static const QString getcdrlist_;
	static const QString getcdrlist_url_;
	static const QString getcdrlist_query_id_;
	static const QString getcdrlist_query_type_;
	static const QString getcdrlist_key_id_;
	static const QString getcdrlist_key_ve_sys_id_;
	static const QString getcdrlist_key_meet_cdr_id_;
	static const QString getcdrlist_key_small_photo_;
	static const QString getcdrlist_key_displayname_;
	static const QString getcdrlist_key_start_call_time_;
	static const QString getcdrlist_key_call_time_;
	static const QString getcdrlist_key_is_answered_;
	static const QString getcdrlist_key_call_mode_;
	static const QString getcdrlist_key_is_success_;

	static const QString getcdrinfo_;
	static const QString getcdrinfo_url_;
	static const QString getcdrinfo_query_meet_cdr_id_;
	static const QString getcdrinfo_key_cdr_type_;
	static const QString getcdrinfo_key_caller_ve_sys_id_;
	static const QString getcdrinfo_key_caller_id_;
	static const QString getcdrinfo_key_caller_ve_login_id_;
	static const QString getcdrinfo_key_caller_display_name_;
	static const QString getcdrinfo_key_caller_mobile_;
	static const QString getcdrinfo_key_caller_small_photo_;
	static const QString getcdrinfo_key_caller_large_photo_;
	static const QString getcdrinfo_key_is_pstn_call_;
	static const QString getcdrinfo_key_call_country_;
	static const QString getcdrinfo_key_callee_ve_sys_id_;
	static const QString getcdrinfo_key_callee_id_;
	static const QString getcdrinfo_key_callee_ve_login_id_;
	static const QString getcdrinfo_key_callee_display_name_;
	static const QString getcdrinfo_key_callee_mobile_;
	static const QString getcdrinfo_key_callee_small_photo_;
	static const QString getcdrinfo_key_callee_large_photo_;
	static const QString getcdrinfo_key_partner_ve_sys_id_;
	static const QString getcdrinfo_key_partner_id_;
	static const QString getcdrinfo_key_partner_login_id_;
	static const QString getcdrinfo_key_partner_display_name_;
	static const QString getcdrinfo_key_partner_small_photo_;
	static const QString getcdrinfo_key_partner_large_photo_;
	static const QString getcdrinfo_key_is_free_;
	static const QString getcdrinfo_key_partner_level_;
	static const QString getcdrinfo_key_language_1_;
	static const QString getcdrinfo_key_language_2_;
	static const QString getcdrinfo_key_call_time_;
	static const QString getcdrinfo_key_start_call_time_;

	static const QString getuserinfo_;
	static const QString getuserinfo_url_;
	static const QString getuserinfo_key_system_id_;
	static const QString getuserinfo_key_caller_id_;
	static const QString getuserinfo_key_login_id_;
	static const QString getuserinfo_key_secret_;
	static const QString getuserinfo_key_display_name_;
	static const QString getuserinfo_key_gender_;
	static const QString getuserinfo_key_birthday_;
	static const QString getuserinfo_key_nationality_;
	static const QString getuserinfo_key_education_;
	static const QString getuserinfo_key_major_;
	static const QString getuserinfo_key_intro_;
	static const QString getuserinfo_key_skill_;
	static const QString getuserinfo_key_skill_language_1_;
	static const QString getuserinfo_key_skill_language_2_;
	static const QString getuserinfo_key_skill_is_free_;
	static const QString getuserinfo_key_skill_level_;
	static const QString getuserinfo_key_small_photo_;
	static const QString getuserinfo_key_large_photo_;

	static const QString getcalleruser_;
	static const QString getcalleruser_url_;
	static const QString getcalleruser_query_caller_id_;
	static const QString getcalleruser_key_small_photo_;
	static const QString getcalleruser_key_large_photo_;
	static const QString getcalleruser_key_display_name_;
	static const QString getcalleruser_key_mobile_;

	static const QString savescreenshot_;
	static const QString savescreenshot_url_;
	static const QString savescreenshot_query_meet_cdr_id_;
	static const QString savescreenshot_query_photo_;

	static const QString forgetpassword_;
	static const QString forgetpassword_url_;
	static const QString forgetpassword_query_user_name_;
	static const QString createaccount_;
	static const QString createaccount_url_;

	static const QString changestatus_;
	static const QString changestatus_url_;
	static const QString changestatus_query_online_visible_status_;

	static const QString getblacklist_;
	static const QString getblacklist_url_;
	static const QString getblacklist_query_id_;
	static const QString getblacklist_key_id_;
	static const QString getblacklist_key_blacklist_id_;
	static const QString getblacklist_key_partner_sys_id_;
	static const QString getblacklist_key_ve_sys_id_;
	static const QString getblacklist_key_display_name_;
	static const QString getblacklist_key_small_photo_;

	static const QString addblacklist_;
	static const QString addblacklist_url_;
	static const QString addblacklist_query_ve_sys_id_;
	static const QString addblacklist_key_id_;
	static const QString addblacklist_key_blacklist_id_;
	static const QString addblacklist_key_partner_sys_id_;
	static const QString addblacklist_key_ve_sys_id_;
	static const QString addblacklist_key_display_name_;
	static const QString addblacklist_key_small_photo_;

	static const QString delblacklist_;
	static const QString delblacklist_url_;
	static const QString delblacklist_query_ve_sys_id_;

	static const QString getmemberinfo_;
	static const QString getmemberinfo_url_;
	static const QString getmemberinfo_query_ve_sys_id_;
	static const QString getmemberinfo_key_ve_sys_id_;
	static const QString getmemberinfo_key_ve_login_id_;
	static const QString getmemberinfo_key_caller_id_;
	static const QString getmemberinfo_key_display_name_;
	static const QString getmemberinfo_key_native_language_;
	static const QString getmemberinfo_key_country_;
	static const QString getmemberinfo_key_major_;
	static const QString getmemberinfo_key_small_photo_;

	static const QString getuploadphoto_;
	static const QString getuploadphoto_url_;
	static const QString getuploadphoto_query_caller_id_;
	static const QString getuploadphoto_query_photo_id;
	static const QString getuploadphoto_key_photo_id;
	static const QString getuploadphoto_key_expire;
	static const QString getuploadphoto_key_small_photo_;
	static const QString getuploadphoto_key_large_photo_;

	static const QString version_;
	static const QString version_url_;
	static const QString version_key_number_;
	static const QString version_key_link_;

	static const QString phoneloadconfig_;
	static const QString phoneloadconfig_url_;
	static const QString phoneloadconfig_query_last_time_;
	static const QString phoneloadconfig_query_reload_;
	static const QString phoneloadconfig_key_info_;
	static const QString phoneloadconfig_key_info_service_;
	static const QString phoneloadconfig_key_info_major_;
	static const QString phoneloadconfig_key_info_useapp_;
	static const QString phoneloadconfig_key_info_mode_;
	static const QString phoneloadconfig_key_version_;
	static const QString phoneloadconfig_key_download_;
	static const QString phoneloadconfig_key_force_update_;
	static const QString phoneloadconfig_key_verify_code_;
	static const QString phoneloadconfig_key_version_name_;
	static const QString phoneloadconfig_key_pstn_supported_;
	static const QString phoneloadconfig_key_href_info_;
	static const QString phoneloadconfig_key_href_info_create_account_;
	static const QString phoneloadconfig_key_href_info_change_password_;
	static const QString phoneloadconfig_key_href_info_edit_userinfo_;
	static const QString phoneregister_;
	static const QString phoneregister_url_;
	static const QString phoneregister_query_native_language_;
	static const QString phoneregister_query_voicera_id_;
	static const QString phoneregister_query_email_;
	static const QString phoneregister_query_password_;
	static const QString phoneregister_query_photo_;
	static const QString phoneregister_key_result_;
	static const QString phoneregister_key_result_sip_caller_id_;
	static const QString phoneregister_key_result_nickname_;
	static const QString phoneregister_key_result_voicera_id_;
	static const QString phoneregister_key_result_native_language_;
	static const QString phoneregister_key_result_small_photo_;
	static const QString phoneregister_key_result_large_photo_;
	static const QString phonelogin_;
	static const QString phonelogin_url_;
	static const QString phonelogin_query_voicera_id_;
	static const QString phonelogin_query_password_;
	static const QString phonelogin_key_result_;
	static const QString phonelogin_key_result_sip_caller_id_;
	static const QString phonelogin_key_result_native_language_;
	static const QString phonelogin_key_result_voicera_id_;
	static const QString phonelogin_key_result_small_photo_;
	static const QString phonelogin_key_result_large_photo_;
	static const QString phoneforgetpassword_;
	static const QString phoneforgetpassword_url_;
	static const QString phoneforgetpassword_query_email_;
	static const QString phonecontactlist_;
	static const QString phonecontactlist_url_;
	static const QString phonecontactlist_query_search_type_;
	static const QString phonecontactlist_key_contact_list_;
	static const QString phonecontactlist_key_contact_list_id_;
	static const QString phonecontactlist_key_contact_list_add_book_id_;
	static const QString phonecontactlist_key_contact_list_contact_name_;
	static const QString phonecontactlist_key_contact_list_small_photo_;
	static const QString phonecontactlist_key_contact_list_large_photo_;
	static const QString phonecontactlist_key_contact_list_phone_;
	static const QString phonecontactlist_key_contact_list_voicera_id_;
	static const QString phonecontactlist_key_contact_list_sip_caller_id_;
	static const QString phonecontactlist_key_contact_list_source_language_;
	static const QString phonecontactlist_key_contact_list_target_language_;
	static const QString phonecontactlist_key_contact_list_online_status_;
	static const QString phonecontactlist_key_contact_list_system_user_;
	static const QString phoneaddcontact_;
	static const QString phoneaddcontact_url_;
	static const QString phoneaddcontact_query_voicera_id_;
	static const QString phoneaddcontact_query_name_;
	static const QString phoneaddcontact_query_phone_;
	static const QString phoneaddcontact_query_phone_country_;
	static const QString phoneaddcontact_query_source_language_;
	static const QString phoneaddcontact_query_target_language_;
	static const QString phoneaddcontact_key_add_book_id_;
	static const QString phoneaddcontact_key_id_;
	static const QString phoneupdatecontact_;
	static const QString phoneupdatecontact_url_;
	static const QString phoneupdatecontact_query_add_book_id_;
	static const QString phoneupdatecontact_query_update_type_;
	static const QString phoneupdatecontact_query_voicera_id_;
	static const QString phoneupdatecontact_query_name_;
	static const QString phoneupdatecontact_query_phone_;
	static const QString phoneupdatecontact_query_old_phone_;
	static const QString phoneupdatecontact_query_phone_country_;
	static const QString phoneupdatecontact_query_delete_phone_;
	static const QString phoneupdatecontact_query_source_language_;
	static const QString phoneupdatecontact_query_target_language_;
	static const QString phonedelcontact_;
	static const QString phonedelcontact_url_;
	static const QString phonedelcontact_query_add_book_id_;
	static const QString phonedetailcontact_;
	static const QString phonedetailcontact_url_;
	static const QString phonedetailcontact_query_add_book_id_;
	static const QString phonedetailcontact_key_info_;
	static const QString phonedetailcontact_key_info_native_language_;
	static const QString phonedetailcontact_key_info_nickname_;
	static const QString phonedetailcontact_key_info_country_;
	static const QString phonedetailcontact_key_info_major_;
	static const QString phonegethistorycdr_;
	static const QString phonegethistorycdr_url_;
	static const QString phonegethistorycdr_query_history_type_;
	static const QString phonegethistorycdr_key_list_;
	static const QString phonegethistorycdr_key_list_id_;
	static const QString phonegethistorycdr_key_list_meet_cdr_id_;
	static const QString phonegethistorycdr_key_list_callee_id_;
	static const QString phonegethistorycdr_key_list_source_language_;
	static const QString phonegethistorycdr_key_list_target_language_;
	static const QString phonegethistorycdr_key_list_calling_time_;
	static const QString phonegethistorycdr_key_list_cost_;
	static const QString phonegethistorycdr_key_list_is_success_;
	static const QString phonegethistorycdr_key_list_is_callout_;
	static const QString phonegetuserinfo_;
	static const QString phonegetuserinfo_url_;
	static const QString phonegetuserinfo_query_voice_id_;
	static const QString phonegetuserinfo_key_info_;
	static const QString phonegetuserinfo_key_info_email_;
	static const QString phonegetuserinfo_key_info_voicera_id_;
	static const QString phonegetuserinfo_key_info_sip_caller_id_;
	static const QString phonegetuserinfo_key_info_account_balance_;
	static const QString phonegetuserinfo_key_info_nickname_;
	static const QString phonegetuserinfo_key_info_native_language_;
	static const QString phonegetuserinfo_key_info_major_;
	static const QString phonegetuserinfo_key_info_country_;
	static const QString phonegetuserinfo_key_info_small_photo_;
	static const QString phonegetuserinfo_key_info_large_photo_;
	static const QString phonegetuserinfo_key_info_online_status_;
	static const QString phoneedituserinfo_;
	static const QString phoneedituserinfo_url_;
	static const QString phoneedituserinfo_query_nickname_;
	static const QString phoneedituserinfo_query_native_language_;
	static const QString phoneedituserinfo_query_nationality_;
	static const QString phoneedituserinfo_query_major_;
	static const QString phoneedituserinfo_query_online_visible_status_;
	static const QString phoneedituserinfo_query_photo_;
	static const QString phoneedituserinfo_query_update_type_;
	static const QString phonechangepassword_;
	static const QString phonechangepassword_url_;
	static const QString phonechangepassword_query_auth_code_;
	static const QString phonechangepassword_query_email_;
	static const QString phonenewpassword_;
	static const QString phonenewpassword_url_;
	static const QString phonenewpassword_query_new_password1_;
	static const QString phonenewpassword_query_new_password2_;
	static const QString phonenewpassword_query_auth_code_;
	static const QString phonenewpassword_query_email_;
	static const QString phonegetcdrid_;
	static const QString phonegetcdrid_url_;
	static const QString phonegetcdrid_query_cdr_id_;
	static const QString phonegetcdrid_key_info_;
	static const QString phonegetcdrid_key_info_id_;
	static const QString phonegetcdrid_key_info_caller_id_;
	static const QString phonegetcdrid_key_info_deduction_time_;
	static const QString phonegetcdrid_key_info_meet_cdr_id_;
	static const QString phonegetcdrid_key_info_call_time_;
	static const QString phonegetcdrid_key_info_contact_name_;
	static const QString phonegetcdrid_key_info_small_photo_;
	static const QString phonegetcdrid_key_info_large_photo_;
	static const QString phonegetcdrid_key_info_call_status_;
	static const QString phonegetcdrid_key_info_call_mode_;
	static const QString phonegetcdrid_key_info_deduction_cost_;
	static const QString phonegetcdrid_key_info_source_language_;
	static const QString phonegetcdrid_key_info_target_language_;
	static const QString phonegetcdrid_key_info_pstn_country_;
	static const QString phonegetcdrid_key_info_voicera_id_;
	static const QString phonedelcdr_;
	static const QString phonedelcdr_url_;
	static const QString phonedelcdr_query_meet_cdr_id_;

    struct
    {
        struct
        {
            QString sessionId;
			QString sign;
			QString timestamp;
			QString client;
			QString version;
			QString language;
			QString deviceId;
			QString deviceModel;
			QString token;
        } query;

        struct
        {
            QString errorCode;
            QString errorMessage;
			QString errorNumber;
			QString errorTips;
			QString result;
        } key;
    } common;

	struct
	{
		QString url;
		
		struct
		{
			QString lastTime;
			QString reload;
		} query;

		struct
		{
			QString config;
			QString client;
		} key;

	} loadconfig;

	struct 
	{
		QString url;

		struct
		{
			QString domain_status;
			QString domain_call;
			QString domain_im;
		} key;
	}getserver;

    struct
    {
        QString url;
        QString encryptKey;

        struct
        {
            QString userName;
            QString password;
        } query;

        struct
        {
			QString user;
			struct {
				QString systemId;
				QString callerId;
				QString loginId;
				QString secret;
				QString displayName;
				QString skill;
				struct {
					QString language1;
					QString language2;
					QString isFree;
					QString level;
				}skillInfo;

				QString local;
				struct{
					QString country;
					QString language;
				}localInfo;

				QString smallPhoto;
				QString largePhoto;
				QString token;
			}userInfo;   
			QString serverIp;
			struct{
				QString domainStatus;
				QString domainCall;
				QString domainIm;
			}serverIpInfo;
			QString servers;
			struct{
				QString domainStatus;
				QString domainCall;
				QString domainIm;
			}serverListInfo;
        } key;
    } login;

	struct
	{
		QString url;

		struct
		{
			QString lastDomainCall;
		} query;

	} setclientinfo;

	struct
	{
		QString url;

		struct
		{
			QString answer;
			QString success;
			QString noanswer;
		} key;
	} todaystat;

	struct
	{
		QString url;

		struct
		{
			QString num;
		} key;
	} getnoreportinfo;

	struct
	{
		QString url;

		struct
		{
			QString id;
			QString type;
		} query;

		struct
		{
			QString id;
			QString VESysId;
			QString meetCDRId;
			QString smallPhoto;
			QString displayName;
			QString startCallTime;
			QString callTime;
			QString isAnswered;
			QString callMode;
			QString isSuccess;
		} key;
	} getcdrlist;

	struct
	{
		QString url;

		struct
		{
			QString meetCDRId;
		} query;

		struct
		{
			QString CDRType;
			QString callerVESysId;
			QString callerId;
			QString callerVELoginId;
			QString callerDisplayName;
			QString callerMobile;
			QString callerSmallPhoto;
			QString callerLargePhoto;
			QString isPSTNCall;
			QString callCountry;
			QString calleeVESysId;
			QString calleeId;
			QString calleeVELoginId;
			QString calleeDisplayName;
			QString calleeMobile;
			QString calleeSmallPhoto;
			QString calleeLargePhoto;
			QString partnerVESysId;
			QString partnerId;
			QString partnerLoginId;
			QString partnerDisplayName;
			QString partnerSmallPhoto;
			QString partnerLargePhoto;
			QString isFree;
			QString partnerLevel;
			QString language1;
			QString language2;
			QString callTime;
			QString startCallTime;
		} key;
	} getcdrinfo;

	struct
	{
		QString url;

		struct
		{
			QString systemId;
			QString callerId;
			QString loginId;
			QString secret;
			QString displayName;
			QString gender;
			QString birthday;
			QString nationality;
			QString education;
			QString major;
			QString intro;
			QString skill;
			struct {
				QString language1;
				QString language2;
				QString isFree;
				QString level;
			}skillInfo;
			QString smallPhoto;
			QString largePhoto;
		} key;
	} getuserinfo;

	struct
	{
		QString url;

		struct
		{
			QString callerId;
		}query;

		struct
		{
			QString smallPhoto;
			QString largePhoto;
			QString displayName;
			QString mobile;
		} key;
	} getcalleruser;

	struct
	{
		QString url;

		struct
		{
			QString meetCDRId;
			QString photo;
		}query;

	} savescreenshot;

    struct
    {
        QString url;

        struct
        {
            QString userName;
        } query;
    } forgetpassword;

	struct
	{
		QString url;
	} createaccount;

	struct
	{
		QString url;

		struct
		{
			QString onlineVisibleStatus;
		} query;

	}changestatus;

	struct
	{
		QString url;

		struct
		{
			QString id;
		}query;

		struct
		{
			QString id;
			QString blacklistId;
			QString partnerSysId;
			QString VESysId;
			QString displayName;
			QString smallPhoto;
		}key;
	}getblacklist;

	struct
	{
		QString url;

		struct
		{
			QString VESysId;
		}query;

		struct
		{
			QString id;
			QString blacklistId;
			QString partnerSysId;
			QString VESysId;
			QString displayName;
			QString smallPhoto;
		}key;
	}addblacklist;

	struct
	{
		QString url;

		struct
		{
			QString VESysId;
		}query;

	}delblacklist;

	struct
	{
		QString url;

		struct
		{
			QString VESysId;
		}query;

		struct
		{
			QString VESysId;
			QString VELoginId;
			QString callerId;
			QString displayName;
			QString nativeLanguage;
			QString country;
			QString major;
			QString smallPhoto;
		}key;

	}getmemberinfo;

	struct
	{
		QString url;

		struct
		{
			QString callerId;
			QString photoId;
		}query;

		struct
		{
			QString photoId;
			QString expire;
			QString smallPhoto;
			QString largePhoto;
		}key;
	}getuploadphoto;

	struct
	{
		QString url;

		struct
		{
			QString number;
			QString link;
		} key;

	}version;



	struct  
	{
		QString url;

		struct
		{
			QString lastTime;
			QString reload;
		}query;

		struct
		{
			QString configinfo;
			struct
			{
				QString service;
				QString major;
				QString useapp;
				QString mode;
			}info;
			
			QString version;
			QString download;
			QString forceUpdate;
			QString verifyCode;
			QString versionName;
			QString PSTNSupported;

			QString hrefinfo;
			struct
			{
				QString createAccount;
				QString changePassword;
				QString editUserInfo;
			}hinfo;

		}key;
	}phoneloadconfig;

	struct  
	{
		QString url;

		struct
		{
			QString nativeLanguage;
			QString voiceraID;
			QString email;
			QString password;
			QString photo;
		}query;

		struct
		{
			QString result;

			struct
			{
				QString SIPCallerID;
				QString nickname;
				QString voiceraID;
				QString nativeLanguage;
				QString smallPhoto;
				QString largePhoto;
			}resultinfo;
		}key;
	}phoneregister;

	struct  
	{
		QString url;

		struct
		{
			QString voiceraID;
			QString password;
		}query;

		struct
		{
			QString result;

			struct
			{
				QString SIPCallerID;
				QString nativeLanguage;
				QString voiceraID;
				QString smallPhoto;
				QString largePhoto;
			}resultinfo;

		}key;
	}phonelogin;

	struct  
	{
		QString url;

		struct
		{
			QString email;
		}query;
	}phoneforgetpassword;

	struct  
	{
		QString url;

		struct
		{
			QString searchType;
		}query;

		struct
		{
			QString list;

			struct
			{
				QString ID;
				QString addBookID;
				QString contactName;
				QString smallPhoto;
				QString largePhoto;
				QString phone;
				QString voiceraID;
				QString SIPCallerID;
				QString sourceLanguage;
				QString targetLanguage;
				QString onlineStatus;
				QString systemUser;
			}contactList;
		}key;
	}phonecontactlist;

	struct  
	{
		QString url;

		struct
		{
			QString voiceraID;
			QString name;
			QString phone;
			QString phoneCountry;
			QString sourceLanguage;
			QString targetLanguage;
		}query;

		struct
		{
			QString addBookID;
			QString ID;
		}key;
	}phoneaddcontact;

	struct  
	{
		QString url;

		struct
		{
			QString addBookID;
			QString updateType;
			QString voiceraID;
			QString name;
			QString phone;
			QString oldPhone;
			QString phoneCountry;
			QString deletePhone;
			QString sourceLanguage;
			QString targetLanguage;
		}query;
	}phoneupdatecontact;

	struct  
	{
		QString url;

		struct
		{
			QString addBookID;
		}query;
	}phonedelcontact;

	struct  
	{
		QString url;

		struct
		{
			QString addBookID;
		}query;

		struct
		{
			QString detailInfo;

			struct
			{
				QString nativeLanguage;
				QString nickname;
				QString country;
				QString major;
			}info;
		}key;
	}phonedetailcontact;

	struct  
	{
		QString url;

		struct
		{
			QString historyType;
		}query;

		struct
		{
			QString cdrlist;

			struct
			{
				QString ID;
				QString meetCDRID;
				QString calleeID;
				QString sourceLanguage;
				QString targetLanguage;
				QString callingTime;
				QString cost;
				QString isSuccess;
				QString isCallout;
			}list;
		}key;
	}phonegethistorycdr;

	struct  
	{
		QString url;

		struct
		{
			QString voiceraID;
		}query;

		struct
		{
			struct
			{
				QString email;
				QString voiceraID;
				QString sipCallerID;
				QString accountBalance;
				QString nickname;
				QString nativeLanguage;
				QString major;
				QString country;
				QString smallPhoto;
				QString largePhoto;
				QString onlineStatus;
			}info;
		}key;
	}phonegetuserinfo;

	struct  
	{
		QString url;

		struct
		{
			QString nickname;
			QString nativeLanguage;
			QString nationality;
			QString major;
			QString onlineVisibleStatus;
			QString photo;
			QString updateType;
		}query;
	}phoneedituserinfo;

	struct  
	{
		QString url;

		struct
		{
			QString authCode;
			QString email;
		}query;
	}phonechangepassword;

	struct  
	{
		QString url;

		struct
		{
			QString newPassword1;
			QString newPassword2;
			QString authCode;
			QString email;
		}query;
	}phonenewpassword;

	struct  
	{
		QString url;

		struct
		{
			QString CDRID;
		}query;

		struct
		{
			struct
			{
				QString ID;
				QString callerID;
				QString deductionTime;
				QString meetCDRID;
				QString callTime;
				QString contactName;
				QString smallPhoto;
				QString largePhoto;
				QString callStatus;
				QString callMode;
				QString deductionCost;
				QString sourceLanguage;
				QString targetLanguage;
				QString PSTNCountry;
				QString voiceraID;
			}info;
		}key;
	}phonegetcdrid;

	struct  
	{
		QString url;

		struct
		{
			QString meetCDRID;
		}query;
	}phonedelcdr;

    CommandServiceOptions();
};

class CommandServicePrivate : public AbstractServicePrivate
{
    Q_DECLARE_PUBLIC(CommandService)

public:
    static const QString serviceHostPlaceholder;
    static const QString tokenSeparator;
	static const QString loadconfigTokenPrefix;
    static const QString loginTokenPrefix;
	static const QString serverTokenPrefix;
	static const QString setclientinfoTokenPrefix;
	static const QString todaystayTokenPrefix;
	static const QString getnoreportinfoTokenPrefix;
	static const QString getcdrlistTokenPrefix;
	static const QString getcdrinfoTokenPrefix;
	static const QString getuserinfoTokenPrefix;
	static const QString getcalleruserTokenPrefix;
	static const QString savescreenshotTokenPrefix;
    static const QString changestatusTokenPrefix;
	static const QString getblacklistTokenPrefix;
	static const QString addblacklistTokenPrefix;
	static const QString delblacklistTokenPrefix;
	static const QString getmemberinfoTokenPrefix;
	static const QString getuploadphotoTokenPrefix;
	static const QString versionTokenPrefix;
	
	static const QString phoneloadconfigTokenPrefix;
	static const QString phoneregisterTokenPrefix;
	static const QString phoneloginTokenPrefix;
	static const QString phoneforgetpasswordTokenPrefix;
	static const QString phonecontactlistTokenPrefix;
	static const QString phoneaddcontactTokenPrefix;
	static const QString phoneupdatecontactTokenPrefix;
	static const QString phonedelcontactTokenPrefix;
	static const QString phonedetailcontactTokenPrefix;
	static const QString phonegethistorycdrTokenPrefix;
	static const QString phonegetuserinfoTokenPrefix;
	static const QString phoneedituserinfoTokenPrefix;
	static const QString phonechangepasswordTokenPrefix;
	static const QString phonenewpasswordTokenPrefix;
	static const QString phonegetcdridTokenPrefix;
	static const QString phonedelcdrTokenPrefix;

    CommandServiceOptions m_opt;
    QPointer<UserService> m_userService;
    DownloadQueue* m_commandQueue;

	QString m_sign;
	QString m_timestamp;
	QString m_client;
	QString m_version;
	QString m_language;
	QString m_deviceId;
	QString m_deviceModel;

	QList<QNetworkCookie> m_cookies;
	QString m_token;

    CommandServicePrivate(ServiceManager* manager, const QString& name);
    virtual ~CommandServicePrivate();

    virtual void startup();
    virtual void cleanup();

	QList<QNetworkCookie> cookies() const;
	void setCookies(const QList<QNetworkCookie>& cookies);

	QString token() const;
	void setToken(const QString& token);

	void loadconfig(const Command::LoadConfigParam& param);
	void getserver(const Command::GetServerParam& param);
    void login(const Command::LoginParam& param);
	void login2(const Command::LoginParam& param);
	void setclientinfo(const Command::SetClientInfoParam& param);
	void todaystat(const Command::TodayStatParam& param);
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

    void _q_commandFinished(const QString& token, const DownloadResult& result);

    static QString makeToken(const QString& prefix, const QString& arg1);
    static QString makeToken(const QString& prefix, const QString& arg1, const QString& arg2);
    static QString makeToken(const QString& prefix, const QString& arg1, const QString& arg2, const QString& arg3);
    static QString makeToken(const QString& prefix, const QString& arg1, const QString& arg2, const QString& arg3, const QString& arg4);
    static QString makeToken(const QString& prefix, const QStringList& args);
    static QStringList extractToken(const QString& token, const QString& prefix);
	static QStringList extractUploadPhotoToken(const QString& token, const QString& prefix);
	static void addCommonRequestHeaders(QNetworkRequest& request);

private:
    void init();

	void addQueryItem(QUrl* url, const QString &key, const QString &value);
	void addSessionIdQueryItem(QUrl* url) const;
    QString makeFullUrl(const QString& url) const;
	void addCommonQueryItem(QUrl* url);
	//void addCommonRequestHeaders(QNetworkRequest& request);

    QByteArray encryptUserName(const QByteArray& text, const QByteArray& key);
    QByteArray encryptPassword(const QByteArray& text, const QByteArray& key);
    QByteArray encryptKey(const QByteArray& text, const QByteArray& key);
};

} // namespace ns

#endif // COMMANDSERVICE_P_H
