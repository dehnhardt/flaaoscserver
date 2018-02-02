#ifndef OSCSENDER_H
#define OSCSENDER_H

#include "oscpkt.hh"
#include "udp.hh"

#include <QObject>
#include <QQueue>
#include <string>


using namespace oscpkt;
using std::string;

class OscSender : public QObject
{
	Q_OBJECT

public:
	OscSender( int iPortNum);
	OscSender( string sHost, int iPortNum);
	virtual ~OscSender();

	void enqueuMessage(Message message);
	void sendQueuedMessages();
	void sendPackage( PacketWriter pw);
	void start();

protected:
	void run();

private: //methods
	void init();

private: //members
	int m_iPing = 0;
	string m_sHost;
	int m_iPortNum = 0;
	bool m_bRunning = false;
	UdpSocket *m_pUdpSocket = 0;
	QQueue<Message> *m_pMessageQueue = 0;
};

#endif // OSCSENDER_H
