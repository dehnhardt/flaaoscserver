#ifndef FLAAOSCSERVER_H
#define FLAAOSCSERVER_H

#include "Flaarlib.h"

#include <memory>

#include <QObject>

class OscListener;
class OscSender;
class FLOModuleInstancesModel;
class FLOFlaarlibBridge;
class FLOModuleRepositoryHandler;
class FLOModuleInstancesHandler;

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

	void init();
	void testConnection();
	void openSockets();
	void closeSockets();
	void saveStructure();
	void readStructure();

public: //getter
	OscListener *udpListener() const;
	OscSender *udpSender() const;
	FLOModuleInstancesModel *moduleInstancesModel() const;
	FLOModuleRepositoryHandler *repositoryModuleHandler() const;
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
	void createGlobalHandlers();

private: // members
	static FlaaOscServer *_instance;

	int m_iListenPort = 0;
	int m_iSendPort = 0;
	std::string m_sSendHost = "";

	flaarlib::Flaarlib *m_pFlaarlib;
	OscListener *m_pUdpListener = 0;
	OscSender *m_pUdpSender = 0;
	QThread *m_pListenerThread = 0;

	// global handler
	std::unique_ptr<FLOModuleInstancesHandler> m_pInstancesModuleHandler;
	std::unique_ptr<FLOModuleRepositoryHandler> m_pRepositoryModuleHandler;

	std::unique_ptr<FLOModuleInstancesModel> m_pModuleInstancesModel;

	// bridge to library
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
