#include "osclistener.h"
#include "oscpkt.hh"
#include "udp.hh"
#include "logging/FLLog.h"

#include <QString>

using namespace oscpkt;

OscListener::OscListener( int iPortNum) :
	QThread( ),
	OscHandler ("/"),
	m_iPortNum( iPortNum)
{
}

OscListener::~OscListener()
{
	delete m_pUdpSocket;
	m_pUdpSocket = 0;
}

void OscListener::run()
{
	m_bRunning = true;
	init();
	runListener();
}

void OscListener::init()
{
	m_pUdpSocket = new oscpkt::UdpSocket();
}

void OscListener::runListener()
{

	using flaarlib::FLLog;


	m_pUdpSocket->bindTo(m_iPortNum);
	if (!m_pUdpSocket->isOk())
		FLLog::error( "Error opening port %d: %s", m_iPortNum, m_pUdpSocket->errorMessage().c_str());
	else
	{
		FLLog::debug( "Listener started, will listen to packets on port %d", m_iPortNum );
		PacketReader pr;
		while (m_bRunning && m_pUdpSocket->isOk())
		{
			if (m_pUdpSocket->receiveNextPacket(30))
			{
				pr.init(m_pUdpSocket->packetData(), m_pUdpSocket->packetSize());
				oscpkt::Message *message;
				while (pr.isOk() && (message = pr.popMessage()) != 0)
				{
					OscHandler *handler = handlerFor(message);
					if( handler)
						handler->handle(m_pUdpSocket, message);
					else
						FLLog::debug( "Listener: unhandled message: %s", message->addressPattern().c_str());
				}
			}
		}
		FLLog::debug("closing socket");
		m_pUdpSocket->close();
	}
}

void OscListener::setBRunning(bool bRunning)
{
	m_bRunning = bRunning;
}
