#ifndef SCHEMAPIPELINE_H
#define SCHEMAPIPELINE_H
#include "schemacstr.h"
#include <QGraphicsScene>

class SchemaPipeline : public SchemaItem {
    Q_OBJECT
    Q_PROPERTY(int FromElementId MEMBER m_FromElementId)
    Q_PROPERTY(int ToElementId MEMBER m_ToElementId)
public:
    SchemaPipeline(SchemaItem *From, SchemaItem *To);
    ~SchemaPipeline();
private:
    SchemaItem *m_From, *m_To;
    QPainterPath* LinePath;
    int m_FromElementId, m_ToElementId;
private slots:
    void drawLine();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCHEMAPIPELINE_H
