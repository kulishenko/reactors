#ifndef SCHEMAPFR_H
#define SCHEMAPFR_H
#include <schemaitem.h>
#include <QPainterPath>

class SchemaPFR : public SchemaItem
{
    Q_OBJECT
public:
    SchemaPFR(qreal Width = 50.0f, qreal Height = 400.0f, QObject* parent = 0);
    ~SchemaPFR();
private:
    QPainterPath* p_Path;
};

#endif // SCHEMAPFR_H
