#ifndef SCHEMAPIPELINE_H
#define SCHEMAPIPELINE_H
#include "schemacstr.h"
#include <QGraphicsScene>

/*!
 * \brief Класс, описывающий соединение элементов графической схемы
 */
class SchemaPipeline : public SchemaItem {
    Q_OBJECT
    Q_PROPERTY(int FromElementId MEMBER m_FromElementId WRITE setFromElementId) //!< Начальный графический элемент схемы
    Q_PROPERTY(int ToElementId MEMBER m_ToElementId WRITE setToElementId) //!< Конечный графический элемент схемы
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
