#include "schemaflowmeter.h"
#include <QLinearGradient>

SchemaFlowmeter::SchemaFlowmeter(qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Pos, int MaxFlow) : SchemaItem(),
    m_FlowrateSet(0), m_FlowrateStepStart(0), m_FlowrateStepStop(0), _numScheduledChanges(0)
{

    QRect rect(0, 0, Width, Height);
    QPainterPath path;
    path.addRect(rect);

    setPath(path);

    QLinearGradient* Gradient = new QLinearGradient(0, 0, Width, 0);

    Gradient->setColorAt(1, QColor::fromRgb(245, 252, 255));
    Gradient->setColorAt(0.5, Qt::white);
    Gradient->setColorAt(0, QColor::fromRgb(209, 244, 255));

    setBrush(*Gradient);
    setPos(PosX,PosY);
    isEnabled = true;
    m_Height = Height;
    m_Width = Width;
    m_Pos = Pos;
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

    setOutletPort(new SchemaPort(Width/2, -40, this));
    OutletPipe = new QGraphicsLineItem(Width/2,  -40, Width/2, 0, this);
    QPen gray;
    QBrush graybrush;
    graybrush.setColor(Qt::gray);
    gray.setColor(Qt::gray);


    for(int i = 1; i <= 10; i++){
        Rulers.push_back(new QGraphicsLineItem(0, 0.09*Height*i, 0.75*Width, 0.09*Height*i, this));
        Rulers.at(i-1)->setPen(gray);
        Labels.push_back(new QGraphicsTextItem(QString::number((11-i)*10),this));
        Labels.at(i-1)->setPos(0.1*Width, 0.09*Height*i - 0.115*Height);
        Labels.at(i-1)->setDefaultTextColor(Qt::white);
        Labels.at(i-1)->setOpacity(0.75);
    }

    setInletPort(new SchemaPort(Width/2, Height, this, 0));
// TODO: Change to LinePath
    //OutletPipe = new QGraphicsPathItem(this);

    m_Flowrate = 0.0f;
    m_FlowrateSet = 0.0f;
  //  OutletPipe->setPen(Qt::black);
 //   InletPort = boundingRect().bottomLeft();
}

SchemaFlowmeter::~SchemaFlowmeter()
{

}

void SchemaFlowmeter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
 /*   if(fabs(Flowrate - m_FlowrateSet) > 0.001){
        Flowrate += (Flowrate < m_FlowrateSet) ? 0.00005 : -0.00005;
        Floater->setPos(boundingRect().bottomLeft()+QPointF(0, -0.75*m_Width - Flowrate*m_Height*0.9));
    } */
    QGraphicsPathItem::paint(painter,option,widget);

}

void SchemaFlowmeter::createAnim()
{
    m_FlowrateStepStart = m_Flowrate;
    m_FlowrateStepStop = m_FlowrateSet;

    QTimeLine *anim = new QTimeLine(500, this);
    anim->setEasingCurve(QEasingCurve::Linear);
    anim->setUpdateInterval(30);
    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (animFloater(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();
}

qreal SchemaFlowmeter::getFlowrateSet()
{
    return m_FlowrateSet;
}

void SchemaFlowmeter::animFloater(qreal Value)
{
    m_Flowrate = m_FlowrateStepStart + (m_FlowrateStepStop - m_FlowrateStepStart) * Value;
    Floater->setPos(boundingRect().bottomLeft() + QPointF(0, -0.75 * m_Width - m_Flowrate * m_Height * 0.9));

}
void SchemaFlowmeter::setFlowrate(qreal Value) {
    //if(PFD) PFD->setFlowrate(Value * m_MaxFlow);


    m_FlowrateSet = Value;
    _numScheduledChanges++;

    if(_numScheduledChanges == 1)
        createAnim();
}

void SchemaFlowmeter::animFinished()
{
    _numScheduledChanges--;

    if(_numScheduledChanges>0)
        createAnim();
    else
        m_Flowrate = m_FlowrateSet;

    emit establishedFlowrate(m_FlowrateSet * m_MaxFlow);
    qDebug() << "Established flowrate: " + QString::number(m_FlowrateSet * m_MaxFlow);

    sender()->~QObject();
}

