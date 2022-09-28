#include <QApplication>
#include "mainwidget.h"
#include <QtCore/QDebug>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	ns::MainWidget w;
	w.show();

    return app.exec();
}
