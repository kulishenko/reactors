#ifndef SCHEMAFLOWMETER_H
#define SCHEMAFLOWMETER_H
#include <QGraphicsRectItem>
#include <schemaitem.h>
#include <QPen>

class SchemaFlowmeter : public SchemaItem,  public QGraphicsRectItem
{
   Q_OBJECT
public:
    SchemaFlowmeter(qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Pos, int MaxFlow=100);
    ~SchemaFlowmeter();
   void setFlowrate(qreal Value);
   QGraphicsPolygonItem* Floater; // TODO: Move to private members!
private:
   QVector<QGraphicsLineItem*> Rulers;
   QGraphicsLineItem* OutletPipe, *InletPipe;
   qreal m_Height, m_Width, m_FlowrateSet;
   bool isEnabled;
   int m_MaxFlow; // Max flowrate in l/hr
   int _numScheduledChanges;
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:
   qreal getFlowrateSet();
private slots:
   void animFloater(qreal Value);
   void animFinished();
};

#endif // SCHEMAFLOWMETER_H
