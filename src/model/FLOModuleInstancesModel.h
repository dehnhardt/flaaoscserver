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
	void sendModules();

public: //setter

public: //getter
	FLOModuleInstanceDAO *getFLOModuleInstance( QUuid uuid);
	QMap<QUuid, FLOModuleInstanceDAO *> getModuleInstancesMap();

signals:
	void moduleInstanceAdded( FLOModuleInstanceDAO *moduleInstance);
	void moduleInstanceModified( FLOModuleInstanceDAO *moduleInstance);
	void moduleInstanceRemoved( const QUuid &uuid);

public slots:
	void addFLOModuleInstance( FLOModuleInstanceDAO *moduleInstance );
	void modifyFLOModuleInstance( FLOModuleInstanceDAO *moduleInstance );
	void removeFLOModuleInstance(const QUuid uuid );

private:
	QMap<QUuid, FLOModuleInstanceDAO *> m_moduleInstancesMap;
};

#endif // FLOMODULEINSTANCESMODEL_H
