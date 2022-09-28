#include <QString>
#include <QFontDialog>
#include <QColorDialog>
#include "preferencedialog.h"
#include "preference.h"
#include "ui_preferencedialog.h"

namespace ns {

PreferenceDialog::PreferenceDialog(Preference* pref, QWidget* parent) :
    QDialog(parent, /*Qt::MSWindowsFixedSizeDialogHint |*/ Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint),
    m_ui(new Ui_PreferenceDialog),
    m_pref(pref)
{
    m_ui->setupUi(this);
}

PreferenceDialog::~PreferenceDialog()
{
}

void PreferenceDialog::changeEvent(QEvent* event)
{
    QDialog::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

QString PreferenceDialog::path() const
{
    return m_ui->pathEdit->text();
}

void PreferenceDialog::setPath(const QString& path)
{
    m_ui->pathEdit->setText(path);
}

void PreferenceDialog::on_getButton_clicked()
{
    if (m_pref) {
        m_ui->valueEdit->setText(m_pref->getString(path()));
    }
}

void PreferenceDialog::on_setButton_clicked()
{
    if (m_pref) {
        m_pref->putString(path(), m_ui->valueEdit->text());
    }
}

void PreferenceDialog::on_setFontButton_clicked()
{
    if (m_pref) {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, m_pref->getFont(path()), this);
        if (ok) {
            m_pref->putFont(path(), font);
            m_ui->valueEdit->setText(m_pref->getString(path()));
        }
    }
}

void PreferenceDialog::on_setColorButton_clicked()
{
    if (m_pref) {
        QColor color = QColorDialog::getColor(m_pref->getColor(path()), this);
        if (color.isValid()) {
            m_pref->putColor(path(), color);
            m_ui->valueEdit->setText(m_pref->getString(path()));
        }
    }
}

void PreferenceDialog::on_resetButton_clicked()
{
    if (m_pref) {
        m_pref->reset(path());
    }
}

void PreferenceDialog::on_resetAllButton_clicked()
{
    if (m_pref) {
        Preference pref(m_pref, path());
        pref.resetAll();
    }
}

} // namespace ns
