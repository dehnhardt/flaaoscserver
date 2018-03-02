#include "FLOModuleInstancesModel.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"
#include "logging/FLLog.h"

#include <QDebug>

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
				qDebug() << "Model: Element Name: " << s;
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

void FLOModuleInstancesModel::sendModules()
{
	for( auto moduleInstance : m_moduleInstancesMap)
	{
		flaarlib::FLLog::debug("Added module instance %s ", moduleInstance->uuid().toString().toStdString().c_str());
		emit(moduleInstanceAdded(moduleInstance));
	}
}

void FLOModuleInstancesModel::addFLOModuleInstance(FLOModuleInstanceDAO *moduleInstance)
{
	m_moduleInstancesMap[moduleInstance->uuid()] = moduleInstance;
	emit(addModuleInstance(moduleInstance));
	emit(moduleInstanceAdded(moduleInstance));
}

void FLOModuleInstancesModel::removeFLOModuleInstance(const QUuid uuid)
{
	m_moduleInstancesMap.remove(uuid);
	emit(removeModuleInstance(uuid));
	emit(moduleInstanceRemoved(uuid));
}

FLOModuleInstanceDAO *FLOModuleInstancesModel::getFLOModuleInstance(QUuid uuid)
{
	return m_moduleInstancesMap[uuid];
}

QMap<QUuid, FLOModuleInstanceDAO *> FLOModuleInstancesModel::getModuleInstancesMap()
{
	return m_moduleInstancesMap;
}
