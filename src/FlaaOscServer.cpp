#include "FlaaOscServer.h"
#include "logging/FLLogger.h"
#include "logging/MyLogger.h"
#include "handler/FLOPingHandler.h"
#include "handler/FLORepositoryModuleHandler.h"

FlaaOscServer::FlaaOscServer()
{
	m_pFlaarlib = flaarlib::Flaarlib::instance();
	m_pFlaarlib->init();
	flaarlib::MyLogger *l = new flaarlib::MyLogger();
	flaarlib::FLLog::registerLogger(l);
	m_pUdpListener = new OscListener(9109);
	m_pUdpSender = new OscSender(9110);
	registerHandler();

	m_pUdpListener->start();
	m_pUdpSender->start();
}

FlaaOscServer::~FlaaOscServer()
{
	m_pUdpListener->setBRunning(false);
	delete m_pUdpListener;
}

void FlaaOscServer::registerHandler()
{
	m_pUdpListener->registerHandler(new FLOPingHandler());
	m_pUdpListener->registerHandler(new FLORepositoryModuleHandler());
}

void FlaaOscServer::connectSlots()
{
	connect(m_pUdpListener, &OscListener::finished, m_pUdpListener, &QObject::deleteLater);
	connect(m_pUdpListener, &OscListener::finished, this, &FlaaOscServer::listenerThreadFinished);
	connect(m_pUdpListener, &OscListener::started, this, &FlaaOscServer::listenerThreadStarted);
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
