#include <QCoreApplication>
#include <iostream>
#include <QThread>

#include "osc/osclistener.h"
#include "osc/oscsender.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	OscListener *oscListener = new OscListener(9109);
	OscSender *oscSender = new OscSender(9110);
	oscListener->start();
	oscSender->start();
	return a.exec();
}
