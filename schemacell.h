#ifndef SCHEMACELL_H
#define SCHEMACELL_H
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <schemaitem.h>

class SchemaCell : public SchemaItem, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    SchemaCell(qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Angle = 180);
    ~SchemaCell();
private:
    QGraphicsRectItem* p_Electrode;
};

#endif // SCHEMACELL_H
