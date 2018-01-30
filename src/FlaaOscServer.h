#ifndef FLAAOSCSERVER_H
#define FLAAOSCSERVER_H

#include "Flaarlib.h"
#include "osc/osclistener.h"
#include "osc/oscsender.h"

class FlaaOscServer
{


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

public: //getter
	OscListener *pOscListener() const;
	OscSender *pOscSender() const;

private: // methods
	FlaaOscServer();
	FlaaOscServer(const FlaaOscServer &);
	virtual ~FlaaOscServer();
	void registerHandler();

private: // members
	static FlaaOscServer *_instance;

	flaarlib::Flaarlib *m_pFlaarlib;
	OscListener *m_pOscListener = 0;
	OscSender *m_pOscSender = 0;

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
