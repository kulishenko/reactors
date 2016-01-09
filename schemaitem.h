#ifndef SCHEMAITEM_H
#define SCHEMAITEM_H
#include <QPoint>
#include <schemaport.h>
#include <schemascene.h>
#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QCursor>

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
    virtual ~SchemaItem();
    enum class RunMode {Online, Offline, Edit};
    static int s_ElementId;
    static RunMode SchemaMode;
    static bool AttachMode;
    void setParent(SchemaScene* scene);
    SchemaScene* parent() const;
    QString getItemType();
    void setDescedant(SchemaItem *item);
    SchemaItem *getDescedant();
    SchemaPort *getInletPort();
    SchemaPort *getOutletPort();
    void setInletPort(SchemaPort* port);
    void setOutletPort(SchemaPort* port);
    void setSelected(bool selected);
private:
    int m_ElementId;
    bool m_isActive;
    const QString m_ItemType;
    SchemaPort* p_OutletPort, *p_InletPort;
    bool m_isSelected;
    qreal m_LineWidth;
protected:
    SchemaItem* p_Descedant;
    SchemaScene *p_parent;
    QPointF _startPos;
    qreal m_Flowrate;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    bool isActive() const;
    void setActive(bool active);
signals:
    void moved();
    void clicked(int ElementId);
public slots:
    void setFlowrate(const qreal Value);
    void activate();
    void deactivate();
};

#endif // SCHEMAITEM_H
