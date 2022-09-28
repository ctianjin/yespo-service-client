#ifndef SETTINGSGENERALPAGE_H
#define SETTINGSGENERALPAGE_H

#include <QtCore/QScopedPointer>
#include "abstractsettingspage.h"

namespace ns {

class Ui_SettingsGeneralPage;

class SettingsGeneralPage : public AbstractSettingsPage
{
    Q_OBJECT

public:
    explicit SettingsGeneralPage(QWidget* parent = 0);
    virtual ~SettingsGeneralPage();

Q_SIGNALS:
	void alwaysOnTop(bool on);
	void showTrayIcon(bool show);

public Q_SLOTS:
    virtual void accept();

protected:
    virtual void changeEvent(QEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);
    
private Q_SLOTS:
	void on_radioButtonMinTray_clicked(bool checked);
	void on_radioButtonExit_clicked(bool checked);

private:
    QScopedPointer<Ui_SettingsGeneralPage> m_ui;
	
    Q_DISABLE_COPY(SettingsGeneralPage)
};

} // namespace ns

#endif // SETTINGSGENERALPAGE_H
