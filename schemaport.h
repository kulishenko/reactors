#ifndef SCHEMAPORT_H
#define SCHEMAPORT_H
#define _USE_MATH_DEFINES
#include <QPointF>
#include <QGraphicsItem>
#include <QtMath>
#include <QPen>
class SchemaPort : public  QGraphicsLineItem //QPointF
{
public:
    SchemaPort(qreal PosX, qreal PosY, QGraphicsItem* parentItem, qreal Angle = 90);
    ~SchemaPort();
//    void setRotation(qreal Angle);
//    void setPos(qreal dx, qreal dy);
//    QGraphicsItem* parent;
    QPointF scenePos() const;
    qreal getAngle() const;
};

#endif // SCHEMAPORT_H
