#ifndef SCHEMAVESSEL_H
#define SCHEMAVESSEL_H

#include <QGraphicsPathItem>
#include <QLinearGradient>
#include <schemaitem.h>
#include <QDebug>



class SchemaVessel : public QObject, public QGraphicsPathItem, public SchemaItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    SchemaVessel(int Height, int Width , int xPos, int yPos, qreal StartLevel);
    ~SchemaVessel();
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
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
      void test();
      void clicked();
protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
//signals:
public slots:
    void changeLevel();
    void fill();

};

#endif // SCHEMAVESSEL_H
