#ifndef SETTINGSUPDATEPAGE_H
#define SETTINGSUPDATEPAGE_H

#include <QtCore/QScopedPointer>
#include "abstractsettingspage.h"

namespace ns {

class Ui_SettingsUpdatePage;

class SettingsUpdatePage : public AbstractSettingsPage
{
    Q_OBJECT

public:
    explicit SettingsUpdatePage(QWidget* parent = 0);
    virtual ~SettingsUpdatePage();

public Q_SLOTS:
    virtual void accept();

protected:
    virtual void changeEvent(QEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);
    

private:
    QScopedPointer<Ui_SettingsUpdatePage> m_ui;
	
    Q_DISABLE_COPY(SettingsUpdatePage)
};

} // namespace ns

#endif // SETTINGSUPDATEPAGE_H
