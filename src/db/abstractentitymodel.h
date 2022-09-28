#ifndef ABSTRACTENTITYMODEL_H
#define ABSTRACTENTITYMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QList>
#include <QtCore/QString>
#include "dbglobal.h"

namespace ns {

class AbstractEntityModelPrivate;

class BUILDSYS_DB_DECL AbstractEntityModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString modelName READ modelName WRITE setModelName)
    Q_PROPERTY(bool editable READ isEditable WRITE setEditable)
    Q_PROPERTY(QList<int> displayFields READ displayFields WRITE setDisplayFields)
    Q_PROPERTY(QVariant entities READ entities WRITE setEntities)

public:
    explicit AbstractEntityModel(int totalFields, QObject* parent = 0);
    virtual ~AbstractEntityModel();

    virtual Qt::ItemFlags flags(const QModelIndex& index) const;

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    QString modelName() const;
    void setModelName(const QString& modelName);

    bool isEditable() const;
    void setEditable(bool editable);

    QList<int> displayFields() const;
    void setDisplayFields(const QList<int>& fields);

    int indexOfDisplayField(int field) const;

    virtual QVariant entities() const = 0;
    virtual void setEntities(const QVariant& entities) = 0;

protected:
    AbstractEntityModel(AbstractEntityModelPrivate& dd, QObject* parent = 0);

    virtual int count() const = 0;
    virtual QString fieldName(int field) const = 0;
    virtual QVariant fieldValue(int row, int field) const = 0;

    AbstractEntityModelPrivate* const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractEntityModel)
    Q_DISABLE_COPY(AbstractEntityModel)
};

} // namespace ns

#endif // ABSTRACTENTITYMODEL_H
