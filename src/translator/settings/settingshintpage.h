#ifndef SETTINGSHINTPAGE_H
#define SETTINGSHINTPAGE_H

#include <QtCore/QScopedPointer>
#include "abstractsettingspage.h"

namespace ns {

class Ui_SettingsHintPage;

class SettingsHintPage : public AbstractSettingsPage
{
    Q_OBJECT

public:
    explicit SettingsHintPage(QWidget* parent = 0);
    virtual ~SettingsHintPage();

public Q_SLOTS:
    virtual void accept();

private Q_SLOTS:
	void on_checkBoxCloseHint_clicked(bool checked);

protected:
    virtual void changeEvent(QEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);
    

private:
    QScopedPointer<Ui_SettingsHintPage> m_ui;
	
    Q_DISABLE_COPY(SettingsHintPage)
};

} // namespace ns

#endif // SETTINGSHINTPAGE_H
