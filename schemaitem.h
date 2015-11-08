#ifndef SCHEMAITEM_H
#define SCHEMAITEM_H
#include <QPoint>
#include <schemaport.h>
#include <QObject>
#include <pfdcontrol.h>
#include <QGraphicsSceneMouseEvent>

class PFDControl;
class SchemaItem  : public QObject
{
    Q_OBJECT
public:
    SchemaItem();
    SchemaPort* OutletPort, *InletPort;
    SchemaItem* Descedant;
    PFDControl* PFD;
    qreal Flowrate;
    ~SchemaItem();
    QPointF _startPos;
signals:
    void moved();
    void clicked();
};

#endif // SCHEMAITEM_H
