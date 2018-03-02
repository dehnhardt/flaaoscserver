#include "FLOModuleInstancesHandler.h"
#include "../flaaoscsdk/oscsender.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"
#include "../FlaaOscServer.h"
#include "../model/FLOModuleInstancesModel.h"
#include "logging/FLLog.h"

#include <QDebug>

FLOModuleInstancesHandler::FLOModuleInstancesHandler() :
	QObject(),
	OscHandler ("/ws/instances/modules")
{
	m_sHandlerName="FLOModuleInstancesHandler";
}

FLOModuleInstancesHandler::~FLOModuleInstancesHandler()
{

}

void FLOModuleInstancesHandler::setModel(FLOModuleInstancesModel *model)
{
	this->m_pModuleInstancesModel = model;
	connect(this->m_pModuleInstancesModel,&FLOModuleInstancesModel::moduleInstanceAdded, this, &FLOModuleInstancesHandler::moduleInstanceAdded );
}

bool FLOModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{
	Q_UNUSED(socket);

	flaarlib::FLLog::debug(message->prettyPrint().c_str());
	std::string function = lastPathToken(message->addressPattern());
	if( function == "add")
	{
		flaarlib::FLLog::debug( "Function String: %s", function.c_str());
		FLOModuleInstanceDAO *moduleInstance = new FLOModuleInstanceDAO();
		moduleInstance->deserialize(message);
		FlaaOscServer::instance()->moduleInstancesModel()->addFLOModuleInstance(moduleInstance);
	}
	if( function == "remove")
	{
		QString uuid;
		flaarlib::FLLog::debug( "Function String: %s", function.c_str());
		message->arg().popStr(uuid);
		FlaaOscServer::instance()->moduleInstancesModel()->removeFLOModuleInstance(QUuid(uuid));
	}
	if( function == "save")
	{
		flaarlib::FLLog::debug( "Function String: %s", function.c_str());
		FlaaOscServer::instance()->saveStructure();
	}
	if( function == "structure")
	{
		flaarlib::FLLog::debug( "Function String: %s", function.c_str());
		FlaaOscServer::instance()->moduleInstancesModel()->sendModules();
	}
	return true;
}

bool FLOModuleInstancesHandler::moduleInstanceAdded(FLOModuleInstanceDAO *module)
{
	std::string path = prefix() + "/added";
	flaarlib::FLLog::debug("start sending module (path: %s)", path.c_str());
	OscSender *sender = FlaaOscServer::instance()->udpSender();
	Message msg(path);
	module->serialize(&msg);
	sender->enqueuMessage(msg);
	return(true);
}

bool FLOModuleInstancesHandler::moduleInstanceRemoved(QUuid uuid)
{
	std::string path = prefix() + "/removed";
	flaarlib::FLLog::debug("start sending module (path: %s)", path.c_str());
	OscSender *sender = FlaaOscServer::instance()->udpSender();
	Message msg(path);
	msg.pushStr(uuid.toString());
	sender->enqueuMessage(msg);
	return(true);
}
