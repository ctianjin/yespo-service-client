#include <QByteArray>
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QList>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QStack>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QUrl>
#include <QVariant>
#include <QXmlStreamReader>
#ifndef BUILDSYS_PREF_DISABLE_GUI
#include <QVector> // for getFont
#include <QFont>
#include <QColor>
#include <QWidget>
#endif // BUILDSYS_PREF_DISABLE_GUI
#include "abstractpreference.h"
#include "abstractpreference_p.h"
#include "preferencetransaction.h"
#include "version.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "AbstractPreference:"
#endif // DEBUG_PREFIX

namespace ns {

AbstractPreferencePrivate::AbstractPreferencePrivate(PreferenceManager* manager, const QString& path) :
    q_ptr(0),
    m_manager(manager),
    m_path(path)
{
}

AbstractPreferencePrivate::~AbstractPreferencePrivate()
{
}

AbstractPreference::AbstractPreference(PreferenceManager* manager, const QString& path, QObject* parent) :
    QObject(parent),
    d_ptr(new AbstractPreferencePrivate(manager, path))
{
    d_ptr->q_ptr = this;
}

AbstractPreference::AbstractPreference(AbstractPreferencePrivate& dd, QObject* parent) :
    QObject(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
}

AbstractPreference::~AbstractPreference()
{
    delete d_ptr;
}

PreferenceManager* AbstractPreference::manager() const
{
    Q_D(const AbstractPreference);
    return d->m_manager;
}

QString AbstractPreference::path() const
{
    Q_D(const AbstractPreference);
    return d->m_path;
}

QString AbstractPreference::fullPath(const QString& path) const
{
    Q_D(const AbstractPreference);

    QString ret = d->m_path;

    if (path.isEmpty()) {
        return ret;
    }

    if (!ret.isEmpty()) {
        ret += QLatin1Char('.');
    }
    ret += path;

    return ret;
}

bool AbstractPreference::import(const QString& fileName)
{
    Q_D(AbstractPreference);

    if (!d->m_manager) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return false;
    }

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qCritical() << DEBUG_PREFIX << "Unable to open file" << fileName;
        return false;
    }

    return import(&file);
}

bool AbstractPreference::import(QIODevice* device)
{
    Q_D(AbstractPreference);

    if (!d->m_manager) {
        qDebug() << DEBUG_PREFIX << "PreferenceManager not available";
        return false;
    }

    PreferenceTransaction trans(this);

    QXmlStreamReader xml(device);
    QStringList paths;
    int depth = 0;

    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isStartElement()) {
            if (depth++ <= 0)
                continue;

            paths << xml.name().toString();
            const QString path = paths.join(QLatin1String("."));

            QXmlStreamAttributes attrs = xml.attributes();
            if (attrs.hasAttribute(QLatin1String("value"))) {
                QStringRef value = attrs.value(QLatin1String("value"));
                if (!value.isNull()) {
                    putString(path, value.toString());
                } else {
                    resetValue(path);
                }
            } else {
                resetValue(path);
            }
        }

        if (xml.isEndElement()) {
            if (--depth <= 0)
                continue;

            if (!paths.empty()) {
                paths.pop_back();
            }
        }
    }

    if (xml.hasError()) {
        qCritical() << DEBUG_PREFIX << xml.errorString();
        return false;
    }

    return trans.commit();
}

bool AbstractPreference::has(const QString& path) const
{
    return hasValue(path);
}

QVariant AbstractPreference::get(const QString& path) const
{
    return getValue(path);
}

QVariant AbstractPreference::get(const QString& path, const QVariant& defaultValue) const
{
    const QVariant value = getValue(path);
    return value.isNull() ? defaultValue : value;
}

void AbstractPreference::put(const QString& path, const QVariant& value)
{
    putValue(path, value);
}

int AbstractPreference::getInt(const QString& path) const
{
    return getValue(path).toInt();
}

int AbstractPreference::getInt(const QString& path, int defaultValue) const
{
    const QVariant value = getValue(path);
    if (!value.isNull()) {
        bool ok = false;
        int ret = value.toInt(&ok);
        if (ok)
            return ret;
    }
    return defaultValue;
}

void AbstractPreference::putInt(const QString& path, int value)
{
    putValue(path, value);
}

