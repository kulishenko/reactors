#ifndef SCHEMAPORT_H
#define SCHEMAPORT_H
#define _USE_MATH_DEFINES
#include <QPointF>
#include <QGraphicsItem>
#include <math.h>

class SchemaPort : public  QGraphicsLineItem //QPointF
{
public:
    SchemaPort(qreal PosX, qreal PosY, QGraphicsItem* parentItem);
    ~SchemaPort();
//    void setRotation(qreal Angle);
//    void setPos(qreal dx, qreal dy);
//    QGraphicsItem* parent;
    QPointF scenePos() const;
};

#endif // SCHEMAPORT_H
