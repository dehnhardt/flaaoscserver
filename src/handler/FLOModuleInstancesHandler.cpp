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

bool FLOModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{
	Q_UNUSED(socket);

	flaarlib::FLLog::debug(message->prettyPrint().c_str());
	std::string function = lastPathToken(message->addressPattern());
	if( function == "add")
	{
		FLOModuleInstanceDAO *moduleInstance = new FLOModuleInstanceDAO();
		moduleInstance->deserialize(message);
		FlaaOscServer::instance()->moduleInstancesModel()->addFLOModuleInstance(moduleInstance);
		flaarlib::FLLog::debug( "Function String: %s", function.c_str());
	}
	if( function == "save")
	{
		FlaaOscServer::instance()->saveStructure();
		flaarlib::FLLog::debug( "Function String: %s", function.c_str());
	}
	if( function == "structure")
	{
		FlaaOscServer::instance()->moduleInstancesModel()->sendModules();
		flaarlib::FLLog::debug( "Function String: %s", function.c_str());
	}
	return true;
}

bool FLOModuleInstancesHandler::addModuleInstance(FLOModuleInstanceDAO *module)
{
	std::string path = prefix() + "/add";
	flaarlib::FLLog::debug("start sending module (path: %s)", path.c_str());
	OscSender *sender = FlaaOscServer::instance()->udpSender();
	Message msg(path);
	module->serialize(&msg);
	sender->enqueuMessage(msg);
	return(true);
}
