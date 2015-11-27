#include "schemapipeline.h"

SchemaPipeline::SchemaPipeline() : SchemaItem(),
    m_From(nullptr), m_To(nullptr), LinePath(nullptr),
    m_FromElementId(0), m_ToElementId(0)
{

}

SchemaPipeline::SchemaPipeline(SchemaItem* From, SchemaItem* To) :
    SchemaItem(), m_From(From), m_To(To), LinePath(nullptr),
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

void SchemaPipeline::setFromElementId(int Value)
{
    m_FromElementId = Value;
    SchemaItem* From = parent()->getItemByElementId(Value);
    if(From) {
        connect(From, SIGNAL(moved()), this, SLOT(drawLine()));
        m_From = From;
    }

    if(m_From && m_To) drawLine();
}

void SchemaPipeline::setToElementId(int Value)
{
    m_ToElementId = Value;
    SchemaItem* To = parent()->getItemByElementId(Value);
    if(To) {
        connect(To, SIGNAL(moved()), this, SLOT(drawLine()));
        m_To = To;
    }

    if(m_From && m_To) drawLine();
}

void SchemaPipeline::drawLine()
{
    if(!m_From->Descedant) m_From->Descedant = m_To; // Remove (make via SIGNAL-SLOT)

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
