#include <QtCore/QDebug>
#include "version.h"

namespace ns {

Version::Version() :
    m_major(0),
    m_minor(0),
    m_patch(0)
{
}

Version::Version(const Version& rhs) :
    m_major(rhs.m_major),
    m_minor(rhs.m_minor),
    m_patch(rhs.m_patch)
{
}

Version::Version(uint major, uint minor, uint patch) :
    m_major(major),
    m_minor(minor),
    m_patch(patch)
{
}

Version::Version(const QString& string, QChar sep)
{
    // TODO Use QString::split
    m_major = string.section(sep, 0, 0).toUInt();
    m_minor = string.section(sep, 1, 1).toUInt();
    m_patch = string.section(sep, 2, 2).toUInt();
}

Version::~Version()
{
}

Version& Version::operator=(const Version& rhs)
{
    m_major = rhs.m_major;
    m_minor = rhs.m_minor;
    m_patch = rhs.m_patch;
    return *this;
}

bool Version::isValid() const
{
    return m_major != 0 || m_minor != 0 || m_patch != 0;
}

uint Version::major() const
{
    return m_major;
}

uint Version::minor() const
{
    return m_minor;
}

uint Version::patch() const
{
    return m_patch;
}

Version Version::setMajor(uint major) const
{
    return Version(major, m_minor, m_patch);
}

Version Version::setMinor(uint minor) const
{
    return Version(m_major, minor, m_patch);
}

Version Version::setPatch(uint patch) const
{
    return Version(m_major, m_minor, patch);
}

Version Version::addMajor(int value) const
{
    int major = static_cast<int>(m_major) + value;
    if (major < 0 || major > 0xFF)
        return Version();

    return setMajor(static_cast<uint>(major));
}

Version Version::addMinor(int value) const
{
    int minor = static_cast<int>(m_minor) + value;
    if (minor < 0 || minor > 0xFF)
        return Version();

    return setMinor(static_cast<uint>(minor));
}

Version Version::addPatch(int value) const
{
    int patch = static_cast<int>(m_patch) + value;
    if (patch < 0 || patch > 0xFFFF)
        return Version();

    return setPatch(static_cast<uint>(patch));
}

int Version::compare(const Version& rhs) const
{
    int ret = 0;

    ret = m_major - rhs.m_major;
    if (ret != 0)
        return ret;

    ret = m_minor - rhs.m_minor;
    if (ret != 0)
        return ret;

    ret = m_patch - rhs.m_patch;

    return ret;
}

QString Version::toString(QChar sep) const
{
    QString ret;
    ret += QString::number(m_major);
    ret += sep;
    ret += QString::number(m_minor);
    ret += sep;
    ret += QString::number(m_patch);
    return ret;
}

Version Version::fromString(const QString& string, QChar sep)
{
    return Version(string, sep);
}

QDebug operator<<(QDebug dbg, const Version& rhs)
{
    dbg.nospace() << '[' << rhs.major() << '.' << rhs.minor() << '.' << rhs.patch() << ']';
    return dbg.space();
}

} // namespace ns
