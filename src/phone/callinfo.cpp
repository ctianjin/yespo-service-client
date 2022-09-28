#include "callinfo.h"

namespace Qpid {

Call::Call() : 
	m_callID(-1), 
	m_callState(CallClosed)
{
}

Call::~Call()
{
}

int Call::callID() const
{
	return m_callID;
}

void Call::setCallID(int ID)
{
	m_callID = ID;
}

Call::CallDirection Call::direction() const
{
	return m_callDirection;
}

void Call::setDirection(CallDirection direction)
{
	m_callDirection = direction;
}

Call::CallState Call::state() const
{
	return m_callState;
}

void Call::setCallState(CallState state)
{
	m_callState = state;
}

QString Call::serviceMode() const
{
	return m_serviceMode;
}

void Call::setServiceMode(const QString& mode)
{
	m_serviceMode = mode;
}

QString Call::callerNumber() const
{
	return m_callerNumber;
}

void Call::setCallerNumber(const QString& number)
{
	m_callerNumber = number;
}

QString Call::callerName() const
{
	return m_callerName;
}

void Call::setCallerName(const QString& name)
{
	m_callerName = name;
}

QString Call::calleeNumber() const
{
	return m_calleeNumber;
}

void Call::setCalleeNumber(const QString& number)
{
	m_calleeNumber = number;
}

QString Call::calleeName() const
{
	return m_calleeName;
}

void Call::setCalleeName(const QString& name)
{
	m_calleeName = name;
}

} // namespace Qpid
