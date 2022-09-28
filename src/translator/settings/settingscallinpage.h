#ifndef SETTINGSCALLINPAGE_H
#define SETTINGSCALLINPAGE_H

#include <QtCore/QScopedPointer>
#include "abstractsettingspage.h"

namespace ns {

class Ui_SettingsCallinPage;

class SettingsCallinPage : public AbstractSettingsPage
{
    Q_OBJECT

public:
    explicit SettingsCallinPage(QWidget* parent = 0);
    virtual ~SettingsCallinPage();

public Q_SLOTS:
    virtual void accept();

private Q_SLOTS:
	void on_checkBoxCallinPrompt_clicked(bool checked);

protected:
    virtual void changeEvent(QEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);
    

private:
    QScopedPointer<Ui_SettingsCallinPage> m_ui;
	
    Q_DISABLE_COPY(SettingsCallinPage)
};

} // namespace ns

#endif // SETTINGSCALLINPAGE_H
