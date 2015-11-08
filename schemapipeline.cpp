#include "schemapipeline.h"

SchemaPipeline::SchemaPipeline(SchemaItem* From, SchemaItem* To) :
    QGraphicsPathItem(), m_From(From), m_To(To), LinePath(Q_NULLPTR)
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
        LinePath->lineTo(ToPos.x(),FromPos.y());
    LinePath->lineTo(ToPos);
    setPath(*LinePath);

    if(LinePath != NULL) delete OldPath;
}

