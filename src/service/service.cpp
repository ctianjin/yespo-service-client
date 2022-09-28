#include <QtCore/QCoreApplication>
#include "service_p.h"

namespace ns {
namespace Service {

QString stateText(State state)
{
    static const char* texts[] = {
        QT_TRANSLATE_NOOP("ns::Service", "Stopped"),
        QT_TRANSLATE_NOOP("ns::Service", "Initializing"),
        QT_TRANSLATE_NOOP("ns::Service", "Logging In"),
        QT_TRANSLATE_NOOP("ns::Service", "Logging Out"),
        QT_TRANSLATE_NOOP("ns::Service", "Active")
    };

    QString ret;
    if (state >= 0 && state < static_cast<int>(sizeof(texts) / sizeof(texts[0]))) {
        ret = QCoreApplication::translate("ns::Service", texts[state]);
    }
    return ret;
}

QString errorText(Error error)
{
    static const char* texts[] = {
        QT_TRANSLATE_NOOP("ns::Service", "No error"),
        QT_TRANSLATE_NOOP("ns::Service", "Network error"),
        QT_TRANSLATE_NOOP("ns::Service", "Network access denied"),
        QT_TRANSLATE_NOOP("ns::Service", "Host not found"),
        QT_TRANSLATE_NOOP("ns::Service", "Host unreachable"),
        QT_TRANSLATE_NOOP("ns::Service", "Connection refused"),
        QT_TRANSLATE_NOOP("ns::Service", "Connection reset"),
        QT_TRANSLATE_NOOP("ns::Service", "Remote host closed the connection"),
        QT_TRANSLATE_NOOP("ns::Service", "Protocol error"),
        QT_TRANSLATE_NOOP("ns::Service", "Packet error"),
        QT_TRANSLATE_NOOP("ns::Service", "Timeout"),
        QT_TRANSLATE_NOOP("ns::Service", "Request serialization failed"),
        QT_TRANSLATE_NOOP("ns::Service", "Response serialization failed"),
        QT_TRANSLATE_NOOP("ns::Service", "Request aborted"),
        QT_TRANSLATE_NOOP("ns::Service", "Request timeout"),
        QT_TRANSLATE_NOOP("ns::Service", "Invalid session"),
        QT_TRANSLATE_NOOP("ns::Service", "Command not supported"),
        QT_TRANSLATE_NOOP("ns::Service", "Invalid argument"),
        QT_TRANSLATE_NOOP("ns::Service", "No privilege"),
        QT_TRANSLATE_NOOP("ns::Service", "No resource"),
        QT_TRANSLATE_NOOP("ns::Service", "Process timeout"),
        QT_TRANSLATE_NOOP("ns::Service", "Please try again"),
        QT_TRANSLATE_NOOP("ns::Service", "Resource busy"),
        QT_TRANSLATE_NOOP("ns::Service", "Resource expired"),
        QT_TRANSLATE_NOOP("ns::Service", "Server internal error"),
        QT_TRANSLATE_NOOP("ns::Service", "Login error"),
        QT_TRANSLATE_NOOP("ns::Service", "Server not available"),
        QT_TRANSLATE_NOOP("ns::Service", "Channel timeout"),
        QT_TRANSLATE_NOOP("ns::Service", "Invalid channel"),
        QT_TRANSLATE_NOOP("ns::Service", "Invalid user"),
        QT_TRANSLATE_NOOP("ns::Service", "Data error"),
        QT_TRANSLATE_NOOP("ns::Service", "File system error"),
        QT_TRANSLATE_NOOP("ns::Service", "There was an error with recording audio"),
        QT_TRANSLATE_NOOP("ns::Service", "No speech heard"),
        QT_TRANSLATE_NOOP("ns::Service", "Speech was heard, but could not be interpreted in the specified language and language model"),
        QT_TRANSLATE_NOOP("ns::Service", "There was an error in the speech recognition grammar"),
        QT_TRANSLATE_NOOP("ns::Service", "Internal error"),
		QT_TRANSLATE_NOOP("ns::Service", ""),
		QT_TRANSLATE_NOOP("ns::Service", ""),
		QT_TRANSLATE_NOOP("ns::Service", "Bind port failed"),
		QT_TRANSLATE_NOOP("ns::Service", "Send broadcast message failed"),
		QT_TRANSLATE_NOOP("ns::Service", "Make call failed")
    };

    QString ret;
    if (error >= 0 && error < static_cast<int>(sizeof(texts) / sizeof(texts[0]))) {
        ret = QCoreApplication::translate("ns::Service", texts[error]);
    } else {
        ret = QCoreApplication::translate("ns::Service", "Unknown error");
    }
    return ret;
}

QString errorText(Error error, const QString& reason)
{
    QString ret = errorText(error);
    if (!reason.isEmpty()) {

		if (ret.isEmpty()){
			ret = QCoreApplication::translate("ns::Service", "%1").arg(reason);
		} else {
			ret = QCoreApplication::translate("ns::Service", "%1: %2").arg(ret, reason);
		}
    }
    return ret;
}

Error translateNetworkError(QNetworkReply::NetworkError e)
{
    if (e == QNetworkReply::NoError)
        return NoError;

    Error error = UnknownError;
    switch (e) {
        case QNetworkReply::ConnectionRefusedError:
            error = ConnectionRefusedError;
            break;
        case QNetworkReply::RemoteHostClosedError:
            error = RemoteHostClosedError;
            break;
        case QNetworkReply::HostNotFoundError:
            error = HostNotFoundError;
            break;
        case QNetworkReply::TimeoutError:
            error = RequestTimeoutError;
            break;
        case QNetworkReply::OperationCanceledError:
            error = RequestAbortedError;
            break;
        case QNetworkReply::SslHandshakeFailedError:
            error = NetworkAccessError;
            break;
        case QNetworkReply::TemporaryNetworkFailureError:
            error = NetworkError;
            break;
        case QNetworkReply::UnknownNetworkError:
            error = NetworkError;
            break;

        case QNetworkReply::ProxyConnectionRefusedError:
            error = NetworkAccessError;
            break;
        case QNetworkReply::ProxyConnectionClosedError:
            error = NetworkAccessError;
            break;
        case QNetworkReply::ProxyNotFoundError:
            error = NetworkAccessError;
            break;
        case QNetworkReply::ProxyTimeoutError:
            error = TimeoutError;
            break;
        case QNetworkReply::ProxyAuthenticationRequiredError:
            error = NetworkAccessError;
            break;
        case QNetworkReply::UnknownProxyError:
            error = NetworkAccessError;
            break;

        case QNetworkReply::ContentAccessDenied:
            error = NetworkAccessError;
            break;
        case QNetworkReply::ContentOperationNotPermittedError:
            error = NetworkAccessError;
            break;
        case QNetworkReply::ContentNotFoundError:
            error = NetworkAccessError;
            break;
        case QNetworkReply::AuthenticationRequiredError:
            error = NetworkAccessError;
            break;
        case QNetworkReply::ContentReSendError:
            error = NetworkAccessError;
            break;
        case QNetworkReply::UnknownContentError:
            error = NetworkAccessError;
            break;

        case QNetworkReply::ProtocolUnknownError:
            error = ProtocolError;
            break;
        case QNetworkReply::ProtocolInvalidOperationError:
            error = ProtocolError;
            break;
        case QNetworkReply::ProtocolFailure:
            error = ProtocolError;
            break;

        default:
            break;
    }

    return error;
}

Error translateSpeechInputError(int status)
{
    // http://www.w3.org/2005/Incubator/htmlspeech/2010/10/google-api-draft.html#speech-input-error
    if (status == 0)
        return NoError;

    Error error = UnknownError;
    switch (status) {
        case 1: // SpeechInputError::ABORTED
            error = RequestAbortedError;
            break;
        case 2: // SpeechInputError::AUDIO
            error = AudioError;
            break;
        case 3: // SpeechInputError::NETWORK
            error = NetworkError;
            break;
        case 4: // SpeechInputError::NO_SPEECH
            error = NoSpeechError;
            break;
        case 5: // SpeechInputError::NO_MATCH
            error = SpeechNoMatchError;
            break;
        case 6: // SpeechInputError::BAD_GRAMMAR
            error = BadSpeechGrammarError;
            break;
        default:
            break;
    }

    return error;
}

} // namespace ns::Service
} // namespace ns
