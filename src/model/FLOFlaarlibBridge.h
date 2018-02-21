#ifndef FLOFLAARLIBBRIDGE_H
#define FLOFLAARLIBBRIDGE_H

#include <QObject>

class FLOModuleInstanceDAO;

class FLOFlaarlibBridge : public QObject
{
	Q_OBJECT
public:
	explicit FLOFlaarlibBridge(QObject *parent = nullptr);

signals:

public slots:
	void moduleAdded( FLOModuleInstanceDAO *moduleInstance);
	void moduleRemoved( const QUuid &uuid);

};

#endif // FLOFLAARLIBBRIDGE_H
