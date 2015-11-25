#include "schemaitem.h"
int SchemaItem::s_ElementId = 0;
SchemaItem::RunMode SchemaItem::SchemaMode = SchemaItem::RunMode::Offline;
SchemaItem::SchemaItem() : QObject(), PFD(nullptr), m_ElementId(s_ElementId++)
{
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap));
}

SchemaItem::~SchemaItem()
{

}

void SchemaItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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
void SchemaItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    setCursor(Qt::ArrowCursor);
    setOpacity(1);
}
void SchemaItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    if(SchemaMode == RunMode::Edit) {
        setCursor(Qt::DragMoveCursor);
        _startPos = event->pos();
        setOpacity(0.75);
    } else
        event->ignore();

}
