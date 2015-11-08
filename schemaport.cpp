#include "schemaport.h"
#include <QDebug>

SchemaPort::SchemaPort(qreal PosX, qreal PosY, QGraphicsItem* parentItem, qreal Angle) :
    QGraphicsLineItem(PosX,PosY,PosX+5*sin(Angle*M_PI/180),PosY+5*cos(Angle*M_PI/180), parentItem) //QPointF(PosX, PosY)
{
    setPen(QPen(Qt::blue));
//    parent = parentItem;
    setVisible(false);
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
    x = fmod(x,360);
    if (x < 0)
        x += 360;
    qDebug() << x;
    return x;
}
//void SchemaPort::setRotation(qreal Angle) {
/*
    qreal s = sin(Angle*M_PI/180);
    qreal c = cos(Angle*M_PI/180);
    qreal py = y();
    qreal px = x();
    // O(ox,oy)
    qreal ox = parent->x();
    qreal oy = parent->y();
    setX(c * (px-ox) - s * (py-oy) + ox);
    setY(s * (px-ox) + c * (py-oy) + oy);

}*/
/*void SchemaPort::setPos(qreal dx, qreal dy) {

   setX(x()+dx);
   setY(y()+dy);
}*/
