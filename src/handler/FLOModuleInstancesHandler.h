#ifndef FLOMODULEINSTANCESHANDLER_H
#define FLOMODULEINSTANCESHANDLER_H

#include "../flaaoscsdk/oschandler.h"

class FLOModuleInstancesHandler : public OscHandler
{
public:
	FLOModuleInstancesHandler();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;

};

#endif // FLOMODULEINSTANCESHANDLER_H
