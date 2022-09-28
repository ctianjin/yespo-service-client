#include <QtCore/QSharedData>
#include "sipcommand.h"

namespace Qpid {
namespace SIP {

// CallInParam
class CallInParamPrivate : public QSharedData
{
public:
	CallInParamPrivate();
	CallInParamPrivate(const CallInParamPrivate& rhs);

	int m_account;
	int m_callID;
	QString m_sourceLang;
	QString m_targetLang;
	QString m_callMode;                         //two-way or three-way
	QString m_serviceMode;                   //Voice Easy service mode (8 types)
	QString m_remoteContact;               //Eg: "Caller name" <CallerID@domain>
};

CallInParamPrivate::CallInParamPrivate() :
	QSharedData(),
	m_account(-1),
	m_callID(-1),
	m_sourceLang(),
	m_targetLang(),
	m_callMode(),
	m_serviceMode(),
	m_remoteContact()
{
}

CallInParamPrivate::CallInParamPrivate(const CallInParamPrivate& rhs) :
	QSharedData(rhs),
	m_account(rhs.m_account),
	m_callID(rhs.m_callID),
	m_sourceLang(rhs.m_sourceLang),
	m_targetLang(rhs.m_targetLang),
	m_callMode(rhs.m_callMode),
	m_serviceMode(rhs.m_serviceMode),
	m_remoteContact(rhs.m_remoteContact)
{
}

CallInParam::CallInParam() :
	d(new CallInParamPrivate)
{
}

CallInParam::CallInParam(const CallInParam& rhs) :
	d(rhs.d)
{
}

CallInParam::~CallInParam()
{
}

CallInParam& CallInParam::operator=(const CallInParam& rhs)
{
	d = rhs.d;
	return *this;
}

int CallInParam::account() const
{
	return d->m_account;
}

void CallInParam::setAccount(int acc)
{
	d->m_account = acc;
}

int CallInParam::callID() const
{
	return d->m_callID;
}

void CallInParam::setCallID(int ID)
{
	d->m_callID = ID;
}

QString CallInParam::sourceLanguage() const
{
	return d->m_sourceLang;
}

void CallInParam::setSourceLanguage(const QString& language)
{
	d->m_sourceLang = language;
}

QString CallInParam::targetLanguage() const
{
	return d->m_targetLang;
}

void CallInParam::setTargetLanguage(const QString& language)
{
	d->m_targetLang = language;
}

QString CallInParam::callMode() const
{
	return d->m_callMode;
}

void CallInParam::setCallMode(const QString& mode)
{
	d->m_callMode = mode;
}

QString CallInParam::serviceMode() const
{
	return d->m_serviceMode;
}

void CallInParam::setServiceMode(const QString& mode)
{
	d->m_serviceMode = mode;
}

QString CallInParam::remoteContact() const
{
	return d->m_remoteContact;
}

void CallInParam::setRemoteContact(const QString& contact)
{
	d->m_remoteContact = contact;
}

bool CallInParam::operator==(const CallInParam& rhs) const
{
	return d->m_account == rhs.d->m_account
		&& d->m_callID == rhs.d->m_callID
		&& d->m_sourceLang == rhs.d->m_sourceLang
		&& d->m_targetLang == rhs.d->m_targetLang
		&& d->m_callMode == rhs.d->m_callMode
		&& d->m_serviceMode == rhs.d->m_serviceMode
		&& d->m_remoteContact == rhs.d->m_remoteContact;
}

bool CallInParam::operator!=(const CallInParam& rhs) const
{
	return !operator==(rhs);
}

} // namespace SIP
} // namespace Qpid
