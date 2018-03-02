#include "FLOFlaarlibBridge.h"
#include "FLOModuleInstancesModel.h"
#include "Flaarlib.h"
#include "modules/FLAudioFileInputModule.h"
#include "modules/FLAudioFileOutputModule.h"
#include "modules/FLMIDIFileInputModule.h"
#include "modules/FLVolumeControl.h"
#include "exceptions/Exceptions.h"
#include "../FlaaOscServer.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"
#include "logging/FLLog.h"

using namespace flaarlib;

FLOFlaarlibBridge::FLOFlaarlibBridge(QObject *parent) : QObject(parent)
{
}

void FLOFlaarlibBridge::setModuleInstancesModel(FLOModuleInstancesModel *model)
{
	this->m_pModuleInstancesModel = model;
	connect(this->m_pModuleInstancesModel, &FLOModuleInstancesModel::addFLOModuleInstance, this, &FLOFlaarlibBridge::addModule);
	connect(this->m_pModuleInstancesModel, &FLOModuleInstancesModel::removeFLOModuleInstance, this, &FLOFlaarlibBridge::removeModule);
}

void FLOFlaarlibBridge::addModule(FLOModuleInstanceDAO *moduleInstance)
{
	switch ( moduleInstance->moduleType())
	{
		case MODULE_TYPE::INPUT:
			switch (moduleInstance->dataType() )
			{
				case DATA_TYPE::AUDIO:
					new FLAudioFileInputModule(moduleInstance->uuid().toString().toStdString(), -1);
					break;
				case DATA_TYPE::MIDI:
					new FLMIDIFileInputModule(moduleInstance->uuid().toString().toStdString());
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
					break;
				default:
					break;
			}
			break;
		case MODULE_TYPE::PROCESSOR:
			switch (moduleInstance->dataType() )
			{
				case DATA_TYPE::AUDIO:
					new FLVolumeControl(moduleInstance->uuid().toString().toStdString(), -1, -1);
					break;
				default:
					break;
			}
			break;
	}
}

void FLOFlaarlibBridge::removeModule(const QUuid &uuid)
{
	Flaarlib::instance()->removeModule(uuid.toString().toStdString());
}
