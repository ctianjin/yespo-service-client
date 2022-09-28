#ifndef INTL_H
#define INTL_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include "intlglobal.h"

QT_BEGIN_NAMESPACE
class QLocale;
QT_END_NAMESPACE

namespace ns {

class IntlPrivate;

class BUILDSYS_INTL_DECL Intl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString translationsPath READ translationsPath WRITE setTranslationsPath)

public:
    explicit Intl(QObject* parent = 0);
    virtual ~Intl();

    QString translationsPath() const;
    void setTranslationsPath(const QString& translationsPath);

    bool installSystemLanguage();
    bool installLanguage(const QLocale& locale);
    bool installLanguage(const QString& language);

    bool installTranslators();
    void removeTranslators();

private:
    IntlPrivate* const d_ptr;

    Q_DECLARE_PRIVATE(Intl)
    Q_DISABLE_COPY(Intl)
};

} // namespace ns

#endif // INTL_H
