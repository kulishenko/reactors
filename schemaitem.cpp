#include "schemaitem.h"
#include <schemacstr.h>
#include <schemaflowmeter.h>
#include <schemavalve.h>
#include <schemapipeline.h>
#include <schemapfr.h>
#include <schemastream.h>

int SchemaItem::s_ElementId = 0;
bool SchemaItem::AttachMode = false;
SchemaItem::RunMode SchemaItem::SchemaMode = SchemaItem::RunMode::Offline;
SchemaItem::SchemaItem() : QObject(), m_ElementId(s_ElementId++),
    m_isActive(false), m_ItemType(getItemType()),
    p_OutletPort(nullptr), p_InletPort(nullptr),
    m_LineWidth(1.5f), p_Descedant(nullptr), p_parent(nullptr), m_Flowrate(0)
{
    setPen(QPen(Qt::black, m_LineWidth, Qt::SolidLine, Qt::RoundCap));
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

    emit clicked(m_ElementId);
    if(SchemaMode == RunMode::Edit) {
        if(AttachMode) ;

        else
            setCursor(Qt::DragMoveCursor);
            _startPos = event->pos();
            setOpacity(0.75);
    } else
        event->ignore();

}


void SchemaItem::setParent(SchemaScene *scene)
{
    /*!
    Устанавливает родительский объект
    \param[in] scene Указатель на родительский объект
    */
    p_parent = scene;
    QObject::setParent(scene);
}


SchemaScene *SchemaItem::parent() const
{
    return p_parent;
}


void SchemaItem::setFlowrate(const qreal Value)
{
    /*!
    Устанавливает объемный расход для элемента схемы
    \param[in] Value Значение относительного объемного расхода
    */
    m_Flowrate = Value;
}


QString SchemaItem::getItemType()
{
    /*! \return Строковое значение имени графического элемента схемы */
    // KOCTbI/|b
    QString type;
    if (dynamic_cast<SchemaCSTR*>(this)) type = "SchemaCSTR";
    else if(dynamic_cast<SchemaPFR*>(this)) type = "SchemaPFR";
    else if(dynamic_cast<SchemaPipeline*>(this)) type = "SchemaPipeline";
    else if(dynamic_cast<SchemaValve*>(this)) type = "SchemaValve";
    else if(dynamic_cast<SchemaFlowmeter*>(this)) type = "SchemaFlowmeter";
    else if(dynamic_cast<SchemaStream*>(this)) type = "SchemaStream";
    else type = "SchemaItem";

    return type;
}

bool SchemaItem::isActive() const
{
    /*! \return Активен ли элемент */
    return m_isActive;
}

void SchemaItem::setActive(bool active)
{
    m_isActive = active;
    QGraphicsPathItem::setActive(active);
}

void SchemaItem::setSelected(bool selected)
{
    if(selected)
        setPen(QPen(Qt::red, m_LineWidth, Qt::SolidLine, Qt::RoundCap));
    else
        setPen(QPen(Qt::black, m_LineWidth, Qt::SolidLine, Qt::RoundCap));

    QGraphicsPathItem::setSelected(selected);
}


void SchemaItem::activate()
{
    setActive(true);
}


void SchemaItem::deactivate()
{
    m_isActive = false;
}


void SchemaItem::setDescedant(SchemaItem *item)
{
    p_Descedant = item;
}


SchemaItem *SchemaItem::getDescedant()
{
    return p_Descedant;
}

SchemaPort *SchemaItem::getInletPort()
{
    return p_InletPort;
}

SchemaPort *SchemaItem::getOutletPort()
{
    return p_OutletPort;
}


void SchemaItem::setInletPort(SchemaPort *port)
{
    p_InletPort = port;
}

void SchemaItem::setOutletPort(SchemaPort *port)
{
    p_OutletPort = port;
}
