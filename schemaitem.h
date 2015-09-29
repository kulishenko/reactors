#ifndef SCHEMAITEM_H
#define SCHEMAITEM_H
#include <QPoint>
#include <schemaport.h>
//#include <QObject>

class SchemaItem// : public QObject
{
//    Q_OBJECT
public:
    SchemaItem();
    SchemaPort* OutletPort, *InletPort;
    SchemaItem* Descedant;
    qreal Flowrate;
    ~SchemaItem();
 //   void setPos(qreal PosX, qreal PosY);
 //   void setRotation(qreal Angle);
};

#endif // SCHEMAITEM_H
