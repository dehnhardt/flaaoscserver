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
	m_pOscListener = new OscListener(9109);
	registerHandler();
	m_pOscSender = new OscSender(9110);
	m_pOscListener->start();
	m_pOscSender->start();
}

FlaaOscServer::~FlaaOscServer()
{
	m_pOscListener->setBRunning(false);
	delete m_pOscListener;
}

void FlaaOscServer::registerHandler()
{
	m_pOscListener->registerHandler(new FLOPingHandler());
	m_pOscListener->registerHandler(new FLORepositoryModuleHandler());
}

OscSender *FlaaOscServer::pOscSender() const
{
	return m_pOscSender;
}

void FlaaOscServer::testConnection()
{
	m_pOscSender->ping();
	m_pOscSender->ping();
	m_pOscSender->ping();
	m_pOscSender->ping();
	m_pOscSender->ping();
	m_pOscSender->ping();
}

OscListener *FlaaOscServer::pOscListener() const
{
	return m_pOscListener;
}

FlaaOscServer *FlaaOscServer::_instance = 0;
