#ifndef FLOFLAARLIBBRIDGE_H
#define FLOFLAARLIBBRIDGE_H

#include <QObject>

class FLOModuleInstanceDAO;
class FLOModuleInstancesModel;

class FLOFlaarlibBridge : public QObject
{
	Q_OBJECT
public:
	explicit FLOFlaarlibBridge(QObject *parent = nullptr);

	void setModuleInstancesModel( FLOModuleInstancesModel *model);

signals:

public slots:
	void addModule( FLOModuleInstanceDAO *moduleInstance);
	void removeModule( const QUuid &uuid);

private: //members
	FLOModuleInstancesModel *m_pModuleInstancesModel = 0;

};

#endif // FLOFLAARLIBBRIDGE_H
