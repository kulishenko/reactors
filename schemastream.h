#ifndef SCHEMASTREAM_H
#define SCHEMASTREAM_H
#include <QGraphicsPolygonItem>
#include <schemaitem.h>


class SchemaStream : public SchemaItem
{
    Q_OBJECT
public:
    SchemaStream(qreal Size, qreal PosX, qreal PosY, qreal Angle);
    virtual ~SchemaStream();
};

#endif // SCHEMASTREAM_H
