#include "schemaitem.h"
#include <schemacstr.h>
#include <schemaflowmeter.h>
#include <schemavalve.h>

int SchemaItem::s_ElementId = 0;
SchemaItem::RunMode SchemaItem::SchemaMode = SchemaItem::RunMode::Offline;
SchemaItem::SchemaItem() : QObject(), OutletPort(nullptr), InletPort(nullptr),
    Descedant(nullptr), m_ElementId(s_ElementId++), p_parent(nullptr), m_Flowrate(0)
{
    setPen(QPen(Qt::black, 1.5f, Qt::SolidLine, Qt::RoundCap));
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


void SchemaItem::setParent(SchemaScene *scene)
{
    p_parent = scene;
    QObject::setParent(scene);
}


SchemaScene *SchemaItem::parent() const
{
    return p_parent;
}


void SchemaItem::setFlowrate(const qreal Value)
{
    m_Flowrate = Value;
}


QString SchemaItem::getItemType()
{
    // KOCTbI/|b
    QString type;
    if (dynamic_cast<SchemaCSTR*>(this)) type = "SchemaCSTR";
    else if(dynamic_cast<SchemaValve*>(this)) type = "SchemaValve";
    else if(dynamic_cast<SchemaFlowmeter*>(this)) type = "SchemaFlowmeter";
    else type = "SchemaItem";

    return type;
}
