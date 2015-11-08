#ifndef SCHEMAPIPELINE_H
#define SCHEMAPIPELINE_H
#include "schemacstr.h"

class SchemaPipeline : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    SchemaPipeline(SchemaItem *From, SchemaItem *To);
    ~SchemaPipeline();
private:
    SchemaItem *m_From, *m_To;
    QPainterPath* LinePath;
private slots:
    void drawLine();
};

#endif // SCHEMAPIPELINE_H
