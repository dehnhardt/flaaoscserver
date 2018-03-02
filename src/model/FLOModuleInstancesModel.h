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
	void addFLOModuleInstance( FLOModuleInstanceDAO *moduleInstance );
	void removeFLOModuleInstance(const QUuid uuid );

public: //setter

public: //getter
	FLOModuleInstanceDAO *getFLOModuleInstance( QUuid uuid);
	QMap<QUuid, FLOModuleInstanceDAO *> getModuleInstancesMap();

signals:
	void addModuleInstance( FLOModuleInstanceDAO *moduleInstance);
	void moduleInstanceAdded( FLOModuleInstanceDAO *moduleInstance);
	void removeModuleInstance( const QUuid &uuid);
	void moduleInstanceRemoved( const QUuid &uuid);

public slots:

private:
	QMap<QUuid, FLOModuleInstanceDAO *> m_moduleInstancesMap;
};

#endif // FLOMODULEINSTANCESMODEL_H
