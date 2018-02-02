#include "FlaaOscServer.h"

#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	FlaaOscServer *s = FlaaOscServer::instance();
	QTimer::singleShot(1000, [s]() {s->testConnection();} );

	return a.exec();
}
