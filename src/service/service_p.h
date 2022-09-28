#ifndef QPID_SERVICE_P_H
#define QPID_SERVICE_P_H

#include <QtCore/QString>
#include <QtNetwork/QNetworkReply>
#include "service.h"

namespace ns {
namespace Service {

QString stateText(State state);
QString errorText(Error error);
QString errorText(Error error, const QString& reason);

Error translateNetworkError(QNetworkReply::NetworkError e);
Error translateSpeechInputError(int status);

} // namespace ns::Service
} // namespace ns

#endif // QPID_SERVICE_P_H
