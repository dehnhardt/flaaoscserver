#ifndef FLOMODULEINSTANCESHANDLER_H
#define FLOMODULEINSTANCESHANDLER_H

#include "../flaaoscsdk/oschandler.h"

#include <QObject>

class FLOModuleInstanceDAO;

class FLOModuleInstancesHandler : public QObject, public OscHandler
{
	Q_OBJECT
public:
	FLOModuleInstancesHandler();
	virtual ~FLOModuleInstancesHandler();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;

public slots:
	bool addModuleInstance(FLOModuleInstanceDAO *module);
};

#endif // FLOMODULEINSTANCESHANDLER_H
