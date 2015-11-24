#include "schemastream.h"
#include <QPen>

SchemaStream::SchemaStream(qreal Size, qreal PosX, qreal PosY, qreal Angle) : SchemaItem()
{
    setPolygon(QPolygonF( QVector<QPointF>() << QPointF( 0, 0 )  << QPointF( 0, Size ) << QPointF( 0.75* Size , 0.5* Size  ) << QPointF(0,0)));
    setPen( QPen(Qt::black) );
    setBrush( Qt::black );

    OutletPort = new SchemaPort(Size*0.75,Size/2, this);

    InletPort = new SchemaPort(0,Size/2, this);
    setRotation(Angle);
    setPos(PosX, PosY);

}

SchemaStream::~SchemaStream()
{

}

void SchemaStream::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //ToDo: Do it more simple...
    QPointF Pos =  event->pos() - _startPos;
    qreal AngleRad = qDegreesToRadians(rotation());

    qreal s = qSin(AngleRad);
    qreal c = qCos(AngleRad);

    qreal py = Pos.y();
    qreal px = Pos.x();

    moveBy(c * px - s * py, s * px + c * py);
    emit moved();
}
void SchemaStream::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    if(SchemaMode == RunMode::Edit) {
        setCursor(Qt::DragMoveCursor);
        _startPos = event->pos();
        setOpacity(0.75);
    } else
        event->ignore();

}

void SchemaStream::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    setCursor(Qt::ArrowCursor);
    setOpacity(1);
}
