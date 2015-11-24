#ifndef SCHEMASTREAM_H
#define SCHEMASTREAM_H
#include <QGraphicsPolygonItem>
#include <schemaitem.h>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>

class SchemaStream : public SchemaItem, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_PROPERTY(qreal Rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal PosX MEMBER m_PosX WRITE setPosX)
    Q_PROPERTY(qreal PoxY MEMBER m_PosY WRITE setPosY)
    Q_INTERFACES(QGraphicsItem)
public:
    SchemaStream(qreal Size, qreal PosX, qreal PosY, qreal Angle);
    virtual ~SchemaStream();
private:
    qreal m_PosX, m_PosY;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private slots:
    void setPosX(const int Value);
    void setPosY(const int Value);
};

#endif // SCHEMASTREAM_H
