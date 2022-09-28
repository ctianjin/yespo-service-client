#ifndef ABSTRACTENTITYTREEMODEL_P_H
#define ABSTRACTENTITYTREEMODEL_P_H

#include <QtCore/QVariant>
#include "abstractentitytreemodel.h"
#include "dbglobal.h"

namespace ns {

class BUILDSYS_DB_DECL AbstractEntityTreeModelPrivate
{
    Q_DECLARE_PUBLIC(AbstractEntityTreeModel)

public:
    AbstractEntityTreeModel* q_ptr;

    QString m_modelName;
    bool m_editable;
    int m_totalFields;
    QList<int> m_displayFields;

    EntityTreeModelItem* m_rootItem;

    explicit AbstractEntityTreeModelPrivate(int totalFields);
    virtual ~AbstractEntityTreeModelPrivate();

    void resetDisplayFields();

protected:
    QVariant columnValue(EntityTreeModelItem* item, int column) const;
    QString filterString(EntityTreeModelItem* item, int column) const;

private:
    Q_DISABLE_COPY(AbstractEntityTreeModelPrivate)
};

} // namespace ns

#endif // ABSTRACTENTITYTREEMODEL_P_H