uint AbstractPreference::getUInt(const QString& path) const
{
    return getValue(path).toUInt();
}

uint AbstractPreference::getUInt(const QString& path, uint defaultValue) const
{
    const QVariant value = getValue(path);
    if (!value.isNull()) {
        bool ok = false;
        uint ret = value.toUInt(&ok);
        if (ok)
            return ret;
    }
    return defaultValue;
}

void AbstractPreference::putUInt(const QString& path, uint value)
{
    putValue(path, value);
}

qlonglong AbstractPreference::getLongLong(const QString& path) const
{
    return getValue(path).toLongLong();
}

qlonglong AbstractPreference::getLongLong(const QString& path, qlonglong defaultValue) const
{
    const QVariant value = getValue(path);
    if (!value.isNull()) {
        bool ok = false;
        qlonglong ret = value.toLongLong(&ok);
        if (ok)
            return ret;
    }
    return defaultValue;
}

void AbstractPreference::putLongLong(const QString& path, qlonglong value)
{
    putValue(path, value);
}

qulonglong AbstractPreference::getULongLong(const QString& path) const
{
    return getValue(path).toULongLong();
}

qulonglong AbstractPreference::getULongLong(const QString& path, qulonglong defaultValue) const
{
    const QVariant value = getValue(path);
    if (!value.isNull()) {
        bool ok = false;
        qulonglong ret = value.toULongLong(&ok);
        if (ok)
            return ret;
    }
    return defaultValue;
}

void AbstractPreference::putULongLong(const QString& path, qulonglong value)
{
    putValue(path, value);
}

float AbstractPreference::getFloat(const QString& path) const
{
    return getValue(path).toFloat();
}

float AbstractPreference::getFloat(const QString& path, float defaultValue) const
{
    const QVariant value = getValue(path);
    if (!value.isNull()) {
        bool ok = false;
        float ret = value.toFloat(&ok);
        if (ok)
            return ret;
    }
    return defaultValue;
}

void AbstractPreference::putFloat(const QString& path, float value)
{
    putValue(path, value);
}

double AbstractPreference::getDouble(const QString& path) const
{
    return getValue(path).toDouble();
}

double AbstractPreference::getDouble(const QString& path, double defaultValue) const
{
    const QVariant value = getValue(path);
    if (!value.isNull()) {
        bool ok = false;
        double ret = value.toDouble(&ok);
        if (ok)
            return ret;
    }
    return defaultValue;
}

void AbstractPreference::putDouble(const QString& path, double value)
{
    putValue(path, value);
}

qreal AbstractPreference::getReal(const QString& path) const
{
    return getValue(path).toReal();
}

qreal AbstractPreference::getReal(const QString& path, qreal defaultValue) const
{
    const QVariant value = getValue(path);
    if (!value.isNull()) {
        bool ok = false;
        qreal ret = value.toReal(&ok);
        if (ok)
            return ret;
    }
    return defaultValue;
}

void AbstractPreference::putReal(const QString& path, qreal value)
{
    putValue(path, value);
}

bool AbstractPreference::getBool(const QString& path) const
{
    return getValue(path).toBool();
}

bool AbstractPreference::getBool(const QString& path, bool defaultValue) const
{
    const QVariant value = getValue(path);
    if (!value.isNull() && value.canConvert<bool>()) {
        return value.toBool();
    }
    return defaultValue;
}

void AbstractPreference::putBool(const QString& path, bool value)
{
    putValue(path, value);
}

QByteArray AbstractPreference::getAsciiString(const QString& path) const
{
    return getValue(path).toByteArray();
}

QByteArray AbstractPreference::getAsciiString(const QString& path, const QByteArray& defaultValue) const
{
    const QByteArray value = getAsciiString(path);
    return value.isNull() ? defaultValue : value;
}

void AbstractPreference::putAsciiString(const QString& path, const QByteArray& value)
{
    putValue(path, value);
}

QString AbstractPreference::getString(const QString& path) const
{
    return getValue(path).toString();
}

QString AbstractPreference::getString(const QString& path, const QString& defaultValue) const
{
    const QString value = getString(path);
    return value.isNull() ? defaultValue : value;
}

