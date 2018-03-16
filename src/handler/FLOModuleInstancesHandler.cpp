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
	connect(this,&FLOModuleInstancesHandler::addModuleInstance, this->m_pModuleInstancesModel, &FLOModuleInstancesModel::addFLOModuleInstance );
	connect(this,&FLOModuleInstancesHandler::modifyModuleInstance, this->m_pModuleInstancesModel, &FLOModuleInstancesModel::modifyFLOModuleInstance );
	connect(this,&FLOModuleInstancesHandler::removeModuleInstance, this->m_pModuleInstancesModel, &FLOModuleInstancesModel::removeFLOModuleInstance );

	connect(this->m_pModuleInstancesModel,&FLOModuleInstancesModel::moduleInstanceAdded, this, &FLOModuleInstancesHandler::moduleInstanceAdded );
	connect(this->m_pModuleInstancesModel,&FLOModuleInstancesModel::moduleInstanceModified, this, &FLOModuleInstancesHandler::moduleInstanceModified );
	connect(this->m_pModuleInstancesModel,&FLOModuleInstancesModel::moduleInstanceRemoved, this, &FLOModuleInstancesHandler::moduleInstanceRemoved);
}

bool FLOModuleInstancesHandler::handle(UdpSocket *socket, Message *message)
{
	Q_UNUSED(socket);

	flaarlib::FLLog::debug(message->prettyPrint().c_str());
	std::string function = lastPathToken(message->addressPattern());
	flaarlib::FLLog::debug( "Function string: %s", function.c_str());
	if( function == "add")
	{
		FLOModuleInstanceDAO *moduleInstance = new FLOModuleInstanceDAO();
		moduleInstance->deserialize(message);
		emit(addModuleInstance(moduleInstance));
	}
	if( function == "modify")
	{
		FLOModuleInstanceDAO *moduleInstance = new FLOModuleInstanceDAO();
		moduleInstance->deserialize(message);
		emit(modifyModuleInstance(moduleInstance));
	}
	if( function == "remove")
	{
		QString uuid;
		message->arg().popStr(uuid);
		flaarlib::FLLog::debug( "Calling remove for UUDI: %s", uuid.toStdString().c_str());
		emit(removeModuleInstance(uuid));
	}
	if( function == "save")
	{
		flaarlib::FLLog::debug( "Calling save structure");
		FlaaOscServer::instance()->saveStructure();
	}
	if( function == "structure")
	{
		flaarlib::FLLog::debug( "Calling send structure", function.c_str());
		FlaaOscServer::instance()->moduleInstancesModel()->sendModules();
	}
	return true;
}

bool FLOModuleInstancesHandler::moduleInstanceAdded(FLOModuleInstanceDAO *module)
{
	std::string path = prefix() + "/added";
	flaarlib::FLLog::debug("start sending module added(path: %s)", path.c_str());
	OscSender *sender = FlaaOscServer::instance()->udpSender();
	Message msg(path);
	module->serialize(&msg);
	sender->enqueuMessage(msg);
	return(true);
}

bool FLOModuleInstancesHandler::moduleInstanceModified(FLOModuleInstanceDAO *module)
{
	std::string path = prefix() + "/modified";
	flaarlib::FLLog::debug("start sending module modified(path: %s)", path.c_str());
	OscSender *sender = FlaaOscServer::instance()->udpSender();
	Message msg(path);
	module->serialize(&msg);
	sender->enqueuMessage(msg);
	return(true);
}

bool FLOModuleInstancesHandler::moduleInstanceRemoved(QUuid uuid)
{
	std::string path = prefix() + "/removed";
	flaarlib::FLLog::debug("start sending module removed(path: %s)", path.c_str());
	OscSender *sender = FlaaOscServer::instance()->udpSender();
	Message msg(path);
	msg.pushStr(uuid.toString());
	sender->enqueuMessage(msg);
	return(true);
}
