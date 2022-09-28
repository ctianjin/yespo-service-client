#ifndef ABSTRACTPREFERENCE_H
#define ABSTRACTPREFERENCE_H

#include <QtCore/QObject>
#include "prefglobal.h"

QT_BEGIN_NAMESPACE
class QByteArray;
class QDate;
class QDateTime;
class QIODevice;
class QPoint;
class QRect;
class QSize;
class QString;
class QStringList;
class QTime;
class QUrl;
class QVariant;
template <typename T> class QList;
#ifndef BUILDSYS_PREF_DISABLE_GUI
class QFont;
class QColor;
class QWidget;
#endif // BUILDSYS_PREF_DISABLE_GUI
QT_END_NAMESPACE

namespace ns {

class PreferenceManager;
class AbstractPreferencePrivate;
class Version;

class BUILDSYS_PREF_DECL AbstractPreference : public QObject
{
    Q_OBJECT

public:
    AbstractPreference(PreferenceManager* manager, const QString& path, QObject* parent = 0);
    virtual ~AbstractPreference();

    PreferenceManager* manager() const;
    QString path() const;

    bool import(const QString& fileName);
    bool import(QIODevice* device);

    bool has(const QString& path) const;
    QVariant get(const QString& path) const;
    QVariant get(const QString& path, const QVariant& defaultValue) const;
    void put(const QString& path, const QVariant& value);

    int getInt(const QString& path) const;
    int getInt(const QString& path, int defaultValue) const;
    void putInt(const QString& path, int value);

    uint getUInt(const QString& path) const;
    uint getUInt(const QString& path, uint defaultValue) const;
    void putUInt(const QString& path, uint value);

    qlonglong getLongLong(const QString& path) const;
    qlonglong getLongLong(const QString& path, qlonglong defaultValue) const;
    void putLongLong(const QString& path, qlonglong value);

    qulonglong getULongLong(const QString& path) const;
    qulonglong getULongLong(const QString& path, qulonglong defaultValue) const;
    void putULongLong(const QString& path, qulonglong value);

    float getFloat(const QString& path) const;
    float getFloat(const QString& path, float defaultValue) const;
    void putFloat(const QString& path, float value);

    double getDouble(const QString& path) const;
    double getDouble(const QString& path, double defaultValue) const;
    void putDouble(const QString& path, double value);

    qreal getReal(const QString& path) const;
    qreal getReal(const QString& path, qreal defaultValue) const;
    void putReal(const QString& path, qreal value);

    bool getBool(const QString& path) const;
    bool getBool(const QString& path, bool defaultValue) const;
    void putBool(const QString& path, bool value);

    QByteArray getAsciiString(const QString& path) const;
    QByteArray getAsciiString(const QString& path, const QByteArray& defaultValue) const;
    void putAsciiString(const QString& path, const QByteArray& value);

    QString getString(const QString& path) const;
    QString getString(const QString& path, const QString& defaultValue) const;
    void putString(const QString& path, const QString& value);

    QDate getDate(const QString& path) const;
    QDate getDate(const QString& path, const QDate& defaultValue) const;
    void putDate(const QString& path, const QDate& value);

    QTime getTime(const QString& path) const;
    QTime getTime(const QString& path, const QTime& defaultValue) const;
    void putTime(const QString& path, const QTime& value);

    QDateTime getDateTime(const QString& path) const;
    QDateTime getDateTime(const QString& path, const QDateTime& defaultValue) const;
    void putDateTime(const QString& path, const QDateTime& value);

    QSize getSize(const QString& path) const;
    QSize getSize(const QString& path, const QSize& defaultValue) const;
    void putSize(const QString& path, const QSize& value);

    QPoint getPoint(const QString& path) const;
    QPoint getPoint(const QString& path, const QPoint& defaultValue) const;
    void putPoint(const QString& path, const QPoint& value);

    QRect getRect(const QString& path) const;
    QRect getRect(const QString& path, const QRect& defaultValue) const;
    void putRect(const QString& path, const QRect& value);

    QUrl getUrl(const QString& path) const;
    QUrl getUrl(const QString& path, const QUrl& defaultValue) const;
    void putUrl(const QString& path, const QUrl& value);

    Version getVersion(const QString& path) const;
    Version getVersion(const QString& path, const Version& defaultValue) const;
    void putVersion(const QString& path, const Version& value);

    // Lists are represented as separated string internally
    QList<QVariant> getList(const QString& path, const QString& sep = QLatin1String("|")) const;
    QList<QVariant> getList(const QString& path, const QList<QVariant>& defaultValue, const QString& sep = QLatin1String("|")) const;
    void putList(const QString& path, const QList<QVariant>& value, const QString& sep = QLatin1String("|"));

    QStringList getStringList(const QString& path, const QString& sep = QLatin1String("|")) const;
    QStringList getStringList(const QString& path, const QStringList& defaultValue, const QString& sep = QLatin1String("|")) const;
    void putStringList(const QString& path, const QStringList& value, const QString& sep = QLatin1String("|"));

#ifndef BUILDSYS_PREF_DISABLE_GUI
    QFont getFont(const QString& path, const QWidget* widget = 0) const;
    QFont getFont(const QString& path, const QFont& defaultValue, const QWidget* widget = 0) const;
    void putFont(const QString& path, const QFont& value, const QWidget* widget = 0);

    QColor getColor(const QString& path) const;
    QColor getColor(const QString& path, const QColor& defaultValue) const;
    void putColor(const QString& path, const QColor& value);
#endif // BUILDSYS_PREF_DISABLE_GUI

protected:
    AbstractPreference(AbstractPreferencePrivate& dd, QObject* parent = 0);

    virtual QString fullPath(const QString& path) const;

    virtual bool hasValue(const QString& path) const = 0;
    virtual QVariant getValue(const QString& path) const = 0;
    virtual void putValue(const QString& path, const QVariant& value) = 0;
    virtual void resetValue(const QString& path) = 0;

    AbstractPreferencePrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractPreference)
    Q_DISABLE_COPY(AbstractPreference)
};

} // namespace ns

#endif // ABSTRACTPREFERENCE_H
