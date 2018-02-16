#ifndef FLOMODULEREPOSITORYHANDLER_H
#define FLOMODULEREPOSITORYHANDLER_H

#include "../flaaoscsdk/oschandler.h"

class FLOModuleRepositoryHandler : public OscHandler
{
public:
	FLOModuleRepositoryHandler();

	bool sendModuleRepository();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;
};

#endif // FLOMODULEREPOSITORYHANDLER_H
