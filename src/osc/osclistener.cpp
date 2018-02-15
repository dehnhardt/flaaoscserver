#include "osclistener.h"
#include "oscpkt.hh"
#include "udp.hh"
#include "logging/FLLog.h"

#include <QString>

using flaarlib::FLLog;

OscListener::OscListener( int iPortNum) :
	QObject( ),
	OscHandler ("/"),
	m_iPortNum( iPortNum)
{
}

OscListener::~OscListener()
{
	delete m_pUdpSocket;
	m_pUdpSocket = 0;
}


void OscListener::init()
{
	FLLog::debug("Listener slot init called");
	m_bRunning = true;
	m_pUdpSocket = new oscpkt::UdpSocket();
	emit( started());
	runListener();
}

void OscListener::exit()
{
	FLLog::debug("Listener slot exit called");
	setBRunning(false);
	emit(finished());
}

void OscListener::runListener()
{

	m_pUdpSocket->bindTo(m_iPortNum);
	if (!m_pUdpSocket->isOk())
		FLLog::error( "Error opening port %d: %s", m_iPortNum, m_pUdpSocket->errorMessage().c_str());
	else
	{
		FLLog::debug( "Listener started, will listen to packets on port %d", m_iPortNum );
		PacketReader pr;
		while ( m_pUdpSocket->isOk())
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
					{
						FLLog::debug( "Listener: unhandled message: ");
						FLLog::debug(message->prettyPrint());
					}
				}
			}
			if( !m_bRunning )
				break;
		}
		FLLog::debug("closing socket");
		m_pUdpSocket->close();
	}
}

void OscListener::setBRunning(bool bRunning)
{
	m_bRunning = bRunning;
}
