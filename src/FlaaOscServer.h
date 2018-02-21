#ifndef FLAAOSCSERVER_H
#define FLAAOSCSERVER_H

#include "Flaarlib.h"
#include "flaaoscsdk/osclistener.h"
#include "flaaoscsdk/oscsender.h"

#include <memory>

#include <QObject>

class FLOModuleInstancesModel;
class FLOFlaarlibBridge;

class FlaaOscServer : public QObject
{
	Q_OBJECT

public:
	// singleton pattern
	static FlaaOscServer *instance()
	{
		static CGuard g;   // Speicherbereinigung
		if (!_instance)
			_instance = new FlaaOscServer();
		return (_instance);
	}

	void testConnection();
	void openSockets();
	void closeSockets();


public: //getter
	OscListener *udpListener() const;
	OscSender *udpSender() const;
	FLOModuleInstancesModel *moduleInstancesModel() const;
	flaarlib::Flaarlib *flaarlib() const;
	FLOFlaarlibBridge *pFlaarlibBride() const;

public: //setter
	void setListenPort(int iListenPort);
	void setSendPort(int iSendHost);
	void setSendHost(const std::string &sSendHost);




public slots:
	void listenerThreadStarted();
	void listenerThreadFinished();
	void onApplicationExit();


private: // methods
	FlaaOscServer();
	FlaaOscServer(const FlaaOscServer &);
	virtual ~FlaaOscServer();
	void registerHandler();
	void connectSlots();

private: // members
	static FlaaOscServer *_instance;

	int m_iListenPort = 0;
	int m_iSendPort = 0;
	std::string m_sSendHost = "";

	flaarlib::Flaarlib *m_pFlaarlib;
	OscListener *m_pUdpListener = 0;
	OscSender *m_pUdpSender = 0;
	QThread *m_pListenerThread = 0;

	std::unique_ptr<FLOModuleInstancesModel> m_pModuleInstancesModel;
	std::unique_ptr<FLOFlaarlibBridge> m_pFlaarlibBride;

	class CGuard
	{
	public:
		~CGuard()
		{
			if ( nullptr != FlaaOscServer::_instance)
			{
				delete FlaaOscServer::_instance;
				FlaaOscServer::_instance = nullptr;
			}
		}
	};

};

#endif // FLAAOSCSERVER_H
