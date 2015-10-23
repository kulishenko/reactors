#ifndef SCHEMAPIPELINE_H
#define SCHEMAPIPELINE_H
#include "schemacstr.h"

class SchemaPipeline : public QGraphicsPathItem
{
public:
    SchemaPipeline(SchemaItem *From, SchemaItem *To);
    QPainterPath* LinePath;
    ~SchemaPipeline();
};

#endif // SCHEMAPIPELINE_H
