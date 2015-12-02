#ifndef SCHEMACELL_H
#define SCHEMACELL_H
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <schemaitem.h>

class SchemaCell : public SchemaItem
{
    Q_OBJECT
public:
    SchemaCell(qreal Width = 100.0f, qreal Height = 40.0f, qreal PosX = 0.0f, qreal PosY = 0.0f, qreal Angle = 180.0f);
    ~SchemaCell();
private:
    QGraphicsRectItem* p_Electrode;
    QGraphicsEllipseItem* p_Button;
    QGraphicsSimpleTextItem* p_ButtonLabel;

};

#endif // SCHEMACELL_H
