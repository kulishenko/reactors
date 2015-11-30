#include "schemavalve.h"
#include <QPen>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QWindow>


//TODO : Remove Length or Width !
SchemaValve::SchemaValve(qreal Width, qreal Length, qreal PosX, qreal PosY, qreal Angle) :
    SchemaItem(), m_Width(Width), m_Length(Length)
{
    QPolygonF polygon( QVector<QPointF>() << QPointF( 0, 0 )
                       << QPointF( 0, Width )
                       << QPointF( 0.75* Width, 0.5* Width )
                       << QPointF( Length, 30 )
                       << QPointF( Length, 0)
                       << QPointF( 0.75* Width,  0.5* Width )
                       << QPointF( 0, 0 )
                       );
    //setPen( QPen(Qt::black) );
    QPainterPath path;
    path.addPolygon(polygon);
    setPath(path);

    p_Brush = new QConicalGradient(0.75* Width, 0.5* Width, 90);
    p_Brush->setColorAt(0.0, Qt::lightGray);
    p_Brush->setColorAt(0.5, Qt::black);
    p_Brush->setColorAt(0.99, Qt::lightGray);

    setBrush( *p_Brush );
    setCursor(Qt::PointingHandCursor);

    OutletPort = new SchemaPort(Length, Width/2, this);


    InletPort = new SchemaPort(0, Width/2, this);

    setRotation(Angle);
    setPos(PosX, PosY);
    setAcceptHoverEvents(true);

    MaxFlow = 1.0f;
    m_Position = 0.0f;
}

SchemaValve::~SchemaValve()
{

}
void SchemaValve::mousePressEvent(QGraphicsSceneMouseEvent *event) {


    if(SchemaMode == RunMode::Edit) {
        setCursor(Qt::DragMoveCursor);
        _startPos = event->pos();
        setOpacity(0.75);
        event->accept();
    } else if(isActive()) {
        qDebug() << "Valve clicked:" << QString::number(event->pos().x());
        if(event->pos().x() >= m_Length * 0.5 && m_Position <= 0.9){
            m_Position += 0.1 ;
            emit FlowIncrease();
        }
        else if(event->pos().x() < m_Length * 0.5 && m_Position >= 0.1){
            m_Position -= 0.1 ;
            emit FlowDecrease();
        }
        //  K0CTIb/|b - MOVE to SIGNAL-SLOT
        //SchemaFlowmeter *tmp = static_cast<SchemaFlowmeter*>(Descedant);
        //tmp->setFlowrate(m_Position * MaxFlow);
        emit FlowrateChanged(m_Position * MaxFlow);
        event->ignore();

    } else event->ignore();
}
void SchemaValve::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    if(SchemaMode == RunMode::Edit) {
        setCursor(Qt::ArrowCursor);
    } else
        setCursor(Qt::PointingHandCursor);

}
