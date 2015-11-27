#ifndef SCHEMAITEM_H
#define SCHEMAITEM_H
#include <QPoint>
#include <schemaport.h>
#include <schemascene.h>
#include <QObject>
#include <pfdcontrol.h>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>

class PFDControl;
class SchemaScene;
class SchemaItem  : public QObject, public QGraphicsPathItem {

    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
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
    void setParent(SchemaScene* scene);
    SchemaScene* parent() const;
private:
    int m_ElementId;
//    qreal m_PosX, m_PosY; // Scene coordinates
protected:
    SchemaScene *p_parent;
    QPointF _startPos;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void moved();
    void clicked();
};

#endif // SCHEMAITEM_H
