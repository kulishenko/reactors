#include "schemaport.h"

SchemaPort::SchemaPort(qreal PosX, qreal PosY, QGraphicsItem* parentItem) : QPointF(PosX, PosY)
{
parent = parentItem;
}

SchemaPort::~SchemaPort()
{

}
void SchemaPort::setRotation(qreal Angle) {

    qreal s = sin(Angle*M_PI/180);
    qreal c = cos(Angle*M_PI/180);
    qreal py = y();
    qreal px = x();
    // O(ox,oy)
    qreal ox =parent->x();
    qreal oy =parent->y();
    setX(c * (px-ox) - s * (py-oy) + ox);
    setY(s * (px-ox) + c * (py-oy) + oy);

}
void SchemaPort::setPos(qreal dx, qreal dy) {
   setX(x()+dx);
   setY(y()+dy);
}
