#include "schemacell.h"
#include <QPen>

SchemaCell::SchemaCell(  qreal Width, qreal Height, qreal PosX, qreal PosY, qreal Angle) : SchemaItem()
{

    QPolygonF polygon( QVector<QPointF>()
                          << QPointF( 0, 0 )
                          << QPointF( Width, 0 )
                          << QPointF( Width, Height )
                          << QPointF( 0.9 * Width, Height )
                          << QPointF( 0.9 * Width, Height * 1.3 )
                          << QPointF( 0.7 * Width, Height * 1.3 )
                          << QPointF( 0.7 * Width, Height )
                          << QPointF( 0.3 * Width, Height )
                          << QPointF( 0.3 * Width, Height * 1.3 )
                          << QPointF( 0.1 * Width, Height * 1.3 )
                          << QPointF( 0.1 * Width, Height )
                          << QPointF( 0, Height )
                          << QPointF( 0, 0 )
                          );
    QPainterPath path;
    path.addPolygon(polygon);

    setPath(path);

//    setPen( QPen(Qt::black) );
    setBrush( Qt::white );

    setOutletPort(new SchemaPort(0, Height * 0.5, this, -90));
    setInletPort(new SchemaPort(Width, Height * 0.5, this, -90));

    p_Electrode = new QGraphicsRectItem(Width * 0.775, Height * 0.2, Width * 0.05, Height * 1.3, this);
    p_Electrode->setBrush(Qt::black);


    // Drawing the Button
    p_Button = new QGraphicsEllipseItem(this);
    int MotorRadius = 40;
    p_Button->setRect(QRect(-Width/2+MotorRadius/2, -80, -MotorRadius, -MotorRadius));
    p_Button->setRotation(180);
    p_Button->setBrush(Qt::white);

    p_ButtonLabel = new QGraphicsSimpleTextItem("S", p_Button);
    p_ButtonLabel->setPos(p_Button->boundingRect().topLeft());
    p_ButtonLabel->moveBy(p_ButtonLabel->boundingRect().width()*2, 0);
    QFont ButtonLabelFont("Calibri", 22, QFont::Bold);
    p_ButtonLabel->setFont(ButtonLabelFont);

    setRotation(Angle);
    setPos(PosX, PosY);

}

SchemaCell::~SchemaCell()
{

}
