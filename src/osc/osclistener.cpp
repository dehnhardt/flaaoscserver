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

void OscListener::run()
{
	m_bRunning = true;
	runListener();
}

void OscListener::runListener()
{

	using flaarlib::FLLog;

	using namespace oscpkt;

	UdpSocket *socket = new UdpSocket();
	socket->bindTo(m_iPortNum);
	if (!socket->isOk())
		FLLog::error( "Error opening port %d: %s", m_iPortNum, socket->errorMessage().c_str());
	else
	{
		FLLog::debug( "Listener started, will listen to packets on port %d", m_iPortNum );
		PacketReader pr;
		PacketWriter pw;
		while (m_bRunning && socket->isOk())
		{
			if (socket->receiveNextPacket(30))
			{
				pr.init(socket->packetData(), socket->packetSize());
				oscpkt::Message *message;
				while (pr.isOk() && (message = pr.popMessage()) != 0)
				{
					OscHandler *handler = handlerFor(message);
					if( handler)
						handler->handle(socket, message);
					else
						FLLog::debug( "Listener: unhandled message: %s", message->addressPattern().c_str());
				}
			}
		}
		FLLog::debug("closing socket");
		socket->close();
	}
}

void OscListener::setBRunning(bool bRunning)
{
	m_bRunning = bRunning;
}
