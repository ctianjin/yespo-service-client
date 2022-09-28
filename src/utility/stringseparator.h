#ifndef STRINGSEPARATOR_H
#define STRINGSEPARATOR_H

#include <QtCore/QString>
#include "utilityglobal.h"

namespace ns {

class BUILDSYS_UTILITY_DECL StringSeparator
{
public:
    explicit StringSeparator(const char* sep);

    operator QLatin1String() const;

private:
    const char* const m_sep;
    mutable const char* m_s;
};

inline StringSeparator::StringSeparator(const char* sep) :
    m_sep(sep),
    m_s(0)
{
}

inline StringSeparator::operator QLatin1String() const
{
    QLatin1String ret(m_s);
    if (!m_s)
        m_s = m_sep;
    return ret;
}

} // namespace ns

#define BUILDSYS_DECLARE_STRING_SEPARATOR(CLASS, SEP) \
    class CLASS : public ::ns::StringSeparator \
    { \
    public: \
        CLASS() : \
            StringSeparator(SEP) \
        { \
        } \
    };

#endif // STRINGSEPARATOR_H
