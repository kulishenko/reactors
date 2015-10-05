#ifndef SCHEMAFLOWMETER_H
#define SCHEMAFLOWMETER_H
#include <QGraphicsRectItem>
#include <schemaitem.h>
#include <QPen>

class SchemaFlowmeter : public QGraphicsRectItem, public SchemaItem
{
public:
    SchemaFlowmeter(qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Pos, int MaxFlow=100);
    ~SchemaFlowmeter();
   QGraphicsPolygonItem* Floater;
   QVector<QGraphicsLineItem*> Rulers;
   QGraphicsLineItem* OutletPipe, *InletPipe;
   qreal Height, Width, Flowrateset;
   bool isEnabled;
   int MaxFlow; // Max flowrate in l/hr
   void setFlowrate(qreal Value);
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // SCHEMAFLOWMETER_H
