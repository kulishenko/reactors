#ifndef SCHEMAITEM_H
#define SCHEMAITEM_H
#include <QPoint>
#include <schemaport.h>
#include <QObject>
#include <pfdcontrol.h>

class PFDControl;
class SchemaItem  : public QObject
{
    Q_OBJECT
public:
    SchemaItem();
    SchemaPort* OutletPort, *InletPort;
    SchemaItem* Descedant;
    PFDControl* PFD;
    qreal Flowrate;
    ~SchemaItem();

};

#endif // SCHEMAITEM_H
