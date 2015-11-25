#ifndef SCHEMAVALVE_H
#define SCHEMAVALVE_H

#include <QGraphicsPolygonItem>
#include <schemaitem.h>
#include <schemaflowmeter.h>
#include <QObject>
#include <QConicalGradient>
#include <QtMath>
#include <QGraphicsScene>

class SchemaValve : public SchemaItem, public QGraphicsPolygonItem
{
    Q_OBJECT
  //  Q_INTERFACES(QGraphicsItem)
public:
    SchemaValve(qreal Width = 30.0f, qreal Length = 45.0f, qreal PosX = 122.5f, qreal PosY = 350.0f, qreal Angle = -90);
    ~SchemaValve();
    qreal m_Position;
    qreal MaxFlow;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    const qreal m_Width, m_Length;
    QConicalGradient* p_Brush;
signals:
    void FlowIncreased();
    void FlowDecreased();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
};

#endif // SCHEMAVALVE_H
