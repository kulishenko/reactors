#include "schemapipeline.h"

SchemaPipeline::SchemaPipeline(SchemaItem* From,SchemaItem* To) : QGraphicsPathItem()
{

    LinePath = new QPainterPath();

    LinePath->moveTo(*(From->OutletPort));
    if((From->OutletPort->x() != To->InletPort->x()) || (From->OutletPort->y() != To->InletPort->y()))
        LinePath->lineTo(To->InletPort->x(),From->OutletPort->y());
    LinePath->lineTo(*(To->InletPort));
    setPath(*LinePath);
}

SchemaPipeline::~SchemaPipeline()
{

}

