#ifndef SCHEMAFLOWMETER_H
#define SCHEMAFLOWMETER_H
#include <QGraphicsRectItem>
#include <schemaitem.h>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

class SchemaFlowmeter : public SchemaItem,  public QGraphicsRectItem
{
   Q_OBJECT
   Q_PROPERTY(qreal Pos MEMBER m_Pos)
public:
    SchemaFlowmeter(qreal Width = 25.0f, qreal Height = 200.0f, qreal PosX = 0.0f, qreal PosY = 0.0f, qreal Pos = 0.0f, int MaxFlow = 100);
    ~SchemaFlowmeter();
   void setFlowrate(qreal Value);
   QGraphicsPolygonItem* Floater; // TODO: Move to private members!
   QRectF boundingRect() const;
private:
   QVector<QGraphicsLineItem*> Rulers;
   QGraphicsLineItem* OutletPipe, *InletPipe;
   qreal m_Height, m_Width, m_FlowrateSet;
   bool isEnabled;
   int m_MaxFlow; // Max flowrate in l/hr
   int _numScheduledChanges;
   qreal m_Pos; // Floater position (frac)
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:
   qreal getFlowrateSet();
private slots:
   void animFloater(qreal Value);
   void animFinished();
};

#endif // SCHEMAFLOWMETER_H
