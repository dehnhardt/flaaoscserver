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
	bool moduleInstanceAdded(FLOModuleInstanceDAO *module, bool multiCast = true);
	bool moduleInstanceModified(FLOModuleInstanceDAO *module, bool multiCast = true);
	bool moduleInstanceRemoved(QUuid uuid, bool multiCast = true);

signals:
	void addModuleInstance(FLOModuleInstanceDAO *module, bool multiCast = true);
	void modifyModuleInstance(FLOModuleInstanceDAO *module, bool multiCast = true);
	void removeModuleInstance(QUuid uuid, bool multiCast = true);

private: //members
	FLOModuleInstancesModel *m_pModuleInstancesModel;
};

#endif // FLOMODULEINSTANCESHANDLER_H
