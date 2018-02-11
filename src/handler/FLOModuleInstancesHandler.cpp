#include "FLOModuleInstancesHandler.h"

#include <QDebug>

FLOModuleInstancesHandler::FLOModuleInstancesHandler() :
	OscHandler ("/ws/instances/modules")
{

}

bool FLOModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{
	qDebug() << message->addressPattern().c_str();
}

