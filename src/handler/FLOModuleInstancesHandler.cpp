#include "FLOModuleInstancesHandler.h"

#include <QDebug>

FLOModuleInstancesHandler::FLOModuleInstancesHandler() :
	OscHandler ("/ws/instances/modules")
{
	m_sHandlerName="FLOModuleInstancesHandler";
}

bool FLOModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{
	qDebug() << message->prettyPrint().c_str();

}

