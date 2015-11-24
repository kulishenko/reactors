#include "schemaport.h"
#include <QDebug>

SchemaPort::SchemaPort(qreal PosX, qreal PosY, QGraphicsItem* parentItem, qreal Angle) :
    QGraphicsLineItem(PosX,PosY,PosX+5*sin(Angle*M_PI/180),PosY+5*cos(Angle*M_PI/180), parentItem)
{
    setPen(QPen(Qt::red));
#ifndef SCHEMA_DEBUG
    setVisible(false);
#endif
}

SchemaPort::~SchemaPort()
{

}

QPointF SchemaPort::scenePos() const
{
    QPointF parentPos = QGraphicsLineItem::scenePos();
    qreal Angle = parentItem()->rotation();
    QPointF Pos = line().p1();

    qreal s = sin(Angle*M_PI/180);
    qreal c = cos(Angle*M_PI/180);

    qreal py = parentPos.y() + Pos.y();
    qreal px = parentPos.x() + Pos.x();
    // O(ox,oy)
    qreal ox =  parentPos.x();
    qreal oy =  parentPos.y();
    QPointF result;
    result.setX(c * (px-ox) - s * (py-oy) + ox);
    result.setY(s * (px-ox) + c * (py-oy) + oy);

    return result;

}

qreal SchemaPort::getAngle() const
{
    qreal x = line().angle() + parentItem()->rotation();
    x = fmod(x, 360);
    if (x < 0)
        x += 360;
    return x;
}
