#include <QApplication>
#include <QThreadPool>
#include "phoneapplication.h"
#include "phonemainwindow.h"
#include "logindialog.h"

int main(int argc, char* argv[])
{
    using Qpid::PhoneApplication;
    using Qpid::LoginDialog;
    using Qpid::PhoneMainWindow;

#ifndef QPID_SHARED_LIBS
    Q_INIT_RESOURCE(phone);
    Q_INIT_RESOURCE(ui);
    Q_INIT_RESOURCE(intl);
#endif // QPID_SHARED_LIBS

    PhoneApplication app(argc, argv);
#ifdef QPID_ENABLE_SINGLE_APPLICATION
    if (app.isRunning()) {
        app.sendMessage(app.arguments().value(1));
        return 0;
    }
#endif // QPID_ENABLE_SINGLE_APPLICATION

    if (!app.init())
        return 1;

//     LoginDialog *l = new LoginDialog;
// 	l->show();
//     l->setUserService(app.userService());
// 	l->setCommandService(app.commandService());
//     l->setPreference(app.newPreference(QLatin1String("ui.login")));
	
    PhoneMainWindow w;
//    w.setLoginDialog(l);
	w.setUserService(app.userService());
	w.setSipService(app.sipService());
	w.setAudioService(app.audioService());
	w.setCommandService(app.commandService());
	w.setMediaService(app.mediaService());
    w.setPreference(app.newPreference());
	w.show();
	w.phoneLoadConfig();
//	w.serverSetting();
//    w.connect(l, SIGNAL(loginSucceeded()), SLOT(showMainWindow()));

#ifdef QPID_ENABLE_SINGLE_APPLICATION
    app.setActivationWindow(&w);
#endif // QPID_ENABLE_SINGLE_APPLICATION

// 	if (l->isAutoLogin()) {
// 		l->login();
// 	} else {
//		l->show();
//	}
	

    int ret = app.exec();

    // XXX Remove some warnings
    QThreadPool::globalInstance()->waitForDone();

    return ret;
}
