#ifndef QPID_CALLINFO_H
#define QPID_CALLINFO_H

#include <QString>

namespace Qpid {

class Call
{
public:
	Call();
	~Call();

	enum CallDirection {
		DirectionIn = 0,
		DirectionOut = 1
	};

	enum CallState {
		CallingIn = 0,
		CallingPreOut = 1,
		CallingOut = 2,
		CallConfirmed = 3,
		CallingInterpreter = 4,
		CallingCallee = 5,
		CallConnected = 6,
		CallClosed = 7
	};

	int callID() const;
	void setCallID(int ID);

	CallDirection direction() const;
	void setDirection(CallDirection direction);

	CallState state() const;
	void setCallState(CallState state);

	QString serviceMode() const;
	void setServiceMode(const QString& mode);

	QString callerNumber() const;
	void setCallerNumber(const QString& number);

	QString callerName() const;
	void setCallerName(const QString& name);

	QString calleeNumber() const;
	void setCalleeNumber(const QString& number);

	QString calleeName() const;
	void setCalleeName(const QString& name);

private:
	int m_callID;
	CallDirection m_callDirection;
	CallState m_callState;
	QString m_serviceMode;
	QString m_callerNumber; //Extension number
	QString m_callerName; //Caller name from incoming call
	QString m_calleeNumber; //Extension number of PSTN number
	QString m_calleeName; //Callee name when re-dial from history
};

} // namespace Qpid

#endif // QPID_CALLINFO_H
