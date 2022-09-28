#ifndef QPID_SERVICE_H
#define QPID_SERVICE_H

#include <QtCore/QMetaType>

namespace ns {
namespace Service {

enum State
{
    StoppedState,
    InitState,
    LoginState,
    LogoutState,
    ActiveState,
};

enum PhoneStatus
{
	IdleStatus = 1,
	BusyStatus
};

enum Error
{
    NoError = 0,

    NetworkError,
    NetworkAccessError,
    HostNotFoundError,
    HostUnreachableError,
    ConnectionRefusedError,
    ConnectionResetError,
    RemoteHostClosedError,
    ProtocolError,
    PacketError,
    TimeoutError,
    RequestSerializationFailedError,
    ResponseSerializationFailedError,
    RequestAbortedError,
    RequestTimeoutError,
    InvalidSessionError,
    CommandNotSupportedError,
    InvalidArgumentError,
    NoPrivilegeError,
    NoResourceError,
    ProcessTimeoutError,
    TryAgainError,
    ResourceBusyError,
    ResourceExpiredError,
    ServerInternalError,
    LoginError,
    ServerNotAvailableError,
    ChannelTimeoutError,
    ChannelInvalidError,
    UserInvalidError,
    DataError,
    FileSystemError,
    AudioError,
    NoSpeechError,
    SpeechNoMatchError,
    BadSpeechGrammarError,
    InternalError,
	SessionInvalid,
	BalanceNotEnough,
	BindPortError,
	SendBroadcastError,
	MakeCallError,
    UnknownError = -1
};

} // namespace ns::Service
} // namespace ns

Q_DECLARE_METATYPE(ns::Service::State)
Q_DECLARE_METATYPE(ns::Service::PhoneStatus)
Q_DECLARE_METATYPE(ns::Service::Error)

#endif // QPID_SERVICE_H
