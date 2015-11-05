#include "schemavalve.h"
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QWindow>


//TODO : Remove Length or Width !
SchemaValve::SchemaValve(qreal Width, qreal Length, qreal PosX, qreal PosY, qreal Angle)
{
    setPolygon(QPolygonF( QVector<QPointF>() << QPointF( 0, 0 )  << QPointF( 0, Width ) << QPointF( 0.75* Width, 0.5* Width ) << QPointF(Length,30)  << QPointF(Length,0) << QPointF( 0.75* Width,  0.5* Width )));
    setPen( QPen(Qt::black) );
    setBrush( Qt::gray );

    setRotation(Angle);
    setPos(PosX,PosY);

    OutletPort = new SchemaPort(Length,Width/2, this);
    OutletPort->setPos(PosX,PosY);
    OutletPort->setRotation(Angle);

    InletPort = new SchemaPort(0,Width/2, this);
    InletPort->setPos(PosX,PosY);
    InletPort->setRotation(Angle);

    MaxFlow=1.0;
    m_Position=0.0;
}

SchemaValve::~SchemaValve()
{

}
void SchemaValve::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "Valve clicked:" << QString::number(event->pos().x());
    if(event->pos().x() >= 22 && m_Position <= 0.9){
        m_Position += 0.1 ;
        emit increase();
    }
    else if(event->pos().x() < 22 && m_Position >= 0.1){
        m_Position -= 0.1 ;
        emit decrease();
    }
    //  K0CTIb/|b
    SchemaFlowmeter *tmp = static_cast<SchemaFlowmeter*>(Descedant);
    tmp->setFlowrate(m_Position*MaxFlow);
    tmp->Floater->update();

    event->accept();
    qDebug() << "Set Flowrate:" << QString::number(tmp->Flowrateset);

}
