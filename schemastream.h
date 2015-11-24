#ifndef SCHEMASTREAM_H
#define SCHEMASTREAM_H
#include <QGraphicsPolygonItem>
#include <schemaitem.h>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>

class SchemaStream : public SchemaItem, public QGraphicsPolygonItem
{
    Q_INTERFACES(QGraphicsItem)
public:
    SchemaStream(qreal Size, qreal PosX, qreal PosY, qreal Angle);
    virtual ~SchemaStream();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCHEMASTREAM_H
