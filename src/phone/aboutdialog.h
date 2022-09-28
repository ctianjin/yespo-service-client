#ifndef QPID_ABOUTDIALOG_H
#define QPID_ABOUTDIALOG_H

#include <QScopedPointer>
#include <QString>
#include <QIcon>
#include "framelessdialog.h"

namespace Qpid {

class Ui_AboutDialog;

class AboutDialog : public FramelessDialog
{
    Q_OBJECT
//	Q_PROPERTY(QString applicationTitle READ applicationTitle WRITE setApplicationTitle)
	Q_PROPERTY(QIcon applicationIcon READ applicationIcon WRITE setApplicationIcon)

public:
    explicit AboutDialog(QWidget* parent = 0);
    virtual ~AboutDialog();

//    QString applicationTitle() const;
//    void setApplicationTitle(const QString& applicationTitle);

   QIcon applicationIcon() const;
   void setApplicationIcon(const QIcon& applicationIcon);

protected:
    virtual void changeEvent(QEvent* event);

private:
    QScopedPointer<Ui_AboutDialog> m_ui;
	QIcon m_applicationIcon;

    Q_DISABLE_COPY(AboutDialog)
};

} // namespace Qpid

#endif // QPID_ABOUTDIALOG_H
