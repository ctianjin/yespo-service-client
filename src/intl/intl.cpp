#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QLibraryInfo>
#include <QtCore/QLocale>
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtCore/QTranslator>
#include "intl.h"
#include "product.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "Intl:"
#endif // DEBUG_PREFIX

namespace ns {

class IntlPrivate
{
public:
    QString m_translationsPath;
    QTranslator* m_translator;
    QTranslator* m_qtTranslator;

    IntlPrivate();

    bool installTranslators(QTranslator* translator, QTranslator* qtTranslator);
    void removeTranslators();
};

IntlPrivate::IntlPrivate() :
    m_translationsPath(QLatin1String(":/translations")),
    m_translator(0),
    m_qtTranslator(0)
{
}

bool IntlPrivate::installTranslators(QTranslator* translator, QTranslator* qtTranslator)
{
    removeTranslators();

    // Qt translator first, then product translator
    m_translator = translator;
    m_qtTranslator = qtTranslator;

    if (m_qtTranslator) {
        qApp->installTranslator(m_qtTranslator);
    }
    if (m_translator) {
        qApp->installTranslator(m_translator);
    }

    return m_translator != 0;
}

void IntlPrivate::removeTranslators()
{
    if (m_translator) {
        qApp->removeTranslator(m_translator);
        m_translator->deleteLater();
        m_translator = 0;
    }

    if (m_qtTranslator) {
        qApp->removeTranslator(m_qtTranslator);
        m_qtTranslator->deleteLater();
        m_qtTranslator = 0;
    }
}

Intl::Intl(QObject* parent) :
    QObject(parent),
    d_ptr(new IntlPrivate)
{
}

Intl::~Intl()
{
    delete d_ptr;
}

QString Intl::translationsPath() const
{
    Q_D(const Intl);
    return d->m_translationsPath;
}

void Intl::setTranslationsPath(const QString& translationsPath)
{
    Q_D(Intl);
    d->m_translationsPath = translationsPath;
}

bool Intl::installSystemLanguage()
{
    QLocale systemLocale = QLocale::system();

    if (QLocale() != systemLocale) {
        QLocale::setDefault(systemLocale);
    }

    return installTranslators();
}

bool Intl::installLanguage(const QLocale& locale)
{
    QLocale::setDefault(locale);
    return installTranslators();
}

bool Intl::installLanguage(const QString& language)
{
    if (!language.isEmpty()) {
        QLocale locale(language);
        if (locale != QLocale::c()) {
            QLocale::setDefault(locale);
        }
    }

    return installTranslators();
}

bool Intl::installTranslators()
{
    Q_D(Intl);

    const QString language = QLocale().name();

	qDebug() << DEBUG_PREFIX << language;

    const QString productTrFile = QLatin1String(BUILDSYS_PRODUCT_DOMAIN"_") + language;
    const QString qtTrFile = QLatin1String("qt_") + language;

    QScopedPointer<QTranslator> translator(new QTranslator(this));
    QScopedPointer<QTranslator> qtTranslator(new QTranslator(this));

    if (translator->load(productTrFile, d->m_translationsPath)) {
        qDebug() << DEBUG_PREFIX << "Loaded product translations" << language
            << "from" << d->m_translationsPath;
    } else {
        qWarning() << DEBUG_PREFIX << "Failed to load product translations" << language;
        translator.reset();
    }

    if (qtTranslator->load(qtTrFile, d->m_translationsPath)) {
        qDebug() << DEBUG_PREFIX << "Loaded Qt translations" << language
            << "from" << d->m_translationsPath;
    } else {
        const QString qtTrPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
        if (qtTranslator->load(qtTrFile, qtTrPath)) {
            qDebug() << DEBUG_PREFIX << "Loaded Qt translations" << language
                << "from" << qtTrPath;
        } else {
            qWarning() << DEBUG_PREFIX << "Failed to load Qt translations" << language;
            qtTranslator.reset();
        }
    }

    return d->installTranslators(translator.take(), qtTranslator.take());
}

void Intl::removeTranslators()
{
    Q_D(Intl);
    d->removeTranslators();
}

} // namespace ns
