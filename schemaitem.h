#ifndef SCHEMAITEM_H
#define SCHEMAITEM_H
#include <QPoint>
#include <schemaport.h>
#include <pfdcontrol.h>

class PFDControl;
class SchemaItem
{
public:
    SchemaItem();
    SchemaPort* OutletPort, *InletPort;
    SchemaItem* Descedant;
    PFDControl* PFD;
    qreal Flowrate;
    ~SchemaItem();
 //   void setPos(qreal PosX, qreal PosY);
 //   virtual void setPos(void) = 0;
};

#endif // SCHEMAITEM_H
