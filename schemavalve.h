#ifndef SCHEMAVALVE_H
#define SCHEMAVALVE_H

#include <QGraphicsPolygonItem>
#include <schemaitem.h>
#include <schemaflowmeter.h>
#include <QObject>

class SchemaValve : public QObject, public QGraphicsPolygonItem, public SchemaItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    SchemaValve(qreal Width, qreal Length, qreal PosX, qreal PosY, qreal Angle);
    ~SchemaValve();
    qreal FlowCoeffitient;
    qreal MaxFlow;
signals:
    void increase();
    void decrease();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCHEMAVALVE_H
