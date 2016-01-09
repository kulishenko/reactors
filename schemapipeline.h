#ifndef SCHEMAPIPELINE_H
#define SCHEMAPIPELINE_H
#include "schemacstr.h"
#include <QGraphicsScene>

class SchemaPipeline : public SchemaItem {
    Q_OBJECT
    Q_PROPERTY(int FromElementId MEMBER m_FromElementId WRITE setFromElementId)
    Q_PROPERTY(int ToElementId MEMBER m_ToElementId WRITE setToElementId)
public:
    SchemaPipeline();
    SchemaPipeline(SchemaItem *From, SchemaItem *To);
    ~SchemaPipeline();
private:
    SchemaItem *m_From, *m_To;
    QPainterPath* LinePath;
    QGraphicsPolygonItem* p_Arrow;
    int m_FromElementId, m_ToElementId;
    void setFromElementId(int Value);
    void setToElementId(int Value);
private slots:
    void drawLine();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCHEMAPIPELINE_H
