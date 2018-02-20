#include "FLOModuleInstancesHandler.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"

#include <QDebug>

FLOModuleInstancesHandler::FLOModuleInstancesHandler() :
	OscHandler ("/ws/instances/modules")
{
	m_sHandlerName="FLOModuleInstancesHandler";
}

bool FLOModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{
	Q_UNUSED(socket);

	qDebug() << message->prettyPrint().c_str();
	std::string function = lastPathToken(message->addressPattern());
	if( function == "add")
	{
		FLOModuleInstanceDAO *moduleInstance = new FLOModuleInstanceDAO();
		moduleInstance->deserialize(message);
		qDebug() << "Function String: " << function.c_str();
	}
	return true;
}

