#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include "confirmbox.h"
#include "application.h"
#include "preference.h"

namespace ns {

ConfirmBox::ConfirmBox(QWidget* parent) :
    QMessageBox(parent),
    m_rememberCheckBox(0)
{
    init(QString());
}

ConfirmBox::ConfirmBox(Icon icon, const QString& title, const QString& text,
    const QString& rememberText, StandardButtons buttons, QWidget* parent, Qt::WindowFlags flags) :
    QMessageBox(icon, title, text, buttons, parent, flags),
    m_rememberCheckBox(0)
{
    init(rememberText);
}

ConfirmBox::~ConfirmBox()
{
}

void ConfirmBox::init(const QString& rememberText)
{
    m_rememberCheckBox = new QCheckBox(this);
    if (!rememberText.isNull()) {
        m_rememberCheckBox->setText(rememberText);
    } else {
        m_rememberCheckBox->setText(tr("Don't ask again"));
    }

    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(layout());
    QDialogButtonBox* buttons = this->findChild<QDialogButtonBox*>();//qFindChild<QDialogButtonBox*>(this);
    if (gridLayout && buttons) {
        const int index = gridLayout->indexOf(buttons);
        int row = 0, column = 0, rowSpan = 0, columnSpan = 0;

        gridLayout->getItemPosition(index, &row, &column, &rowSpan, &columnSpan);
        QLayoutItem* buttonsItem = gridLayout->takeAt(index);

        gridLayout->addWidget(m_rememberCheckBox, row, column, rowSpan, columnSpan,
            Qt::AlignHCenter | Qt::AlignVCenter);
        gridLayout->addItem(buttonsItem, ++row, column, rowSpan, columnSpan);
    }
}

bool ConfirmBox::isRemembered() const
{
    return m_rememberCheckBox->checkState() == Qt::Checked;
}

void ConfirmBox::setRemembered(bool remembered)
{
    m_rememberCheckBox->setCheckState(Qt::Checked);
}

bool ConfirmBox::confirm(QWidget* parent, Preference* rememberPref, const QString& text,
    QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
    return confirm(parent, rememberPref,
        qApp->applicationName(), text, QString(), buttons, defaultButton);
}

bool ConfirmBox::confirm(QWidget* parent, Preference* rememberPref, const QString& title,
    const QString& text, const QString& rememberText, QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton defaultButton)
{
    Q_ASSERT(rememberPref);

    bool remembered = rememberPref->getBool(QString());
    if (remembered)
        return true;

    ConfirmBox dlg(Question, title, text, rememberText, buttons, parent);
    dlg.setDefaultButton(defaultButton);
    dlg.exec();

    ButtonRole role = dlg.buttonRole(dlg.clickedButton());
    if (role == AcceptRole || role == YesRole) {
        rememberPref->putBool(QString(), dlg.isRemembered());
        return true;
    } else {
        return false;
    }
}

} // namespace ns
