#ifndef QPID_SETTINGSDIALOG_H
#define QPID_SETTINGSDIALOG_H
#include "framelessdialog.h"

#include <QScopedPointer>
#include <QDialog>
#include "preferencesupport.h"

namespace Qpid {

class Ui_SettingsDialog;

class SettingsDialog : public FramelessDialog, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = 0);
    virtual ~SettingsDialog();

    virtual void setPreference(Preference* pref);
    virtual void accept();

protected:
    virtual void changeEvent(QEvent* event);
	virtual void showEvent(QShowEvent *event);
private Q_SLOTS:
    void on_indexTree_itemSelectionChanged();
    void on_pages_currentChanged(int index);

private:
    void buildIndexTree();
	QPoint offset; 
    QScopedPointer<Ui_SettingsDialog> m_ui;

    Q_DISABLE_COPY(SettingsDialog)
};

} // namespace Qpid

#endif // QPID_SETTINGSDIALOG_H
