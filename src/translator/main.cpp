#include <QApplication>
#include <QString>
#include <QThreadPool>
#include <BreakpadHandler.h>
#include "phoneapplication.h"
#include "phonemainwindow.h"

int main(int argc, char* argv[])
{
    using ns::PhoneApplication;
    using ns::PhoneMainWindow;

#ifndef BUILDSYS_SHARED_LIBS
    Q_INIT_RESOURCE(phone);
    Q_INIT_RESOURCE(ui);
    Q_INIT_RESOURCE(intl);
#endif // BUILDSYS_SHARED_LIBS

    PhoneApplication app(argc, argv);
#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
    if (app.isRunning()) {
        app.sendMessage(app.arguments().value(1));
        return 0;
    }
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

    if (!app.init())
        return 1;

	QString userName, password;
	if(argc >= 3) {
		userName = QLatin1String(argv[1]);
		password = QLatin1String(argv[2]);
	}

    PhoneMainWindow w;
	w.setUserService(app.userService());
	w.setSipService(app.sipService());
	w.setAudioService(app.audioService());
	w.setCommandService(app.commandService());
	w.setPhotoService(app.photoService());
	w.setCameraService(app.cameraService());
    w.setPreference(app.newPreference());
	w.loadConfig();
	w.show();
	//w.getServer();
	//Auto login
	if(!userName.isEmpty() && !password.isEmpty())
		w.autoLogin(userName, password);

// 	BreakpadQt::GlobalHandler::instance()->setDumpPath(QLatin1String("crashes"));
// 	BreakpadQt::GlobalHandler::instance()->setReporter(QLatin1String("CrashReport"));
// 	BreakpadQt::GlobalHandler::instance()->setReportCrashesToSystem(BreakpadQt::ReportUnhandled);

#ifdef BUILDSYS_ENABLE_SINGLE_APPLICATION
    app.setActivationWindow(&w);
#endif // BUILDSYS_ENABLE_SINGLE_APPLICATION

    int ret = app.exec();

    // XXX Remove some warnings
    QThreadPool::globalInstance()->waitForDone();

    return ret;
}
