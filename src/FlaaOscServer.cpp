#include "FlaaOscServer.h"
#include "logging/FLLogger.h"
#include "logging/MyLogger.h"
#include "handler/FLOPingHandler.h"
#include "handler/FLOModuleRepositoryHandler.h"

#include <QCoreApplication>
#include <QThread>

FlaaOscServer::FlaaOscServer()
{
	m_pFlaarlib = flaarlib::Flaarlib::instance();
	m_pFlaarlib->init();
	flaarlib::MyLogger *l = new flaarlib::MyLogger();
	flaarlib::FLLog::registerLogger(l);
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
	registerHandler();
	connectSlots();

	m_pUdpSender->start();
	m_pListenerThread->start();
}

void FlaaOscServer::closeSockets()
{
	m_pUdpListener->setBRunning(false);
	m_pUdpListener->deleteLater();
	m_pListenerThread->terminate();
	m_pListenerThread->wait();
	m_pListenerThread->deleteLater();
}

void FlaaOscServer::registerHandler()
{
	m_pUdpListener->registerHandler(new FLOPingHandler());
	m_pUdpListener->registerHandler(new FLOModuleRepositoryHandler());
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
	qDebug( "listener thread has started" );
}

void FlaaOscServer::listenerThreadFinished()
{
	qDebug( "listener thread has stopped" );
	m_pUdpListener->deleteLater();
}

void FlaaOscServer::onApplicationExit()
{
	closeSockets();
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

FlaaOscServer *FlaaOscServer::_instance = 0;
