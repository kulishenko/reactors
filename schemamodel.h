#ifndef SCHEMAMODEL_H
#define SCHEMAMODEL_H
#include <schemadata.h>

class SchemaModel
{
public:
    SchemaModel(SchemaData* Data = 0);
protected:
    SchemaData* p_Data;
};

#endif // SCHEMAMODEL_H
