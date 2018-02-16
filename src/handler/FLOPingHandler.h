#ifndef PINGHANDLER_H
#define PINGHANDLER_H

#include "../flaaoscsdk/oschandler.h"

class FLOPingHandler : public OscHandler
{
public:
	FLOPingHandler();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;
	void sendPing();

private:
	int m_iPing = 0;
};

#endif // PINGHANDLER_H
