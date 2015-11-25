#ifndef SCHEMACELL_H
#define SCHEMACELL_H
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <schemaitem.h>

class SchemaCell : public SchemaItem, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    SchemaCell(qreal Width = 100.0f, qreal Height = 40.0f, qreal PosX = 1255.0f, qreal PosY = 550.0f, qreal Angle = 180.0f);
    ~SchemaCell();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
private:
    QGraphicsRectItem* p_Electrode;

};

#endif // SCHEMACELL_H
