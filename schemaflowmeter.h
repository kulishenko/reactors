#ifndef SCHEMAFLOWMETER_H
#define SCHEMAFLOWMETER_H
#include <QGraphicsRectItem>
#include <schemaitem.h>

class SchemaFlowmeter : public QGraphicsRectItem, public SchemaItem
{
public:
    SchemaFlowmeter(qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Pos);
    ~SchemaFlowmeter();
   QGraphicsPolygonItem* Floater;
   QGraphicsLineItem* OutletPipe, *InletPipe;
   qreal Height, Width, Flowrateset;
   void setFlowrate(qreal Value);
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // SCHEMAFLOWMETER_H
