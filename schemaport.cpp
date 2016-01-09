#include "schemaport.h"
#include <QDebug>

SchemaPort::SchemaPort(qreal PosX, qreal PosY, QGraphicsItem* parentItem, qreal Angle) :
    QGraphicsLineItem(PosX, PosY, PosX+10*qSin(qDegreesToRadians(Angle)),PosY+10*qCos(qDegreesToRadians(Angle)), parentItem)
{
    setPen(QPen(Qt::red, 2.0));

    setVisible(false); // Comment for debug

}

SchemaPort::~SchemaPort()
{

}

QPointF SchemaPort::scenePos() const
{
    QPointF parentPos = QGraphicsLineItem::scenePos();
    qreal Angle = qDegreesToRadians(parentItem()->rotation());
    QPointF Pos = line().p1();

    qreal s = qSin(Angle);
    qreal c = qCos(Angle);

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
