#ifndef QPID_PHONEGLOBAL_H
#define QPID_PHONEGLOBAL_H

#include <QString>

namespace Qpid {

struct AppOptions {
	static const QString countryNumberSep_;
	static const QString phonesSep_;
};

struct ServiceModeOptions
{
	static const QString PSTNPaid_;
	static const QString PSTNFree_;
	static const QString PSTNDirect_;

	static const QString VEPaid_;
	static const QString VEFree_;
	static const QString VEDirect_;

	static const QString centerFree_;
};

struct CallModeOptions
{
	static const QString twoWay_;
	static const QString threeWay_;
};

struct UpdateContactOptions {
	static const QString updatePhone_;
	static const QString updateVoiceID_;
	static const QString updateLanguage_;
	static const QString deletePhone_;
	static const QString updateContactName_;
};

bool isDirectCall(const QString& serviceMode);
bool isInterpreterCall(const QString& serviceMode);
bool isCenterCall(const QString& serviceMode);
bool isFreeCall(const QString& serviceMode);

QString phoneErrorMsg(const QString& code);

} // namespace Qpid

#endif // QPID_PHONEGLOBAL_H
