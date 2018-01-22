#include "oscsender.h"
#include "oscpkt.hh"
#include "udp.hh"

#include <iostream>

using std::cout;
using std::cerr;

using namespace oscpkt;


OscSender::OscSender( int iPortNum) :
	QThread(),
	m_iPortNum(iPortNum)
{

}

void OscSender::run()
{
	runSender();
}

void OscSender::runSender()
{
	UdpSocket sock;
	sock.connectTo("localhost", m_iPortNum);
	if (!sock.isOk())
		cerr << "Error connection to port " << m_iPortNum << ": " << sock.errorMessage() << "\n";
	else
	{
		cout << "Sender started, will send packets to port " << m_iPortNum << std::endl;
		int iping = 1;
		while (sock.isOk())
		{
			Message msg("/ping");
			msg.pushInt32(iping);
			PacketWriter pw;
			pw.startBundle().startBundle().addMessage(msg).endBundle().endBundle();
			bool ok = sock.sendPacket(pw.packetData(), pw.packetSize());
			cout << "Sender: sent /ping " << iping++ << ", ok=" << ok << "\n";
			// wait for a reply ?
			if (sock.receiveNextPacket(30 /* timeout, in ms */))
			{
				PacketReader pr(sock.packetData(), sock.packetSize());
				Message *incoming_msg;
				while (pr.isOk() && (incoming_msg = pr.popMessage()) != 0)
					cout << "Client: received "; // << *incoming_msg << "\n";
			}
			if( iping > 2 )
				break;
		}
		cout << "sock error: " << sock.errorMessage() << " -- is the server running?\n";
	}

}
