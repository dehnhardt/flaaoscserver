#ifndef FLOMODULEINSTANCESHANDLER_H
#define FLOMODULEINSTANCESHANDLER_H

#include "../flaaoscsdk/oschandler.h"

#include <QObject>
#include <QUuid>
#include <memory>

class FLOModuleInstanceDAO;
class FLOModuleInstancesModel;

class FLOModuleInstancesHandler : public QObject, public OscHandler
{
	Q_OBJECT
public:
	FLOModuleInstancesHandler();
	virtual ~FLOModuleInstancesHandler();
	void setModel(FLOModuleInstancesModel *model);

	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;

public slots:
	bool moduleInstanceAdded(FLOModuleInstanceDAO *module);
	bool moduleInstanceRemoved(QUuid uuid);

private: //members
	FLOModuleInstancesModel *m_pModuleInstancesModel;
};

#endif // FLOMODULEINSTANCESHANDLER_H
