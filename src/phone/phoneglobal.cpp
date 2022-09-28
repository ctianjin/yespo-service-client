#include <QObject>
#include "phoneglobal.h"

namespace Qpid {

const QString AppOptions::countryNumberSep_ = QLatin1String("|");
const QString AppOptions::phonesSep_= QLatin1String(",");

const QString ServiceModeOptions::PSTNPaid_   = QLatin1String("00");
const QString ServiceModeOptions::PSTNFree_   = QLatin1String("01");
const QString ServiceModeOptions::PSTNDirect_ = QLatin1String("02");
const QString ServiceModeOptions::VEPaid_       = QLatin1String("03");
const QString ServiceModeOptions::VEFree_       = QLatin1String("04");
const QString ServiceModeOptions::VEDirect_     = QLatin1String("05");
const QString ServiceModeOptions::centerFree_ = QLatin1String("06");

const QString CallModeOptions::twoWay_        = QLatin1String("two_way");
const QString CallModeOptions::threeWay_        = QLatin1String("three_way");

const QString UpdateContactOptions::updatePhone_ = QLatin1String("updatephone");
const QString UpdateContactOptions::updateVoiceID_ = QLatin1String("updatevoiceraid");
const QString UpdateContactOptions::updateLanguage_ = QLatin1String("updatelang");
const QString UpdateContactOptions::deletePhone_ = QLatin1String("delphone");
const QString UpdateContactOptions::updateContactName_ = QLatin1String("updatecontactname");

bool isDirectCall(const QString& serviceMode)
{
	return (serviceMode==ServiceModeOptions::PSTNDirect_ 
		|| serviceMode==ServiceModeOptions::VEDirect_);
}

bool isInterpreterCall(const QString& serviceMode)
{
	return (serviceMode==ServiceModeOptions::PSTNPaid_ || serviceMode==ServiceModeOptions::PSTNFree_
		|| serviceMode==ServiceModeOptions::VEPaid_ || serviceMode==ServiceModeOptions::VEFree_)
		|| serviceMode==ServiceModeOptions::centerFree_;
}

bool isCenterCall(const QString& serviceMode)
{
	return (serviceMode==ServiceModeOptions::centerFree_);
}

bool isFreeCall(const QString& serviceMode)
{
	return (serviceMode!=ServiceModeOptions::PSTNPaid_ && serviceMode!=ServiceModeOptions::VEPaid_);
}

QString phoneErrorMsg(const QString& code)
{
	int errorCode = code.toInt();
	switch (errorCode)
	{
	case 1001: return QObject::tr("Sorry, Your user ID should be in 6 - 32 characters.");
	case 1003: return QObject::tr("Sorry, Please enter your user ID.");
	case 1004: return QObject::tr("Sorry, User ID can only contains English alphabet,number and underscore \"-\"  in 6-32 characters.");
	case 1005: return QObject::tr("Please select your native language.");
	case 1006: return QObject::tr("Please enter your Email address.");
	case 1007: return QObject::tr("Sorry, Your Email address is invalid.");
	case 1008: return QObject::tr("Please enter your new password.");
	case 1009: return QObject::tr("Sorry, Your password should be in 6 - 32 characters.");
	case 1011: return QObject::tr("Sorry, The Voice Easy ID your entered already exists.");
	case 1012: return QObject::tr("Sorry, The Email address your entered already exists.");
	case 1013: return QObject::tr("Please enter your login Email or user ID");
	case 1014: return QObject::tr("Please enter your password.");
	case 1015: return QObject::tr("Sorry this user ID is not exists.");
	case 1016: return QObject::tr("Your user ID or password incorrect.");
	case 1018: return QObject::tr("Sorry,Password does not match");
	case 1019: return QObject::tr("Sorry,The verification code you entered is incorrect");
	case 1020: return QObject::tr("Password Update is incorrect.");
	case 1021: return QObject::tr("Sorry, Your verification code has been expired.");
	case 1025: return QObject::tr("Please enter a valid phone number");
	case 1026: return QObject::tr("Phone number length cannot exceed 32 character.");
	case 1027: return QObject::tr("Sorry, Please enter your nickname.");
	case 1028: return QObject::tr("Sorry,The nickname your entered should be in 6 - 32 characters.");
	case 1029: return QObject::tr("Sorry, Your nickname can only contains English alphabet, numbers, space and underscore \"_\" in 6 - 32 characters.");
	case 1030: return QObject::tr("Please select language.");
	case 1031: return QObject::tr("Nationality can not be empty ");
	case 1032: return QObject::tr("Profession can not be empty ");
	case 1033: return QObject::tr("Please select translate the source language");
	case 1034: return QObject::tr("Please select the translation target language");
	case 1035: return QObject::tr("Please enter contact name");
	case 1036: return QObject::tr("can only contains English alphabet, numbers, space and underscore \"_\" in 6 - 32 characters.");
	case 1037: return QObject::tr("Please enter a phone number");
	case 1040: return QObject::tr("Please select country code.");
	case 1044: return QObject::tr("Please confirm your new password.");
	case 1045: return QObject::tr("Sorry this VoiceEasy ID is not exists Please re-enter .");
	case 1046: return QObject::tr("This language is not exists in our cope of services.");
	case 1047: return QObject::tr("Add contacts to fail.");
	case 1048: return QObject::tr("Contact does not exist.");
	case 1049: return QObject::tr("Contact information update failed.");
	case 1050: return QObject::tr("Please select the contacts you want to delete.");
	case 1051: return QObject::tr("Contact delete failed.");
	case 1052: return QObject::tr("Please select the contacts that you need to obtain.");
	case 1053: return QObject::tr("This Nickname already exists,Please re-enter.");
	case 1054: return QObject::tr("Please select an image to upload.");
	case 1055: return QObject::tr("Image size can not be zero.");
	case 1056: return QObject::tr("Only upload pictures smaller than 2M.");
	case 1057: return QObject::tr("You can only upload JPG format images.");
	case 1058: return QObject::tr("File upload failed, please try again letter.");
	case 1059: return QObject::tr("User ID information update failed");
	case 1060: return QObject::tr("Email does not exist , please enter your registered Email.");
	case 1061: return QObject::tr("Sorry,This VoiceEasy ID is already in your contact list.");
	case 1062: return QObject::tr("Sorry, you can not add yourself as VoiceEasy friend");
	case 1063: return QObject::tr("");
	default: return QString();
	}
}

} // namespace Qpid
