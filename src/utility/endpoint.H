#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include "utilityglobal.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace ns {

class EndpointPrivate;

class BUILDSYS_UTILITY_DECL Endpoint
{
public:
    Endpoint();
    Endpoint(const Endpoint& rhs);
    Endpoint(const QString& host, int port);
    ~Endpoint();

    Endpoint& operator=(const Endpoint& rhs);

    bool isValid() const;

    QString host() const;
    void setHost(const QString& host);

    int port() const;
    void setPort(int port);

    void clear();

    QString toString() const;
    static Endpoint fromString(const QString& string);

private:
    QSharedDataPointer<EndpointPrivate> d;
};

BUILDSYS_UTILITY_DECL QDebug operator<<(QDebug, const Endpoint& rhs);

} // namespace ns

Q_DECLARE_METATYPE(ns::Endpoint)

#endif // ENDPOINT_H
