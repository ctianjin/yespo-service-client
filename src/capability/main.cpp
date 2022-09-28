#include <QApplication>
#include <QThreadPool>
#include <Qdir>
#include <QDebug>
#include "capabilityapplication.h"
#include "callerdialog.h"
#include "calleedialog.h"
#include "configinfo.h"

int main(int argc, char* argv[])
{

#ifndef QPID_SHARED_LIBS
    Q_INIT_RESOURCE(capability);
    Q_INIT_RESOURCE(ui);
    Q_INIT_RESOURCE(intl);
#endif // QPID_SHARED_LIBS

	ns::CapabilityApplication app(argc, argv);

	const QDir cfgDir(app.applicationDirPath());
	QString cfgFileName = cfgDir.absoluteFilePath(QLatin1String("config.xml"));

	qDebug() << "Load config : " << cfgFileName;

	ns::ConfigInfo config;
	config.reload(cfgFileName);

    if (!app.init())
        return 1;

	if (config.owner() == ns::ConfigInfo::Caller) {

		ns::CallerDialog *callDialog = new ns::CallerDialog;
		callDialog->setSipService(app.sipService());
		callDialog->show();
		callDialog->setConfig(&config);
	}

	if (config.owner() == ns::ConfigInfo::Translator 
		|| config.owner() == ns::ConfigInfo::Callee)
	{
		ns::CalleeDialog *callDialog = new ns::CalleeDialog;
		callDialog->setSipService(app.sipService());
		callDialog->show();
		callDialog->setConfig(&config);
	}	

    int ret = app.exec();

    // XXX Remove some warnings
    QThreadPool::globalInstance()->waitForDone();

    return ret;
}
