#include "schemapipeline.h"

SchemaPipeline::SchemaPipeline(SchemaItem* From, SchemaItem* To) :
    QGraphicsPathItem(), m_From(From), m_To(To), LinePath(nullptr)
{
    drawLine();
    // Pointer to the next item in schema
    From->Descedant = To;
    connect(From,SIGNAL(moved()),this,SLOT(drawLine()));
    connect(To,SIGNAL(moved()),this,SLOT(drawLine()));
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

    LinePath->moveTo(FromPos);
    if((FromPos.x() != ToPos.x()) || (FromPos.y() != ToPos.y()))
        if(fabs(m_From->OutletPort->getAngle() - m_To->InletPort->getAngle()) > 1e-2)
            LinePath->lineTo(ToPos.x(), FromPos.y());
        else
        {
            LinePath->lineTo((ToPos.x() + FromPos.x())/2,  FromPos.y());
            LinePath->lineTo((ToPos.x() + FromPos.x())/2,  ToPos.y());
        }

    LinePath->lineTo(ToPos);
    setPath(*LinePath);

    if(LinePath != NULL) delete OldPath;
}

