#include <QtCore/QDebug>
#include <QtCore/QString>
#include "endpoint.h"

namespace ns {

class EndpointPrivate : public QSharedData
{
public:
    QString m_host;
    int m_port;

    EndpointPrivate();
    EndpointPrivate(const EndpointPrivate& rhs);
    EndpointPrivate(const QString& host, int port);
    ~EndpointPrivate();
};

EndpointPrivate::EndpointPrivate() :
    QSharedData(),
    m_host(),
    m_port(0)
{
}

EndpointPrivate::EndpointPrivate(const EndpointPrivate& rhs) :
    QSharedData(rhs),
    m_host(rhs.m_host),
    m_port(rhs.m_port)
{
}

EndpointPrivate::EndpointPrivate(const QString& host, int port) :
    QSharedData(),
    m_host(host),
    m_port(port)
{
}

EndpointPrivate::~EndpointPrivate()
{
}

Endpoint::Endpoint() :
    d(new EndpointPrivate)
{
}

Endpoint::Endpoint(const Endpoint& rhs) :
    d(rhs.d)
{
}

Endpoint::Endpoint(const QString& host, int port) :
    d(new EndpointPrivate(host, port))
{
}

Endpoint::~Endpoint()
{
}

Endpoint& Endpoint::operator=(const Endpoint& rhs)
{
    d = rhs.d;
    return *this;
}

bool Endpoint::isValid() const
{
    return !d->m_host.isEmpty() && d->m_port != 0;
}

QString Endpoint::host() const
{
    return d->m_host;
}

void Endpoint::setHost(const QString& host)
{
    d->m_host = host;
}

int Endpoint::port() const
{
    return d->m_port;
}

void Endpoint::setPort(int port)
{
    d->m_port = port;
}

void Endpoint::clear()
{
    *this = Endpoint();
}

QString Endpoint::toString() const
{
    QString ret = d->m_host;
    ret.append(QLatin1Char(':'));
    ret.append(QString::number(d->m_port));
    return ret;
}

Endpoint Endpoint::fromString(const QString& string)
{
    QChar sep = QLatin1Char(':');
    QString host = string.section(sep, 0, 0);
    int port = string.section(sep, 1, 1).toInt();
    return Endpoint(host, port);
}

QDebug operator<<(QDebug dbg, const Endpoint& rhs)
{
    dbg.nospace() << rhs.host() << ':' << rhs.port();
    return dbg.space();
}

} // namespace ns
