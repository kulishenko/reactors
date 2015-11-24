#ifndef SCHEMAFLOWMETER_H
#define SCHEMAFLOWMETER_H
#include <QGraphicsRectItem>
#include <schemaitem.h>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

class SchemaFlowmeter : public SchemaItem,  public QGraphicsRectItem
{
   Q_OBJECT
   Q_PROPERTY(qreal PosX MEMBER m_PosX WRITE setPosX)
   Q_PROPERTY(qreal PosY MEMBER m_PosY WRITE setPosY)
   Q_PROPERTY(qreal Pos MEMBER m_Pos)
public:
    SchemaFlowmeter(qreal Width = 25.0f, qreal Height = 200.0f, qreal PosX = 0.0f, qreal PosY = 0.0f, qreal Pos = 0.0f, int MaxFlow = 100);
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
   qreal m_PosX, m_PosY; // Scene coordinates
   qreal m_Pos; // Floater position (frac)
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:
   qreal getFlowrateSet();
protected:
   void mousePressEvent(QGraphicsSceneMouseEvent *event);
   void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private slots:
   void animFloater(qreal Value);
   void animFinished();
   void setPosX(const int Value);
   void setPosY(const int Value);
};

#endif // SCHEMAFLOWMETER_H
