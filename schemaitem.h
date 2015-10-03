#ifndef SCHEMAITEM_H
#define SCHEMAITEM_H
#include <QPoint>
#include <schemaport.h>
#include <pfdcontrol.h>
//#include <QObject>
class PFDControl;
class SchemaItem// : public QObject
{
//    Q_OBJECT
public:
    SchemaItem();
    SchemaPort* OutletPort, *InletPort;
    SchemaItem* Descedant;
    PFDControl* PFD;
    qreal Flowrate;
    ~SchemaItem();
 //   void setPos(qreal PosX, qreal PosY);
 //   void setRotation(qreal Angle);
};

#endif // SCHEMAITEM_H
