#ifndef SCHEMACELL_H
#define SCHEMACELL_H
#include <QGraphicsPolygonItem>
#include <schemaitem.h>

class SchemaCell : public QGraphicsPolygonItem, SchemaItem
{
public:
    SchemaCell(qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Angle = 180);
    ~SchemaCell();
};

#endif // SCHEMACELL_H
