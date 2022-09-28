#ifndef QPID_SETTINGSGENERALPAGE_H
#define QPID_SETTINGSGENERALPAGE_H

#include <QtCore/QScopedPointer>
#include "abstractsettingspage.h"

namespace Qpid {

class Ui_SettingsGeneralPage;

class SettingsGeneralPage : public AbstractSettingsPage
{
    Q_OBJECT

public:
    explicit SettingsGeneralPage(QWidget* parent = 0);
    virtual ~SettingsGeneralPage();

public Q_SLOTS:
    virtual void accept();

protected:
    virtual void changeEvent(QEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);
    

private:
    QScopedPointer<Ui_SettingsGeneralPage> m_ui;
	
    Q_DISABLE_COPY(SettingsGeneralPage)
};

} // namespace Qpid

#endif // QPID_SETTINGSGENERALPAGE_H
