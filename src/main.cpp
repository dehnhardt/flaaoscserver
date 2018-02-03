#include "FlaaOscServer.h"

#include <iostream>
#include <QCoreApplication>
#include <QTimer>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
	using std::cout;
	using std::cerr;

	QCoreApplication app(argc, argv);
	QCommandLineParser *parser = new QCommandLineParser();
	parser->setApplicationDescription("Flexible advanced audio routing osc server");
	parser->addHelpOption();
	parser->addVersionOption();

	QCommandLineOption listenPortOption("l", "the port to listen on", "listenPort");
	listenPortOption.setDefaultValue("9109");
	QCommandLineOption sendHostOption("S", "the host to send to", "sendHost");
	sendHostOption.setDefaultValue("localhost");
	QCommandLineOption sendPortOption("s", "the port to send to", "sendPort");
	sendPortOption.setDefaultValue("9110");
	parser->addOption(listenPortOption);
	parser->addOption(sendHostOption);
	parser->addOption(sendPortOption);

	// Process the actual command line arguments given by the user
	parser->process(app);
	QString lp = parser->value("l");
	QString sh = parser->value("S");
	QString sp = parser->value("s");

	FlaaOscServer *s = FlaaOscServer::instance();
	bool ok;
	if( lp != "" )
	{
		int i = lp.toInt(&ok);
		if( !ok )
		{
			cerr << parser->helpText().toStdString();
			exit(1);
		}
		s->setListenPort(i);
	}
	else
	{
		cerr << parser->helpText().toStdString();
		exit (2);
	}
	if( sp != "" )
	{
		int i = sp.toInt(&ok);
		if( !ok )
		{
			cerr << parser->helpText().toStdString();
			exit(1);
		}
		s->setSendPort(i);
	}
	else
	{
		cerr << parser->helpText().toStdString();
		exit (2);
	}
	if( sh != "" )
		s->setSendHost(sh.toStdString());
	else
	{
		cerr << parser->helpText().toStdString();
		exit (2);
	}
	s->openSockets();
	QTimer::singleShot(1000, [s]() {s->testConnection();} );

	return app.exec();
}
