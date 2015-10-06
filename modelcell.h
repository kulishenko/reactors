#ifndef MODELCELL_H
#define MODELCELL_H
#include <schemadata.h>
#include <gsl/gsl_math.h>
#include <QDebug>

class ModelCell
{
public:
    ModelCell();
    ~ModelCell();
    SchemaData* Data;
};

#endif // MODELCELL_H
