#include <QCoreApplication>
#include <iostream>
#include <QThread>

#include "FlaaOscServer.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	FlaaOscServer::instance()->testConnection();
	return a.exec();
}
