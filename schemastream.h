#ifndef SCHEMASTREAM_H
#define SCHEMASTREAM_H
#include <QGraphicsPolygonItem>
#include <schemaitem.h>

class SchemaStream : public QGraphicsPolygonItem, public SchemaItem
{
public:
    SchemaStream(qreal Size, qreal PosX, qreal PosY, qreal Angle);
    ~SchemaStream();
};

#endif // SCHEMASTREAM_H
