#include "schemastream.h"
#include <QPen>

SchemaStream::SchemaStream(qreal Size, qreal PosX, qreal PosY, qreal Angle) : SchemaItem(), QGraphicsPolygonItem()
{
    m_PosX = PosX;
    m_PosY = PosY;
    setPolygon(QPolygonF( QVector<QPointF>() << QPointF( 0, 0 )  << QPointF( 0, Size ) << QPointF( 0.75* Size , 0.5* Size  ) << QPointF(0,0)));
    setPen( QPen(Qt::black) );
    setBrush( Qt::black );

    SchemaItem* p_this = static_cast<SchemaItem*> (this);

    OutletPort = new SchemaPort(Size*0.75,Size/2, p_this);

    InletPort = new SchemaPort(0,Size/2, p_this);
    SchemaItem::setRotation(Angle);
    SchemaItem::setPos(PosX, PosY);

}

SchemaStream::~SchemaStream()
{

}

QRectF SchemaStream::boundingRect() const
{
    return QGraphicsPolygonItem::boundingRect();
}

void SchemaStream::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPolygonItem::paint(painter, option, widget);
}
