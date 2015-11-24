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
    Q_INTERFACES(QGraphicsItem)
public:
    SchemaValve(qreal Width, qreal Length, qreal PosX, qreal PosY, qreal Angle);
    ~SchemaValve();
    qreal m_Position;
    qreal MaxFlow;
private:
    const qreal m_Width, m_Length;
    QConicalGradient* p_Brush;
signals:
    void FlowIncreased();
    void FlowDecreased();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
};

#endif // SCHEMAVALVE_H
