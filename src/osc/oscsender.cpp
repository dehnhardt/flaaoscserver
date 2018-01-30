#include "oscsender.h"
#include "logging/FLLog.h"

#include <iostream>
#include <QDebug>

using std::cout;
using std::cerr;

using namespace oscpkt;
using flaarlib::FLLog;

OscSender::OscSender( int iPortNum) :
	QThread(),
	m_sHost("localhost"),
	m_iPortNum(iPortNum)
{
	init();
}

OscSender::OscSender(string sHost, int iPortNum) :
	QThread(),
	m_sHost(sHost),
	m_iPortNum(iPortNum)
{
	init();
}

OscSender::~OscSender()
{
	FLLog::debug("Sender: Closing socket");
	m_pUdpSocket->close();
	delete m_pMessageQueue;
	delete m_pUdpSocket;
	m_pMessageQueue = 0;
	m_pUdpSocket = 0;
}

void OscSender::init()
{
	m_pMessageQueue = new QQueue<Message>();
	m_pUdpSocket = new UdpSocket();
	if( m_pUdpSocket->isOk() )
		FLLog::debug("Socket ready, sending to host %s on port %d", m_sHost.c_str(), m_iPortNum);
	else
		FLLog::error("Socket not ready: %s (host %s, port %d)", m_pUdpSocket->errorMessage().c_str(), m_sHost.c_str(), m_iPortNum);

}

void OscSender::enqueuMessage(Message message)
{

	if( m_pMessageQueue )
		m_pMessageQueue->enqueue(message);
	if( ! m_bRunning )
		sendQueuedMessages();
}

void OscSender::sendQueuedMessages()
{
	if( m_pUdpSocket->isOk() )
		m_bRunning = true;
	else
	{
		FLLog::error("Socket not ready: %s", m_pUdpSocket->errorMessage().c_str());
		return;
	}
	while( !m_pMessageQueue->isEmpty())
	{
		Message message = m_pMessageQueue->dequeue();
		PacketWriter pw;
		pw.startBundle().addMessage(message).endBundle();
		sendPackage(pw);
	}
	m_bRunning = false;
}

void OscSender::sendPackage(PacketWriter pw)
{
	bool ok = m_pUdpSocket->sendPacket(pw.packetData(), pw.packetSize());
	if (ok && m_pUdpSocket->receiveNextPacket(30 /* timeout, in ms */))
	{
		PacketReader pr(m_pUdpSocket->packetData(), m_pUdpSocket->packetSize());
		Message *incoming_msg;
		while (pr.isOk() && (incoming_msg = pr.popMessage()) != 0)
			FLLog::debug("Client: received %s", incoming_msg->addressPattern().c_str());
	}
	if( !ok )
		FLLog::error("Sending of packet failed");
	wait(30);
}

void OscSender::ping()
{
	Message msg("/ping");
	msg.pushInt32(m_iPing);
	enqueuMessage(msg);
	qDebug() << "Send ping: " << m_iPing;
	++m_iPing;

}

void OscSender::run()
{
	m_pUdpSocket->connectTo(m_sHost, m_iPortNum);
}
