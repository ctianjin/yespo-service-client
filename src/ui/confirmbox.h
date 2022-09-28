#ifndef CONFIRMBOX_H
#define CONFIRMBOX_H

#include <QMessageBox>
#include "uiglobal.h"

QT_BEGIN_NAMESPACE
class QCheckBox;
QT_END_NAMESPACE

namespace ns {

class Preference;

class BUILDSYS_UI_DECL ConfirmBox : public QMessageBox
{
    Q_OBJECT
    Q_PROPERTY(bool remembered READ isRemembered WRITE setRemembered)

public:
    explicit ConfirmBox(QWidget* parent = 0);
    ConfirmBox(Icon icon, const QString& title, const QString& text,
        const QString& rememberText = QString(),
        StandardButtons buttons = NoButton, QWidget* parent = 0,
        Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    virtual ~ConfirmBox();

    bool isRemembered() const;
    void setRemembered(bool remembered);

    static bool confirm(QWidget* parent, Preference* rememberPref, const QString& text,
        QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No,
        QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
    static bool confirm(QWidget* parent, Preference* rememberPref, const QString& title,
        const QString& text, const QString& rememberText = QString(),
        QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No,
        QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);

private:
    void init(const QString& rememberText);

    QCheckBox* m_rememberCheckBox;

    Q_DISABLE_COPY(ConfirmBox)
};

} // namespace ns

#endif // CONFIRMBOX_H
