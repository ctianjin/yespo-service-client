#ifndef ABSTRACTENTITYMODEL_P_H
#define ABSTRACTENTITYMODEL_P_H

#include "abstractentitymodel.h"
#include "dbglobal.h"

namespace ns {

class BUILDSYS_DB_DECL AbstractEntityModelPrivate
{
    Q_DECLARE_PUBLIC(AbstractEntityModel)

public:
    AbstractEntityModel* q_ptr;

    QString m_modelName;
    bool m_editable;
    int m_totalFields;
    QList<int> m_displayFields;

    explicit AbstractEntityModelPrivate(int totalFields);
    virtual ~AbstractEntityModelPrivate();

    void resetDisplayFields();

private:
    Q_DISABLE_COPY(AbstractEntityModelPrivate)
};

} // namespace ns

#endif // ABSTRACTENTITYMODEL_P_H
