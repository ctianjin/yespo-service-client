#ifndef DATABASEREGISTRAR_H
#define DATABASEREGISTRAR_H

#include <QtCore/QObject>
#include "dbglobal.h"

QT_BEGIN_NAMESPACE
class QSqlError;
QT_END_NAMESPACE

namespace ns {

class Database;
class DatabaseRegistrarPrivate;

class BUILDSYS_DB_DECL DatabaseRegistrar : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseRegistrar(QObject* parent = 0);
    virtual ~DatabaseRegistrar();

    void registerDatabase(Database* db);
    void unregisterDatabase(Database* db);

    bool isError() const;
    QString errorText() const;
    QSqlError lastError() const;

Q_SIGNALS:
    void error(Database* db, const QSqlError& e);

private:
    DatabaseRegistrarPrivate* const d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_databaseDestroyed(QObject*))
    Q_PRIVATE_SLOT(d_func(), void _q_error(const QSqlError&))

    Q_DECLARE_PRIVATE(DatabaseRegistrar)
    Q_DISABLE_COPY(DatabaseRegistrar)
};

} // namespace ns

#endif // DATABASEREGISTRAR_H
