#include "schemavalve.h"
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QWindow>


//TODO : Remove Length or Width !
SchemaValve::SchemaValve(qreal Width, qreal Length, qreal PosX, qreal PosY, qreal Angle) :
    SchemaItem(), QGraphicsPolygonItem(), m_Width(Width), m_Length(Length)
{
    setPolygon(QPolygonF( QVector<QPointF>() << QPointF( 0, 0 )  << QPointF( 0, Width ) << QPointF( 0.75* Width, 0.5* Width ) << QPointF(Length,30)  << QPointF(Length,0) << QPointF( 0.75* Width,  0.5* Width )));
    setPen( QPen(Qt::black) );

    p_Brush = new QConicalGradient(0.75* Width, 0.5* Width, 90);
    p_Brush->setColorAt(0.0, Qt::lightGray);
    p_Brush->setColorAt(0.5, Qt::black);
    p_Brush->setColorAt(0.99, Qt::lightGray);

    setBrush( *p_Brush );
    setCursor(Qt::PointingHandCursor);

    OutletPort = new SchemaPort(Length, Width/2, this);


    InletPort = new SchemaPort(0, Width/2, this);

    setRotation(Angle);
    setPos(PosX,PosY);


    MaxFlow = 1.0;
    m_Position = 0.0;
}

SchemaValve::~SchemaValve()
{

}
void SchemaValve::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "Valve clicked:" << QString::number(event->pos().x());
    if(event->pos().x() >= m_Length * 0.5 && m_Position <= 0.9){
        m_Position += 0.1 ;
        emit FlowIncreased();
    }
    else if(event->pos().x() < m_Length * 0.5 && m_Position >= 0.1){
        m_Position -= 0.1 ;
        emit FlowDecreased();
    }
    //  K0CTIb/|b
    SchemaFlowmeter *tmp = static_cast<SchemaFlowmeter*>(Descedant);
    tmp->setFlowrate(m_Position * MaxFlow);
    tmp->Floater->update();

    event->accept();
    qDebug() << "Set Flowrate:" << QString::number(tmp->getFlowrateSet());

}
