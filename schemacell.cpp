#include "schemacell.h"
#include <QPen>

SchemaCell::SchemaCell(  qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Angle) :
    SchemaItem(), QGraphicsPolygonItem()
{
    setPolygon(QPolygonF( QVector<QPointF>()
                          << QPointF( 0, 0 )
                          << QPointF( Width, 0 )
                          << QPointF(  Width, Height)
                          << QPointF(  0.9 *Width, Height)
                          << QPointF(  0.9 *Width, Height *1.3)
                          << QPointF(  0.7 *Width, Height *1.3)
                          << QPointF(  0.7 *Width, Height)

                          << QPointF(  0.3 *Width, Height)
                          << QPointF(  0.3 *Width, Height *1.3)
                          << QPointF(  0.1 *Width, Height *1.3)
                          << QPointF(  0.1 *Width, Height)
                          << QPointF( 0, Height)

                          ));
    setPen( QPen(Qt::black) );
    setBrush( Qt::white );
    OutletPort = new SchemaPort(0,Height*0.5,this, -90);
    InletPort = new SchemaPort(Width,Height*0.5,this, -90);

    setRotation(Angle);
    setPos(PosX,PosY);

}

SchemaCell::~SchemaCell()
{

}

