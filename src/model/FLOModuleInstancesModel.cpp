#include "FLOModuleInstancesModel.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"
#include "logging/FLLog.h"
#include "Flaarlib.h"
#include "modules/FLAudioFileInputModule.h"
#include "modules/FLAudioFileOutputModule.h"
#include "modules/FLMIDIFileInputModule.h"
#include "modules/FLVolumeControl.h"
#include "exceptions/Exceptions.h"

#include <QDebug>

using namespace flaarlib;

FLOModuleInstancesModel::FLOModuleInstancesModel(QObject *parent)
	: QObject(parent)
{

}

void FLOModuleInstancesModel::serialize(QXmlStreamWriter *xmlWriter)
{
	xmlWriter->writeStartElement("Modules");
	xmlWriter->writeAttribute("count", QString::number(m_moduleInstancesMap.size() ));
	for( auto it : m_moduleInstancesMap )
	{
		FLOModuleInstanceDAO *i = it;
		i->serialize(xmlWriter);
	}
	xmlWriter->writeEndElement();
}

void FLOModuleInstancesModel::deserialize(QXmlStreamReader *xmlReader)
{
	while(!xmlReader->atEnd())
	{
		QXmlStreamReader::TokenType t = xmlReader->readNext();
		QStringRef s = xmlReader->name();
		switch( t )
		{
			case QXmlStreamReader::TokenType::StartElement:
				flaarlib::FLLog::debug("Model: Element Name: %s", s.toString().toStdString().c_str() );
				if( s  == "Module")
				{
					FLOModuleInstanceDAO *i = new FLOModuleInstanceDAO();
					i->deserialize(xmlReader);
					addFLOModuleInstance(i);
				}
				break;
			case QXmlStreamReader::TokenType::EndElement:
				if( s == "Modules")
					return;
				break;
			default:
				break;
		}
	}
}

void FLOModuleInstancesModel::sendModules(bool multiCast)
{
	for( auto moduleInstance : m_moduleInstancesMap)
	{
		flaarlib::FLLog::debug("Added module instance %s ", moduleInstance->uuid().toString().toStdString().c_str());
		emit(moduleInstanceAdded(moduleInstance, multiCast));
	}
}

void FLOModuleInstancesModel::addFLOModuleInstance(FLOModuleInstanceDAO *moduleInstance, bool multiCast)
{
	bool instanceAdded = false;
	switch ( moduleInstance->moduleType())
	{
		case MODULE_TYPE::INPUT:
			switch (moduleInstance->dataType() )
			{
				case DATA_TYPE::AUDIO:
					new FLAudioFileInputModule(moduleInstance->uuid().toString().toStdString(), -1);
					instanceAdded = true;
					break;
				case DATA_TYPE::MIDI:
					new FLMIDIFileInputModule(moduleInstance->uuid().toString().toStdString());
					instanceAdded = true;
					break;
				default:
					break;
			}
			break;
		case MODULE_TYPE::OUTPUT:
			switch (moduleInstance->dataType() )
			{
				case DATA_TYPE::AUDIO:
					new FLAudioFileOutputModule(moduleInstance->uuid().toString().toStdString(), -1);
					instanceAdded = true;
					break;
				default:
					break;
			}
			break;
		case MODULE_TYPE::PROCESSOR:
			switch (moduleInstance->dataType() )
			{
				case DATA_TYPE::ALL:
					new FLVolumeControl(moduleInstance->uuid().toString().toStdString(), -1, -1);
					instanceAdded = true;
					break;
				default:
					break;
			}
			break;
	}
	if( instanceAdded )
	{
		m_moduleInstancesMap[moduleInstance->uuid()] = moduleInstance;
		emit(moduleInstanceAdded(moduleInstance, multiCast));
	}
}

void FLOModuleInstancesModel::modifyFLOModuleInstance(FLOModuleInstanceDAO *moduleInstance, bool multiCast)
{
	// Currently only the Position can change. Nothing more to do here.
	m_moduleInstancesMap[moduleInstance->uuid()] = moduleInstance;
	emit(moduleInstanceModified(moduleInstance, multiCast));
}

void FLOModuleInstancesModel::removeFLOModuleInstance(const QUuid uuid, bool multiCast)
{
	flaarlib::FLLog::debug("try to remove module from instances map");
	flaarlib::FLLog::debug("bridging remove call");

	if(	Flaarlib::instance()->removeModule(uuid.toString().toStdString()) )
	{
		if( m_moduleInstancesMap.remove(uuid) == 1 )
		{
			flaarlib::FLLog::debug("removed module from instances map");
			emit(moduleInstanceRemoved(uuid, multiCast));
		}
		else if( m_moduleInstancesMap.remove(uuid) > 1 )
			flaarlib::FLLog::error("more than one module removed");
		else if( m_moduleInstancesMap.remove(uuid) == 0 )
			flaarlib::FLLog::debug("error when removing module from instances map");
	}
}

FLOModuleInstanceDAO *FLOModuleInstancesModel::getFLOModuleInstance(QUuid uuid)
{
	return m_moduleInstancesMap[uuid];
}

QMap<QUuid, FLOModuleInstanceDAO *> FLOModuleInstancesModel::getModuleInstancesMap()
{
	return m_moduleInstancesMap;
}
