#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QPointer>
#include <QScopedPointer>
#include <QString>
#include <QDialog>
#include "uiglobal.h"

namespace ns {

class Preference;
class Ui_PreferenceDialog;

class BUILDSYS_UI_DECL PreferenceDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath)

public:
    explicit PreferenceDialog(Preference* pref, QWidget* parent = 0);
    virtual ~PreferenceDialog();

    QString path() const;
    void setPath(const QString& path);

protected:
    virtual void changeEvent(QEvent* event);

private Q_SLOTS:
    void on_getButton_clicked();
    void on_setButton_clicked();
    void on_setFontButton_clicked();
    void on_setColorButton_clicked();
    void on_resetButton_clicked();
    void on_resetAllButton_clicked();

private:
    QScopedPointer<Ui_PreferenceDialog> m_ui;
    QPointer<Preference> m_pref;

    Q_DISABLE_COPY(PreferenceDialog)
};

} // namespace ns

#endif // PREFERENCEDIALOG_H
