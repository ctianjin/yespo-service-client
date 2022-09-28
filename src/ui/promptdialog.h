#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

#include <QPointer>
#include <QScopedPointer>
#include <QString>
#include <QDialog>

#include "uiglobal.h"
#include "framelessdialog.h"

namespace ns {

class Preference;
class Ui_PromptDialog;

class BUILDSYS_UI_DECL PromptDialog : public FramelessDialog
{
    Q_OBJECT

public:
    explicit PromptDialog(QWidget* parent = 0);
    virtual ~PromptDialog();

	enum PromptButton { ok=0x1, yes=0x2};

	int information(const QString &text, PromptButton button = ok, bool showIcon = true);
	int critical(const QString &text, PromptButton button = ok, bool showIcon = true);
	int question(const QString &text, PromptButton button = ok, bool showIcon = true);
	int warning(const QString &text, PromptButton button = ok, bool showIcon = true);
	int however(const QString &text, PromptButton button = ok, bool showIcon = true);
	int weep(const QString &text, PromptButton button = ok, bool showIcon = true);
protected:
    virtual void changeEvent(QEvent* event);
	
private Q_SLOTS:

private:
    QScopedPointer<Ui_PromptDialog> m_ui;

	int prompt(const QString &image, const QString &text, PromptButton button, bool showIcon);
	
    Q_DISABLE_COPY(PromptDialog)
};

} // namespace ns

#endif // PROMPTDIALOG_H
