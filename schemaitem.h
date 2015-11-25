#ifndef SCHEMAITEM_H
#define SCHEMAITEM_H
#include <QPoint>
#include <schemaport.h>
#include <QObject>
#include <pfdcontrol.h>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>

class PFDControl;
class SchemaItem  : public QGraphicsObject {//, public QGraphicsItem {
    friend class SchemaFlowmeter;
    friend class SchemaCell;
    friend class SchemaValve;
    friend class SchemaStream;
    friend class SchemaCSTR;
    Q_OBJECT
    Q_PROPERTY(int ElementId MEMBER m_ElementId)
public:
    SchemaItem();
    SchemaPort* OutletPort, *InletPort;
    SchemaItem* Descedant;
    PFDControl* PFD;
    qreal Flowrate;
    virtual ~SchemaItem();
    enum class RunMode {Online, Offline, Edit};
    static int s_ElementId;
    static RunMode SchemaMode;
private:
    int m_ElementId;
    qreal m_PosX, m_PosY; // Scene coordinates
protected:
    QPointF _startPos;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void moved();
    void clicked();
};

#endif // SCHEMAITEM_H
