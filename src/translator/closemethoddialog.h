#ifndef CLOSEMETHODDIALOG_H
#define CLOSEMETHODDIALOG_H

#include <QScopedPointer>
#include <QString>
#include <QIcon>
#include "preferencesupport.h"
#include "framelessdialog.h"

namespace ns {

class Ui_CloseMethodDialog;

class CloseMethodDialog : public FramelessDialog, public PreferenceSupport
{
    Q_OBJECT

public:
    explicit CloseMethodDialog(QWidget* parent = 0);
    virtual ~CloseMethodDialog();

   virtual void setPreference(Preference* pref);

   virtual void accept();

   bool closeWindow();

Q_SIGNALS:
   void showTrayIcon(bool show);
protected:
    virtual void changeEvent(QEvent* event);
	virtual void closeEvent(QCloseEvent* event);
	virtual void applyPreference(const QSet<QString>& paths, bool force);

private:
	bool m_closwWindow;
    QScopedPointer<Ui_CloseMethodDialog> m_ui;

    Q_DISABLE_COPY(CloseMethodDialog)
};

} // namespace ns

#endif // CLOSEMETHODDIALOG_H