void AbstractPreference::putString(const QString& path, const QString& value)
{
    putValue(path, value);
}

QDate AbstractPreference::getDate(const QString& path) const
{
    return getValue(path).toDate();
}

QDate AbstractPreference::getDate(const QString& path, const QDate& defaultValue) const
{
    const QDate value = getDate(path);
    return !value.isValid() ? defaultValue : value;
}

void AbstractPreference::putDate(const QString& path, const QDate& value)
{
    putValue(path, value);
}

QTime AbstractPreference::getTime(const QString& path) const
{
    return getValue(path).toTime();
}

QTime AbstractPreference::getTime(const QString& path, const QTime& defaultValue) const
{
    const QTime value = getTime(path);
    return !value.isValid() ? defaultValue : value;
}

void AbstractPreference::putTime(const QString& path, const QTime& value)
{
    putValue(path, value);
}

QDateTime AbstractPreference::getDateTime(const QString& path) const
{
    return getValue(path).toDateTime();
}

QDateTime AbstractPreference::getDateTime(const QString& path, const QDateTime& defaultValue) const
{
    const QDateTime value = getDateTime(path);
    return !value.isValid() ? defaultValue : value;
}

void AbstractPreference::putDateTime(const QString& path, const QDateTime& value)
{
    putValue(path, value);
}

QSize AbstractPreference::getSize(const QString& path) const
{
    return getSize(path, QSize());
}

QSize AbstractPreference::getSize(const QString& path, const QSize& defaultValue) const
{
    const QString s = getString(path);
    if (s.isEmpty()) { // XXX Use isEmpty instead of isNull
        return defaultValue;
    }

    const QStringList l = s.split(QLatin1Char(','));
    if (l.size() == 1) {
        int n = l[0].toInt();
        return QSize(n, n);
    }
    if (l.size() == 2) {
        return QSize(l[0].toInt(), l[1].toInt());
    }
    return defaultValue;
}

void AbstractPreference::putSize(const QString& path, const QSize& value)
{
    QString s;
    s += QString::number(value.width());
    s += QLatin1Char(',');
    s += QString::number(value.height());
    putValue(path, s);
}

QPoint AbstractPreference::getPoint(const QString& path) const
{
    return getPoint(path, QPoint());
}

QPoint AbstractPreference::getPoint(const QString& path, const QPoint& defaultValue) const
{
    const QString s = getString(path);
    if (s.isEmpty()) { // XXX Use isEmpty instead of isNull
        return defaultValue;
    }

    const QStringList l = s.split(QLatin1Char(','));
    if (l.size() == 1) {
        int n = l[0].toInt();
        return QPoint(n, n);
    }
    if (l.size() == 2) {
        return QPoint(l[0].toInt(), l[1].toInt());
    }
    return defaultValue;
}

void AbstractPreference::putPoint(const QString& path, const QPoint& value)
{
    QString s;
    s += QString::number(value.x());
    s += QLatin1Char(',');
    s += QString::number(value.y());
    putValue(path, s);
}

QRect AbstractPreference::getRect(const QString& path) const
{
    return getRect(path, QRect());
}

QRect AbstractPreference::getRect(const QString& path, const QRect& defaultValue) const
{
    const QString s = getString(path);
    if (s.isEmpty()) { // XXX Use isEmpty instead of isNull
        return defaultValue;
    }

    const QStringList l = s.split(QLatin1Char(','));
    if (l.size() == 2) {
        int point = l[0].toInt();
        int size = l[1].toInt();
        return QRect(point, point, size, size);
    }
    if (l.size() == 4) {
        return QRect(l[0].toInt(), l[1].toInt(), l[2].toInt(), l[3].toInt());
    }
    return defaultValue;
}

void AbstractPreference::putRect(const QString& path, const QRect& value)
{
    QString s;
    s += QString::number(value.x());
    s += QLatin1Char(',');
    s += QString::number(value.y());
    s += QLatin1Char(',');
    s += QString::number(value.width());
    s += QLatin1Char(',');
    s += QString::number(value.height());
    putValue(path, s);
}

QUrl AbstractPreference::getUrl(const QString& path) const
{
    return getValue(path).toUrl();
}

