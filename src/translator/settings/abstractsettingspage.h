#ifndef ABSTRACTSETTINGSPAGE_H
#define ABSTRACTSETTINGSPAGE_H

#include <QWidget>
#include "preferencesupport.h"

namespace ns {

class AbstractSettingsPage : public QWidget, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit AbstractSettingsPage(QWidget* parent = 0);
    virtual ~AbstractSettingsPage();

public Q_SLOTS:
    virtual void accept() = 0;

Q_SIGNALS:
    void modified();

private:
    Q_DISABLE_COPY(AbstractSettingsPage)
};

} // namespace ns

#endif // ABSTRACTSETTINGSPAGE_H
