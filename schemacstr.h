#ifndef SCHEMACSTR_H
#define SCHEMACSTR_H

#include <QGraphicsPathItem>
#include <QLinearGradient>
#include  <math.h>
#include <schemaitem.h>
#include <QDebug>

class SchemaCSTR : public QGraphicsPathItem, public SchemaItem
{
public:

    SchemaCSTR(int Height = 120,  int Width = 90, int xPos = 0 , int yPos = 0,qreal StartLevel = 0.2);
    ~SchemaCSTR();
    QLinearGradient* Gradient;
    QPainterPath* path;
    QPainterPath* Stirpath;
    qreal LiquidLevel;
    qreal LiquidLevelSet;
    QGraphicsEllipseItem* Motor;
    QGraphicsSimpleTextItem* MotorLabel;
    QGraphicsPathItem* Stir;
    QGraphicsPolygonItem* Mixer;
    QGraphicsRectItem* Ring1;
    QGraphicsRectItem* Ring2;
    bool isWorking;
    qreal MixerAngle;
    void setLevel(qreal Level);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCHEMACSTR_H
