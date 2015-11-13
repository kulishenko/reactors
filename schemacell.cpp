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
    OutletPort = new SchemaPort(0, Height * 0.5, this, -90);
    InletPort = new SchemaPort(Width, Height * 0.5, this, -90);

    p_Electrode = new QGraphicsRectItem(Width * 0.775, Height * 0.2, Width*0.05, Height * 1.3, this);
    p_Electrode->setBrush(Qt::black);

    setRotation(Angle);
    setPos(PosX,PosY);

}

SchemaCell::~SchemaCell()
{

}

