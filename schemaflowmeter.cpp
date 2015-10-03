#include "schemaflowmeter.h"
#include <QLinearGradient>

SchemaFlowmeter::SchemaFlowmeter(qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Pos, int MaxFlow)
{
    setRect(0,0,Width,Height);
    setPos(PosX,PosY);
    Pos = 0.0;
    isEnabled = true;
    this->Height = Height;
    this->Width = Width;
    this->MaxFlow = MaxFlow;
    PFD = NULL;
    Floater = new QGraphicsPolygonItem( QPolygonF( QVector<QPointF>() << QPointF( 2, 0 )  << QPointF( Width-2, 0 ) << QPointF( Width/2,0.75*Width ) << QPointF(2,0)), this);
//    Floater->setPen( QPen(Qt::black) );

    QLinearGradient* Gradient=new QLinearGradient(Floater->boundingRect().topLeft(),Floater->boundingRect().topRight());

    Gradient->setColorAt(1,Qt::lightGray);
    Gradient->setColorAt(0,Qt::black);


    Floater->setBrush(*Gradient);


 //   Floater->setBrush( Qt::gray);
    Floater->setPos(boundingRect().bottomLeft());
    Floater->moveBy(0,-0.75*Width + Pos*Height);

    OutletPort = new SchemaPort(Width/2+PosX, PosY - 40, this);
    OutletPipe = new QGraphicsLineItem(Width/2,  - 40,Width/2, 0, this);


    InletPort = new SchemaPort(Width/2+PosX, PosY + Height, this);
// TODO: Change to LinePath
    //OutletPipe = new QGraphicsPathItem(this);

    Flowrate=0.0f;
    Flowrateset=0.0f;
  //  OutletPipe->setPen(Qt::black);
 //   InletPort = boundingRect().bottomLeft();
}

SchemaFlowmeter::~SchemaFlowmeter()
{

}

void SchemaFlowmeter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(fabs(Flowrate-Flowrateset)>0.001){
        Flowrate += (Flowrate < Flowrateset) ? 0.00005 : -0.00005;
        Floater->setPos(boundingRect().bottomLeft()+QPointF(0,-0.75*Width - Flowrate*Height*0.9));
    }
    QGraphicsRectItem::paint(painter,option,widget);

  }
void SchemaFlowmeter::setFlowrate(qreal Value) {

    Flowrateset = Value;
    if(PFD!=NULL)
        PFD->setFlowrate(Value*MaxFlow);

}
