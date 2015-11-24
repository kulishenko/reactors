#include "schemaflowmeter.h"
#include <QLinearGradient>
#include <QTimeLine>

SchemaFlowmeter::SchemaFlowmeter(qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Pos = 0.0, int MaxFlow)
{
    setRect(0, 0, Width, Height);

    QLinearGradient* Gradient = new QLinearGradient(0, 0, Width, 0);

    Gradient->setColorAt(1, QColor::fromRgb(245, 252, 255));
    Gradient->setColorAt(0.5, Qt::white);
    Gradient->setColorAt(0, QColor::fromRgb(209, 244, 255));

    setBrush(*Gradient);
    setPos(PosX,PosY);
    isEnabled = true;
    m_Height = Height;
    m_Width = Width;
    m_MaxFlow = MaxFlow;
    Floater = new QGraphicsPolygonItem( QPolygonF( QVector<QPointF>() << QPointF( 2, 0 )
                                                   << QPointF( Width - 2, 0 )
                                                   << QPointF( Width / 2, 0.75 * Width )
                                                   << QPointF( 2, 0 ) ), this);

    Gradient=new QLinearGradient(Floater->boundingRect().topLeft(), Floater->boundingRect().topRight());

    Gradient->setColorAt(1, Qt::lightGray);
    Gradient->setColorAt(0 ,Qt::black);


    Floater->setBrush(*Gradient);


 //   Floater->setBrush( Qt::gray);
    Floater->setPos(boundingRect().bottomLeft());
    Floater->moveBy(0, -0.75 * Width + Pos * Height);

    OutletPort = new SchemaPort(Width/2, -40, this);
    OutletPipe = new QGraphicsLineItem(Width/2,  -40, Width/2, 0, this);
    QPen gray;
    QBrush graybrush;
    graybrush.setColor(Qt::gray);
    gray.setColor(Qt::gray);


    for(int i = 1; i <= 10; i++){
        Rulers.push_back(new QGraphicsLineItem(0, 0.09*Height*i, Width, 0.09*Height*i, this));
        Rulers.at(i-1)->setPen(gray);

    }


    InletPort = new SchemaPort(Width/2, Height, this, 0);
// TODO: Change to LinePath
    //OutletPipe = new QGraphicsPathItem(this);

    qDebug() << "Flowmeter Inlet: " + QString::number(InletPort->getAngle());
    qDebug() << "Flowmeter Outlet: " + QString::number(OutletPort->getAngle());

    Flowrate = 0.0f;
    m_FlowrateSet = 0.0f;
  //  OutletPipe->setPen(Qt::black);
 //   InletPort = boundingRect().bottomLeft();
}

SchemaFlowmeter::~SchemaFlowmeter()
{

}

void SchemaFlowmeter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(fabs(Flowrate - m_FlowrateSet) > 0.001){
        Flowrate += (Flowrate < m_FlowrateSet) ? 0.00005 : -0.00005;
        Floater->setPos(boundingRect().bottomLeft()+QPointF(0, -0.75*m_Width - Flowrate*m_Height*0.9));
    }
    QGraphicsRectItem::paint(painter,option,widget);

}

qreal SchemaFlowmeter::getFlowrateSet()
{
    return m_FlowrateSet;
}

void SchemaFlowmeter::animFloater(qreal Value)
{
    if(fabs(Flowrate - m_FlowrateSet) > 0.001){
        Flowrate += (Flowrate < m_FlowrateSet) ? 0.00005 : -0.00005;
        Floater->setPos(boundingRect().bottomLeft() + QPointF(0, -0.75 * m_Width - Flowrate * m_Height * 0.9));
    }
}
void SchemaFlowmeter::setFlowrate(qreal Value) {

    m_FlowrateSet = Value;
    if(PFD) PFD->setFlowrate(Value * m_MaxFlow);

/*    QTimeLine *anim = new QTimeLine(1000, this);
    anim->setUpdateInterval(30);
    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (animFloater(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start(); */

}
void SchemaFlowmeter::animFinished()
{
    if (_numScheduledChanges > 0)
        _numScheduledChanges--;
    else
        _numScheduledChanges++;
    sender()->~QObject();
}
void SchemaFlowmeter::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(SchemaMode == RunMode::Edit) {
        clicked();
        setCursor(QCursor(Qt::DragMoveCursor));
        _startPos = event->pos();
        setOpacity(0.75);
    } else
        event->ignore();

}
void SchemaFlowmeter::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF dP =  event->pos() - _startPos;
    moveBy(dP.x(), dP.y());
    emit moved();
}
void SchemaFlowmeter::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    setCursor(QCursor(Qt::ArrowCursor));
    setOpacity(1);
}
