#ifndef SCHEMAVESSEL_H
#define SCHEMAVESSEL_H

#include <QGraphicsPathItem>
#include <QLinearGradient>
#include <schemaitem.h>
#include <QDebug>
#include <QColor>
//#include <pfdcontrol.h>

//class PFDControl;
class SchemaCSTR : public QObject, public QGraphicsPathItem, public SchemaItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    SchemaCSTR(int Height, int Width , int xPos, int yPos, qreal StartLevel, int Index = 1);
    ~SchemaCSTR();
//    PFDControl* PFD;
    QLinearGradient* Gradient;
    QPainterPath* path;
    QPainterPath* Stirpath;
    qreal LiquidLevel;
    qreal LiquidLevelSet;
    qreal* tau;
    QGraphicsEllipseItem* Motor;
    QGraphicsSimpleTextItem* MotorLabel;
    QGraphicsPathItem* Stir;
    QGraphicsPolygonItem* Mixer;
    QGraphicsRectItem* Ring1;
    QGraphicsRectItem* Ring2;
    QColor LiquidBottomColor;
    QColor LiquidTopColor;
    QColor GasColor;
    bool isWorking;
    bool isFeeding;
    bool isReady;
    qreal MixerAngle;
    int numInCascade;
    void setLevel(qreal Level, int TransTime);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void test();
    void clicked();

public slots:
    void changeLevel();
    void fill();
    void activateMotor();

    void startFeed();
private slots:
    void animLevel(qreal Value);
    void animMotor(qreal Value);
    void animFinished();
};


#endif // SCHEMAVESSEL_H
