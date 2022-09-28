#ifndef ABSTRACTDATABASEMODEL_H
#define ABSTRACTDATABASEMODEL_H

#include <QtCore/QAbstractListModel>
#include "dbglobal.h"

namespace ns {

class Database;
class AbstractEntityDao;
class AbstractDatabaseModelPrivate;

class BUILDSYS_DB_DECL AbstractDatabaseModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString modelName READ modelName WRITE setModelName)

public:
    enum Roles
    {
        EntityModelRole = Qt::UserRole + 1
    };

    explicit AbstractDatabaseModel(QObject* parent = 0);
    virtual ~AbstractDatabaseModel();

    bool isValid() const;

    Database* database() const;
    void setDatabase(Database* db);

    QString modelName() const;
    void setModelName(const QString& modelName);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public Q_SLOTS:
    void refresh();
    void refresh(const QModelIndex& index);

protected:
    AbstractDatabaseModel(AbstractDatabaseModelPrivate& dd, QObject* parent = 0);

    virtual void addModels() = 0;
    virtual void addModel(AbstractEntityDao* dao, QAbstractItemModel* model);

    AbstractDatabaseModelPrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractDatabaseModel)
    Q_DISABLE_COPY(AbstractDatabaseModel)
};

} // namespace ns

#endif // ABSTRACTDATABASEMODEL_H
