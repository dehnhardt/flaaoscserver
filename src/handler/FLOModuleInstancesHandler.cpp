#include "FLOModuleInstancesHandler.h"

#include <QDebug>

FLOModuleInstancesHandler::FLOModuleInstancesHandler() :
	OscHandler ("/ws/instances/modules/add")
{

}

bool FLOModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{
	qDebug() << message->prettyPrint().c_str();
}

