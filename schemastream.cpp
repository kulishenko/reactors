#include "schemastream.h"
#include <QPen>

SchemaStream::SchemaStream(qreal Size, qreal PosX, qreal PosY, qreal Angle)
{
    setPolygon(QPolygonF( QVector<QPointF>() << QPointF( 0, 0 )  << QPointF( 0, Size ) << QPointF( 0.75* Size , 0.5* Size  ) << QPointF(0,0)));
    setPen( QPen(Qt::black) );
    setBrush( Qt::black );

    setRotation(Angle);
    setPos(PosX,PosY);

    OutletPort = new SchemaPort(Size*0.75,Size/2, this);
    OutletPort->setPos(PosX,PosY);
    OutletPort->setRotation(Angle);

    InletPort = new SchemaPort(0,Size/2, this);
    InletPort->setPos(PosX,PosY);
    InletPort->setRotation(Angle);

}

SchemaStream::~SchemaStream()
{

}

