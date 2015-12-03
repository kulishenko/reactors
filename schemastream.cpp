#include "schemastream.h"
#include <QPen>

SchemaStream::SchemaStream(qreal Size, qreal PosX, qreal PosY, qreal Angle) : SchemaItem()
{

    QPolygonF polygon( QVector<QPointF>() << QPointF( 0, 0 )  << QPointF( 0, Size ) << QPointF( 0.75* Size , 0.5* Size  ) << QPointF(0,0));
    QPainterPath path;
    path.addPolygon(polygon);

    setPath(path);

    setPen( QPen(Qt::black) );
    setBrush( Qt::black );

    setOutletPort(new SchemaPort(Size*0.75, Size/2, this));
    setInletPort(new SchemaPort(0, Size/2, this));

    setRotation(Angle);
    setPos(PosX, PosY);

}

SchemaStream::~SchemaStream()
{

}
