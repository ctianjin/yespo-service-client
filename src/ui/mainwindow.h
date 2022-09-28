#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include "uiglobal.h"

namespace ns {

class BUILDSYS_UI_DECL MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString settingsGroup READ settingsGroup WRITE setSettingsGroup)

public:
    explicit MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    virtual ~MainWindow();

    QString settingsGroup() const;
    void setSettingsGroup(const QString& settingsGroup);

protected:
    static const QString mainWindowGroup;
    static const QString geometryKey;
    static const QString maximizedKey;
    static const QString fullScreenKey;
    static const QString stateKey;

    virtual void readSettings();
    virtual void writeSettings();

private:
    QString m_settingsGroup;

    Q_DISABLE_COPY(MainWindow)
};

} // namespace ns

#endif // MAINWINDOW_H
