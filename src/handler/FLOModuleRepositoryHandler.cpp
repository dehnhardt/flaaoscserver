#include "FLOModuleRepositoryHandler.h"

#include "logging/FLLog.h"
#include "Flaarlib.h"
#include "FLRepository.h"
#include "../FlaaOscServer.h"
#include "../flaaoscsdk/oscpkt.hh"
#include "../flaaoscsdk/oscsender.h"

using namespace flaarlib;
using flaarlib::FLLog;

FLOModuleRepositoryHandler::FLOModuleRepositoryHandler()
	:OscHandler ("/ws/repository/get")
{
	m_sHandlerName = "FLOModuleRepositoryHandler";

}

bool FLOModuleRepositoryHandler::sendModuleRepository(bool multiCast)
{
	std::string path = "/ws/repository/module";
	FLLog::debug("start sending module Repository (path: %s)", path.c_str());
	OscSender *sender = FlaaOscServer::instance()->udpSender();
	FLRepository *repository = Flaarlib::instance()->flRepository();
	std::map<MODULE_TYPE,std::map<std::string,MODULE_INFO *>> moduleType = repository->FLRepositoryMap();
	oscpkt::PacketWriter w;
	w.startBundle();
	for( auto moduleTypeIt : moduleType )
	{
		std::string moduleTypeName;
		MODULE_TYPE type = moduleTypeIt.first;
		switch (type)
		{
			case INPUT:
				moduleTypeName = "InputModule";
				break;
			case OUTPUT:
				moduleTypeName = "OutputModule";
				break;
			case PROCESSOR:
				moduleTypeName = "ProcessorModule";
				break;
		}
		FLLog::debug("ModuleType %d, name: %s", static_cast<int>(type), moduleTypeName.c_str());
		std::map<std::string, MODULE_INFO *> lal = moduleTypeIt.second;
		for( auto moduleIt : lal )
		{
			MODULE_INFO *i = moduleIt.second;
			FLLog::debug("\tfunctional name: %s", i->functionalname.c_str());
			FLLog::debug("\tdescription: %s", i->description.c_str());
			oscpkt::Message msgType = oscpkt::Message(path);
			msgType.pushStr(moduleTypeName);
			msgType.pushInt32(i->type);
			msgType.pushInt32(i->dataType);
			msgType.pushStr(i->functionalname);
			msgType.pushStr(i->description);
			w.addMessage(msgType);
		}
	}
	w.endBundle();
	if( multiCast )
		sender->sendPackageMulticast(w);
	else
		sender->sendPackage(w);
	return true;
}

bool FLOModuleRepositoryHandler::handle(UdpSocket *socket, Message *message)
{
	Q_UNUSED(socket)

	if( message->match(prefix()))
	{
		sendModuleRepository(false);
		return true;
	}
	return false;
}
