#ifndef QPID_SIPCOMMAND_H
#define QPID_SIPCOMMAND_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include "serviceglobal.h"

namespace Qpid {
namespace SIP {

enum CallState
{
	CallNone,
	Calling,
	CallIncoming,
	CallEarly,
	CallConnecting,
	CallConfirmed,
	CallDisconnected
};

class CallInParamPrivate;
class QPID_SERVICE_DECL CallInParam
{
public:
	CallInParam();
	CallInParam(const CallInParam& rhs);
	~CallInParam();

	CallInParam& operator=(const CallInParam& rhs);

	int account() const;
	void setAccount(int acc);

	int callID() const;
	void setCallID(int ID);

	QString sourceLanguage() const;
	void setSourceLanguage(const QString& language);

	QString targetLanguage() const;
	void setTargetLanguage(const QString& language);

	QString callMode() const;
	void setCallMode(const QString& mode);

	QString serviceMode() const;
	void setServiceMode(const QString& mode);

	QString remoteContact() const;
	void setRemoteContact(const QString& contact);

	bool operator==(const CallInParam& rhs) const;
	bool operator!=(const CallInParam& rhs) const;

private:
	QSharedDataPointer<CallInParamPrivate> d;
};

} // namespace Qpid::SIP
} // namespace Qpid

Q_DECLARE_METATYPE(Qpid::SIP::CallState)
Q_DECLARE_METATYPE(Qpid::SIP::CallInParam)


#endif // QPID_SIPCOMMAND_H
