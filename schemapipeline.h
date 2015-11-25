#ifndef SCHEMAPIPELINE_H
#define SCHEMAPIPELINE_H
#include "schemacstr.h"
#include <QGraphicsScene>

class SchemaPipeline : public SchemaItem, public QGraphicsPathItem
{
    Q_OBJECT
    Q_PROPERTY(int FromElementId MEMBER m_FromElementId)
    Q_PROPERTY(int ToElementId MEMBER m_ToElementId)
public:
    SchemaPipeline(SchemaItem *From, SchemaItem *To);
    ~SchemaPipeline();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    SchemaItem *m_From, *m_To;
    QPainterPath* LinePath;
    int m_FromElementId, m_ToElementId;
private slots:
    void drawLine();
};

#endif // SCHEMAPIPELINE_H
