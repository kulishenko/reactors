#ifndef SCHEMASTREAM_H
#define SCHEMASTREAM_H
#include <QGraphicsPolygonItem>
#include <schemaitem.h>


class SchemaStream : public SchemaItem, public QGraphicsPolygonItem
{
    Q_OBJECT
//    Q_INTERFACES(QGraphicsItem)
public:
    SchemaStream(qreal Size, qreal PosX, qreal PosY, qreal Angle);
    virtual ~SchemaStream();
    QRectF boundingRect() const;
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);
};

#endif // SCHEMASTREAM_H
