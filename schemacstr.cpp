#include "schemacstr.h"
#include <QLinearGradient>
#include <QFont>
#include  <math.h>

SchemaCSTR::SchemaCSTR(int Height, int Width , int xPos, int yPos, qreal StartLevel)
{
    if(StartLevel!=0.0)
        LiquidLevel = LiquidLevelSet = StartLevel;

    MixerAngle=rand();
    isWorking = true;

    InletPort = new SchemaPort(10+xPos,yPos+10, this);
    OutletPort = new SchemaPort(Width+xPos,yPos+80, this);


    // Drawing Vessel
    path = new QPainterPath();
    path->lineTo(0,Height);
    path->quadTo(Width/2,Height+40,Width,Height);
    path->lineTo(Width,0);
    path->quadTo(Width/2,-40,0,0);

    // Drawing Liquid
    Gradient=new QLinearGradient(path->boundingRect().bottomLeft(),path->boundingRect().topLeft());
    Gradient->setColorAt(1,Qt::white);
    Gradient->setColorAt(LiquidLevel,Qt::white);
    Gradient->setColorAt(LiquidLevel-1e-10,Qt::blue);
    Gradient->setColorAt(0,Qt::darkBlue);
    //       path.setFillRule(Qt::WindingFill);
    setPath(*path);
    setBrush(*Gradient);
    setPos(xPos,yPos);

    // Drawing Rings
    Ring1 = new QGraphicsRectItem(this);
    Ring1->setRect(-5,20,Width+10,5);
    Ring2 = new QGraphicsRectItem(this);
    Ring2->setRect(-5,25,Width+10,5);

    // Drawing Motor Block
    Motor = new QGraphicsEllipseItem(this);
    int MotorRadius = 40;
    Motor->setRect(QRect(Width/2-MotorRadius/2,-80,MotorRadius,MotorRadius));

    MotorLabel = new QGraphicsSimpleTextItem("M",Motor);
    MotorLabel->setPos(Motor->boundingRect().topLeft());
    MotorLabel->moveBy(MotorLabel->boundingRect().width(),0);
    QFont MotorLabelFont("Calibri", 22, QFont::Bold);
    MotorLabel->setFont(MotorLabelFont);
//    MotorLabel->


    Stir = new QGraphicsPathItem(Motor);
    Stirpath = new QPainterPath();
    Stirpath->moveTo(Width/2,-40);
    Stirpath->lineTo(Width/2,100);
    Stir->setPath(*Stirpath);


    Mixer = new QGraphicsPolygonItem( QPolygonF( QVector<QPointF>() << QPointF( 0, 0 )  << QPointF( 0, 20 ) << QPointF( 15, 10 ) << QPointF(30,20)  << QPointF(30,0) << QPointF( 15, 10 ) ), this);
 //   Mixer->setPen( QPen(Qt::black) );
    Mixer->setBrush( Qt::gray );
    Mixer->setPos(Width/2-15,100-10);

}

SchemaCSTR::~SchemaCSTR()
{

}
void SchemaCSTR::setLevel(qreal Level) {

    LiquidLevelSet = Level;

}
void SchemaCSTR::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){

// Liquid Level Animations
    if(fabs(LiquidLevel-LiquidLevelSet)>0.001){
        qreal dL = 0.0005;
        LiquidLevel += dL;
        setBrush(Qt::white);
        delete Gradient;
      //  QLinearGradient* Gradient1=new QLinearGradient(path->boundingRect().bottomLeft(),path->boundingRect().topLeft());
        Gradient = new QLinearGradient(path->boundingRect().bottomLeft(),path->boundingRect().topLeft());
// TODO: Garbage Cleaning for Gradient1
         Gradient->setColorAt(1,Qt::white);
         Gradient->setColorAt(LiquidLevel,Qt::white);
         Gradient->setColorAt(LiquidLevel-1e-10,Qt::blue);
         Gradient->setColorAt(0,Qt::darkBlue);


      setBrush(*Gradient);

    }
// Mixer Animation
    if(isWorking) {
        MixerAngle += 0.5;
      // KOCTbI/\b
      Mixer->setTransform(QTransform().translate(15,0).rotate(MixerAngle,Qt::YAxis).translate(-15,0));

    }
QGraphicsPathItem::paint(painter,option,widget);

  }

void SchemaCSTR::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    qDebug() << "Custom item clicked.";
    setLevel(0.5);
}