QUrl AbstractPreference::getUrl(const QString& path, const QUrl& defaultValue) const
{
    const QUrl value = getUrl(path);
    return !value.isValid() ? defaultValue : value;
}

void AbstractPreference::putUrl(const QString& path, const QUrl& value)
{
    putValue(path, value);
}

Version AbstractPreference::getVersion(const QString& path) const
{
    return Version(getString(path));
}

Version AbstractPreference::getVersion(const QString& path, const Version& defaultValue) const
{
    const QString s = getString(path);
    if (s.isNull())
        return defaultValue;
    return Version(s);
}

void AbstractPreference::putVersion(const QString& path, const Version& value)
{
    putString(path, value.toString());
}

QList<QVariant> AbstractPreference::getList(const QString& path, const QString& sep) const
{
    const QStringList l = getString(path).split(sep);

    QList<QVariant> value;
    foreach (const QString& s, l) {
        value << QVariant(s);
    }
    return value;
}

QList<QVariant> AbstractPreference::getList(const QString& path, const QList<QVariant>& defaultValue, const QString& sep) const
{
    const QString s = getString(path);
    if (s.isNull())
        return defaultValue;

    const QStringList l = s.split(sep);

    QList<QVariant> value;
    foreach (const QString& s, l) {
        value << QVariant(s);
    }
    return value;
}

void AbstractPreference::putList(const QString& path, const QList<QVariant>& value, const QString& sep)
{
    QStringList l;
    foreach (const QVariant& v, value) {
        l << v.toString();
    }
    putString(path, l.join(sep));
}

QStringList AbstractPreference::getStringList(const QString& path, const QString& sep) const
{
    return getString(path).split(sep);
}

QStringList AbstractPreference::getStringList(const QString& path, const QStringList& defaultValue, const QString& sep) const
{
    const QString s = getString(path);
    return s.isNull() ? defaultValue : s.split(sep);
}

void AbstractPreference::putStringList(const QString& path, const QStringList& value, const QString& sep)
{
    putString(path, value.join(sep));
}

#ifndef BUILDSYS_PREF_DISABLE_GUI
QFont AbstractPreference::getFont(const QString& path, const QWidget* widget) const
{
    return getFont(path, widget ? widget->font() : QFont());
}

QFont AbstractPreference::getFont(const QString& path, const QFont& defaultValue, const QWidget* widget) const
{
    const QString s = getString(path);
    if (s.isEmpty()) { // XXX Use isEmpty instead of isNull
        return defaultValue;
    }

    const QFont appFont = widget ? widget->font() : QFont();
    QStringList descs = s.split(QLatin1Char(','));
    QStringList appDescs = appFont.toString().split(QLatin1Char(','));

    if (descs.size() != appDescs.size()) {
        QVector<QString> v = descs.toVector();
        v.resize(appDescs.size());
        descs = v.toList();
    }

    // Set the segment which is empty to application font segment
    Q_ASSERT(descs.size() == appDescs.size());
    for (int i = 0; i < descs.size(); ++i) {
        if (descs[i].isEmpty())
            descs[i] = appDescs[i];
    }

    QFont value;
    if (value.fromString(descs.join(QLatin1String(",")))) {
        return value;
    } else {
        return defaultValue;
    }
}

void AbstractPreference::putFont(const QString& path, const QFont& value, const QWidget* widget)
{
    const QFont appFont = widget ? widget->font() : QFont();
    QStringList descs = value.toString().split(QLatin1Char(','));
    QStringList appDescs = appFont.toString().split(QLatin1Char(','));

    // Clear the segment which is identical to application font
    for (int i = 0; i < descs.size() && i < appDescs.size(); ++i) {
        if (descs[i] == appDescs[i])
            descs[i].clear();
    }

    putString(path, descs.join(QLatin1String(",")));
}

QColor AbstractPreference::getColor(const QString& path) const
{
    return getValue(path).value<QColor>();
}

QColor AbstractPreference::getColor(const QString& path, const QColor& defaultValue) const
{
    const QColor value = getColor(path);
    return !value.isValid() ? defaultValue : value;
}

void AbstractPreference::putColor(const QString& path, const QColor& value)
{
    putValue(path, value);
}
#endif // BUILDSYS_PREF_DISABLE_GUI

} // namespace ns
