#include "FLOPingHandler.h"
#include "logging/FLLog.h"

FLOPingHandler::FLOPingHandler() : OscHandler ("/ping")
{

}

bool FLOPingHandler::handle(oscpkt::UdpSocket *socket, oscpkt::Message *message)
{
	int iarg;
	using flaarlib::FLLog;

	if (message->arg().popInt32(iarg).isOkNoMoreArgs())
	{
		FLLog::debug( "Listener: received /ping %d from %s", iarg, socket->packetOrigin().asString().c_str());
		oscpkt::Message repl;
		repl.init("/pong").pushInt32(iarg+1);
		oscpkt::PacketWriter pw;
		pw.init().addMessage(repl);
		socket->sendPacketTo(pw.packetData(), pw.packetSize(), socket->packetOrigin());
		return true;
	}
	return false;
}
