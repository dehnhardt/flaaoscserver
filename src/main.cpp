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

	// A boolean option with multiple names (-f, --filename)
	QCommandLineOption listenPortOption("lp", "the port to listen on", "listenPort");
	listenPortOption.setDefaultValue("9109");
	QCommandLineOption sendHostOption("sh", "the hoste to send to", "sendHost");
	sendHostOption.setDefaultValue("localhost");
	QCommandLineOption sendPortOption("sp", "the port to send to", "sendPort");
	sendPortOption.setDefaultValue("9110");
	parser->addOption(listenPortOption);
	parser->addOption(sendHostOption);
	parser->addOption(sendPortOption);

	// Process the actual command line arguments given by the user
	parser->process(app);
	QString lp = parser->value("lp");
	QString sh = parser->value("sh");
	QString sp = parser->value("sp");

	FlaaOscServer *s = FlaaOscServer::instance();
	bool ok;
	if( lp != "" )
	{
		int i = lp.toInt(&ok);
		if( !ok )
		{
			cerr << parser->helpText().toStdString();
			exit(0);
		}
		s->setListenPort(i);
	}
	else
	{
		cerr << parser->helpText().toStdString();
		exit (10);
	}
	if( sp != "" )
	{
		int i = sp.toInt(&ok);
		if( !ok )
		{
			cerr << parser->helpText().toStdString();
			exit(0);
		}
		s->setSendPort(i);
	}
	else
	{
		cerr << parser->helpText().toStdString();
		exit (10);
	}
	if( sh != "" )
	{
		cerr << parser->helpText().toStdString();
		s->setSendHost(sh.toStdString());
	}
	else
	{
		cerr << parser->helpText().toStdString();
		exit (10);
	}
	s->openSockets();
	QTimer::singleShot(1000, [s]() {s->testConnection();} );

	return app.exec();
}
