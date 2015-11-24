#ifndef SCHEMAITEM_H
#define SCHEMAITEM_H
#include <QPoint>
#include <schemaport.h>
#include <QObject>
#include <pfdcontrol.h>
#include <QGraphicsItem>

class PFDControl;
class SchemaItem  : public QObject {//, public QGraphicsItem {
    Q_OBJECT
public:
    SchemaItem();

/*
    QRectF boundingRect() const;
    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget);
*/
    SchemaPort* OutletPort, *InletPort;
    SchemaItem* Descedant;
    PFDControl* PFD;
    qreal Flowrate;
    virtual ~SchemaItem();
    enum class RunMode {Online, Offline, Edit};
    static RunMode SchemaMode;
protected:
    QPointF _startPos;
signals:
    void moved();
    void clicked();
};

#endif // SCHEMAITEM_H
