#include <QCoreApplication>
#include "osclistener.h"
#include "oscsender.h"
#include <iostream>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	if (argc > 1 && strcmp(argv[1], "--listen") == 0)
		OscListener(9109);
	else if (argc > 1 && strcmp(argv[1], "--send") == 0)
		OscSender(9109);
	else
		std::cout << "syntax: --send to run as sender, --listen to run as listener\n";
	return a.exec();
}
