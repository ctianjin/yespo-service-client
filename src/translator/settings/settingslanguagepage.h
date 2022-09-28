#ifndef SETTINGSLANGUAGEPAGE_H
#define SETTINGSLANGUAGEPAGE_H

#include <QtCore/QScopedPointer>
#include "abstractsettingspage.h"

namespace ns {

class Ui_SettingsLanguagePage;

class SettingsLanguagePage : public AbstractSettingsPage
{
    Q_OBJECT

public:
    explicit SettingsLanguagePage(QWidget* parent = 0);
    virtual ~SettingsLanguagePage();

public Q_SLOTS:
    virtual void accept();

protected:
    virtual void changeEvent(QEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);
    

private:
    QScopedPointer<Ui_SettingsLanguagePage> m_ui;
	
    Q_DISABLE_COPY(SettingsLanguagePage)
};

} // namespace ns

#endif // SETTINGSLANGUAGEPAGE_H
