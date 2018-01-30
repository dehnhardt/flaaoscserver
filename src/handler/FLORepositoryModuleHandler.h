#ifndef FLOREPOSITORYMODULEHANDLER_H
#define FLOREPOSITORYMODULEHANDLER_H

#include "../osc/oschandler.h"

class FLORepositoryModuleHandler : public OscHandler
{
public:
	FLORepositoryModuleHandler();

	bool sendModuleRepository();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;
};

#endif // FLOREPOSITORYMODULEHANDLER_H
