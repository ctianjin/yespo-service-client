#ifndef VERSION_H
#define VERSION_H

#include <QtCore/QMetaType>
#include <QtCore/QString>
#include "utilityglobal.h"

namespace ns {

class BUILDSYS_UTILITY_DECL Version
{
public:
    Version();
    Version(const Version& rhs);
    Version(uint major, uint minor, uint patch);
    explicit Version(const QString& string, QChar sep = QLatin1Char('.'));
    ~Version();

    Version& operator=(const Version& rhs);

    bool isValid() const;

    uint major() const;
    uint minor() const;
    uint patch() const;

    Version setMajor(uint major) const;
    Version setMinor(uint minor) const;
    Version setPatch(uint patch) const;

    Version addMajor(int value) const;
    Version addMinor(int value) const;
    Version addPatch(int value) const;

    int compare(const Version& rhs) const;

    QString toString(QChar sep = QLatin1Char('.')) const;
    static Version fromString(const QString& string, QChar sep = QLatin1Char('.'));

private:
    uint m_major : 8;
    uint m_minor : 8;
    uint m_patch : 16;
};

inline bool operator==(const Version& lhs, const Version& rhs)
{
    return lhs.compare(rhs) == 0;
}

inline bool operator!=(const Version& lhs, const Version& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(const Version& lhs, const Version& rhs)
{
    return lhs.compare(rhs) < 0;
}

inline bool operator<=(const Version& lhs, const Version& rhs)
{
    return lhs.compare(rhs) <= 0;
}

inline bool operator>(const Version& lhs, const Version& rhs)
{
    return lhs.compare(rhs) > 0;
}

inline bool operator>=(const Version& lhs, const Version& rhs)
{
    return lhs.compare(rhs) >= 0;
}

BUILDSYS_UTILITY_DECL QDebug operator<<(QDebug, const Version& rhs);

} // namespace ns

Q_DECLARE_METATYPE(ns::Version)

#endif // VERSION_H
