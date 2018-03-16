#include "flaaoscsdk/osclistener.h"
#include "flaaoscsdk/oscsender.h"
#include "FlaaOscServer.h"
#include "logging/FLLogger.h"
#include "logging/MyLogger.h"
#include "logging/FLLog.h"
#include "handler/FLOPingHandler.h"
#include "handler/FLOModuleRepositoryHandler.h"
#include "handler/FLOModuleInstancesHandler.h"
#include "model/FLOModuleInstancesModel.h"
#include "flaaoscsdk/FLOModuleInstanceDAO.h"

#include <QCoreApplication>
#include <QThread>
#include <QFile>
#include <QTextCodec>

FlaaOscServer::FlaaOscServer()
{
	m_pFlaarlib = flaarlib::Flaarlib::instance();
	m_pFlaarlib->init();
	flaarlib::MyLogger *l = new flaarlib::MyLogger();
	flaarlib::FLLog::registerLogger(l);
	m_pModuleInstancesModel = std::make_unique<FLOModuleInstancesModel>();
}

FlaaOscServer::~FlaaOscServer()
{
	m_pUdpListener->setBRunning(false);
	delete m_pUdpListener;
}

void FlaaOscServer::openSockets()
{
	m_pListenerThread = new QThread(this);
	m_pUdpSender = new OscSender(m_sSendHost, m_iSendPort);
	m_pUdpListener = new OscListener(m_iListenPort);

	m_pUdpListener->moveToThread(m_pListenerThread);
	createGlobalHandlers();
	registerHandler();
	connectSlots();

	m_pUdpSender->start();
	m_pListenerThread->start();
	m_pRepositoryModuleHandler.get()->sendModuleRepository();
}

void FlaaOscServer::closeSockets()
{
	m_pUdpListener->setBRunning(false);
	m_pListenerThread->terminate();
	m_pListenerThread->wait();
	m_pListenerThread->deleteLater();
}

void FlaaOscServer::registerHandler()
{
	m_pUdpListener->registerHandler(new FLOPingHandler());
	m_pUdpListener->registerHandler(m_pRepositoryModuleHandler.get());
	m_pUdpListener->registerHandler(m_pInstancesModuleHandler.get());
}

void FlaaOscServer::connectSlots()
{
	connect(m_pListenerThread, &QThread::started, m_pUdpListener, &OscListener::init);
	connect(m_pListenerThread, &QThread::finished, m_pUdpListener, &OscListener::exit);
	connect(m_pUdpListener, &OscListener::started, this, &FlaaOscServer::listenerThreadStarted);
	connect(m_pUdpListener, &OscListener::finished, this, &FlaaOscServer::listenerThreadFinished);

	// Allow graceful termination of the thread
	connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &FlaaOscServer::onApplicationExit );
}

void FlaaOscServer::createGlobalHandlers()
{
	this->m_pRepositoryModuleHandler = std::make_unique<FLOModuleRepositoryHandler>();
	this->m_pInstancesModuleHandler = std::make_unique<FLOModuleInstancesHandler>();
	this->m_pInstancesModuleHandler.get()->setModel(this->moduleInstancesModel());
}

FLOModuleRepositoryHandler *FlaaOscServer::repositoryModuleHandler() const
{
	return m_pRepositoryModuleHandler.get();
}

FLOFlaarlibBridge *FlaaOscServer::pFlaarlibBride() const
{
	return m_pFlaarlibBride.get();
}

flaarlib::Flaarlib *FlaaOscServer::flaarlib() const
{
	return m_pFlaarlib;
}

FLOModuleInstancesModel *FlaaOscServer::moduleInstancesModel() const
{
	return m_pModuleInstancesModel.get();
}

void FlaaOscServer::setSendHost(const std::string &sSendHost)
{
	m_sSendHost = sSendHost;
}

void FlaaOscServer::setSendPort(int iSendHost)
{
	m_iSendPort = iSendHost;
}

void FlaaOscServer::setListenPort(int iListenPort)
{
	m_iListenPort = iListenPort;
}

OscSender *FlaaOscServer::udpSender() const
{
	return m_pUdpSender;
}

void FlaaOscServer::listenerThreadStarted()
{
	flaarlib::FLLog::debug("listener thread has started" );
}

void FlaaOscServer::listenerThreadFinished()
{
	flaarlib::FLLog::debug( "listener thread has stopped" );
	m_pUdpListener->deleteLater();
}

void FlaaOscServer::onApplicationExit()
{
	closeSockets();
}


void FlaaOscServer::init()
{
	openSockets();
	readStructure();
}

void FlaaOscServer::testConnection()
{
	FLOPingHandler *phandler = static_cast<FLOPingHandler *>(m_pUdpListener->handlerFor("/ping"));
	phandler->sendPing();
	phandler->sendPing();
	phandler->sendPing();
	phandler->sendPing();
	phandler->sendPing();
}

OscListener *FlaaOscServer::udpListener() const
{
	return m_pUdpListener;
}

void FlaaOscServer::saveStructure()
{
	QFile *file = new QFile("/home/dehnhardt/serverstructure.xml");
	if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QXmlStreamWriter *w = new QXmlStreamWriter(file);
	w->setAutoFormatting(true);
	w->setCodec(QTextCodec::codecForName("utf-8"));
	w->writeStartDocument("1.0");
	w->writeStartElement("ModuleInstances");
	m_pModuleInstancesModel->serialize(w);
	w->writeEndElement();
	w->writeEndDocument();
	file->close();
}

void FlaaOscServer::readStructure()
{
	QFile *file = new QFile("/home/dehnhardt/serverstructure.xml");
	if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QXmlStreamReader *xmlReader = new QXmlStreamReader(file);
	while(!xmlReader->atEnd())
	{
		QXmlStreamReader::TokenType t = xmlReader->readNext();
		QStringRef s = xmlReader->name();
		switch( t )
		{
			case QXmlStreamReader::TokenType::StartElement:
				flaarlib::FLLog::debug("Widget: Element Name: %s", s.toString().toStdString().c_str());
				if( s == "ModuleInstances")
					m_pModuleInstancesModel->deserialize(xmlReader);
				break;
			case QXmlStreamReader::TokenType::EndElement:
				if( s == "ModuleInstances")
					return;
				break;
			default:
				break;
		}
	}
	file->close();
}

FlaaOscServer *FlaaOscServer::_instance = 0;
