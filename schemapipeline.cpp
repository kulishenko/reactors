#include "schemapipeline.h"

SchemaPipeline::SchemaPipeline(SchemaItem* From, SchemaItem* To) :
    SchemaItem(), QGraphicsPathItem(), m_From(From), m_To(To), LinePath(nullptr),
    m_FromElementId(From->property("ElementId").toInt()),
    m_ToElementId(To->property("ElementId").toInt())
{
    drawLine();

    // Pointer to the next item in schema
    From->Descedant = To;
    connect(From, SIGNAL(moved()), this, SLOT(drawLine()));
    connect(To, SIGNAL(moved()), this, SLOT(drawLine()));
}

SchemaPipeline::~SchemaPipeline()
{

}

void SchemaPipeline::drawLine()
{
    QPainterPath* OldPath;
    OldPath = LinePath;
    LinePath = new QPainterPath();

    QPointF FromPos = m_From->OutletPort->scenePos();
    QPointF ToPos = m_To->InletPort->scenePos();

//    qDebug() << "From: "+QString::number(m_From->OutletPort->getAngle());
//    qDebug() << "To: "+QString::number(m_To->InletPort->getAngle());

    LinePath->moveTo(FromPos);
    if((FromPos.x() != ToPos.x()) || (FromPos.y() != ToPos.y()))
    {
        if(fabs(m_From->OutletPort->getAngle() - m_To->InletPort->getAngle()) > 1e-2)
            LinePath->lineTo(ToPos.x(), FromPos.y());
        else if(fmod(m_From->OutletPort->getAngle(), 180) < 1e-2)
        {
            LinePath->lineTo((ToPos.x() + FromPos.x())/2,  FromPos.y());
            LinePath->lineTo((ToPos.x() + FromPos.x())/2,  ToPos.y());
        }
        else // if()
        {
            LinePath->lineTo(FromPos.x(),(ToPos.y() + FromPos.y())/2);
            LinePath->lineTo(ToPos.x(), (ToPos.y() + FromPos.y())/2);
        }
    }
    LinePath->lineTo(ToPos);
    setPath(*LinePath);

    // K0CTbI/\b
    QGraphicsScene *scn = SchemaItem::scene();
    if(scn) scn->update(scn->sceneRect());
    if(LinePath) delete OldPath;
}

void SchemaPipeline::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

void SchemaPipeline::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

void SchemaPipeline::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

QRectF SchemaPipeline::boundingRect() const
{
    return QGraphicsPathItem::boundingRect();
}

void SchemaPipeline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPathItem::paint(painter, option, widget);
}
