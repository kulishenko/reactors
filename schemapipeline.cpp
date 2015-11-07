#include "schemapipeline.h"

SchemaPipeline::SchemaPipeline(SchemaItem* From, SchemaItem* To) : QGraphicsPathItem()
{

    LinePath = new QPainterPath();

    QPointF FromPos = From->OutletPort->scenePos();
    QPointF ToPos = To->InletPort->scenePos();

    LinePath->moveTo(FromPos);
    if((FromPos.x() != ToPos.x()) || (FromPos.y() != ToPos.y()))
        LinePath->lineTo(ToPos.x(),FromPos.y());
    LinePath->lineTo(ToPos);
    setPath(*LinePath);
    // Pointer to the next item in schema
    From->Descedant = To;
}

SchemaPipeline::~SchemaPipeline()
{

}

