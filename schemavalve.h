#ifndef SCHEMAVALVE_H
#define SCHEMAVALVE_H

#include <QGraphicsPolygonItem>
#include <schemaitem.h>
#include <schemaflowmeter.h>
#include <QObject>
#include <QConicalGradient>
#include <QtMath>
#include <QGraphicsScene>

class SchemaValve : public SchemaItem, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_PROPERTY(qreal Rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal PosX MEMBER m_PosX WRITE setPosX)
    Q_PROPERTY(qreal PoxY MEMBER m_PosY WRITE setPosY)
    Q_INTERFACES(QGraphicsItem)
public:
    SchemaValve(qreal Width = 30.0f, qreal Length = 45.0f, qreal PosX = 122.5f, qreal PosY = 350.0f, qreal Angle = -90);
    ~SchemaValve();
    qreal m_Position;
    qreal MaxFlow;
private:
    const qreal m_Width, m_Length;
    qreal m_PosX, m_PosY;
    QConicalGradient* p_Brush;
signals:
    void FlowIncreased();
    void FlowDecreased();
private slots:
    void setPosX(qreal Value);
    void setPosY(qreal Value);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
};

#endif // SCHEMAVALVE_H
