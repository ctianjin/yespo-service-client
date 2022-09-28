#include <QtCore/QByteArray>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include "mainwindow.h"

#ifndef DEBUG_PREFIX
#  define DEBUG_PREFIX "MainWindow:"
#endif // DEBUG_PREFIX

namespace ns {

const QString MainWindow::mainWindowGroup = QLatin1String("MainWindow");
const QString MainWindow::geometryKey = QLatin1String("Geometry");
const QString MainWindow::maximizedKey = QLatin1String("Maximized");
const QString MainWindow::fullScreenKey = QLatin1String("FullScreen");
const QString MainWindow::stateKey = QLatin1String("State");

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags),
    m_settingsGroup(mainWindowGroup)
{
}

MainWindow::~MainWindow()
{
}

QString MainWindow::settingsGroup() const
{
    return m_settingsGroup;
}

void MainWindow::setSettingsGroup(const QString& settingsGroup)
{
    m_settingsGroup = settingsGroup;
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup(m_settingsGroup);

    const QVariant geom = settings.value(geometryKey);
    if (geom.isValid()) {
        setGeometry(geom.toRect());
    } 

    if (settings.value(maximizedKey, false).toBool()) {
        setWindowState(Qt::WindowMaximized);
    }
    if (settings.value(fullScreenKey, false).toBool()) {
        setWindowState(windowState() | Qt::WindowFullScreen);
    }

    const QVariant state = settings.value(stateKey);
    if (state.isValid()) {
        restoreState(state.toByteArray());
    }

    settings.endGroup();
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup(m_settingsGroup);

    if (windowState() & (Qt::WindowMaximized | Qt::WindowFullScreen)) {
        settings.setValue(maximizedKey, static_cast<bool>(windowState() & Qt::WindowMaximized));
        settings.setValue(fullScreenKey, static_cast<bool>(windowState() & Qt::WindowFullScreen));
    } else {
        settings.setValue(maximizedKey, false);
        settings.setValue(fullScreenKey, false);
        settings.setValue(geometryKey, geometry());
    }

    settings.setValue(stateKey, saveState());

    settings.endGroup();
}

} // namespace ns
