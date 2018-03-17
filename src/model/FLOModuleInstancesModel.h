#ifndef FLOMODULEINSTANCESMODEL_H
#define FLOMODULEINSTANCESMODEL_H

#include <QObject>
#include <QMap>
#include <QUuid>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

class FLOModuleInstanceDAO;

class FLOModuleInstancesModel : public QObject
{
	Q_OBJECT
public:
	explicit FLOModuleInstancesModel(QObject *parent = nullptr);
	void serialize(QXmlStreamWriter *xmlWriter);
	void deserialize(QXmlStreamReader *xmlReader);
	void sendModules(bool multiCast = true);

public: //setter

public: //getter
	FLOModuleInstanceDAO *getFLOModuleInstance( QUuid uuid);
	QMap<QUuid, FLOModuleInstanceDAO *> getModuleInstancesMap();

signals:
	void moduleInstanceAdded( FLOModuleInstanceDAO *moduleInstance, bool multiCast = true);
	void moduleInstanceModified( FLOModuleInstanceDAO *moduleInstance, bool multiCast = true);
	void moduleInstanceRemoved( const QUuid &uuid, bool multiCast = true);

public slots:
	void addFLOModuleInstance( FLOModuleInstanceDAO *moduleInstance, bool multiCast = true );
	void modifyFLOModuleInstance( FLOModuleInstanceDAO *moduleInstance, bool multiCast = true );
	void removeFLOModuleInstance(const QUuid uuid, bool multiCast = true );

private:
	QMap<QUuid, FLOModuleInstanceDAO *> m_moduleInstancesMap;
};

#endif // FLOMODULEINSTANCESMODEL_H
