#ifndef SCHEMACELL_H
#define SCHEMACELL_H
#include <QGraphicsPolygonItem>
#include <QGraphicsRectItem>
#include <schemaitem.h>

class SchemaCell : public SchemaItem, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_PROPERTY(qreal Rotation READ rotation WRITE setRotation)
    Q_PROPERTY(qreal PosX MEMBER m_PosX WRITE setPosX)
    Q_PROPERTY(qreal PoxY MEMBER m_PosY WRITE setPosY)
public:
    SchemaCell(qreal Width = 100.0f, qreal Height = 40.0f, qreal PosX = 1255.0f, qreal PosY = 550.0f, qreal Angle = 180.0f);
    ~SchemaCell();
private slots:
    void setPosX(const int Value);
    void setPosY(const int Value);
private:
    QGraphicsRectItem* p_Electrode;
    qreal m_PosX, m_PosY;
};

#endif // SCHEMACELL_H
