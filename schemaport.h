#ifndef SCHEMAPORT_H
#define SCHEMAPORT_H
#include <QPointF>
#include <math.h>
//#include <schemaitem.h>
#include <QGraphicsItem>

class SchemaPort : public QPointF
{
public:
    SchemaPort(qreal PosX, qreal PosY, QGraphicsItem* parentItem);
    ~SchemaPort();
    void setRotation(qreal Angle);
    void setPos(qreal dx, qreal dy);
    QGraphicsItem* parent;
};

#endif // SCHEMAPORT_H
