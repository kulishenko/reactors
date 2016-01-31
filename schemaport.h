#ifndef SCHEMAPORT_H
#define SCHEMAPORT_H
#define _USE_MATH_DEFINES
#include <QPointF>
#include <QGraphicsItem>
#include <QtMath>
#include <QPen>

/*!
 * \brief Класс, описывающий порты графических элементов схемы
 */
class SchemaPort : public  QGraphicsLineItem //QPointF
{
public:
    SchemaPort(qreal PosX, qreal PosY, QGraphicsItem* parentItem, qreal Angle = 90);
    ~SchemaPort();
//    void setRotation(qreal Angle);
//    void setPos(qreal dx, qreal dy);
//    QGraphicsItem* parent;
    QPointF scenePos() const; //!< Выводит преобразованные координаты порта в сцене
    qreal getAngle() const; //!< Выводит угол порта в системе координат сцены
};

#endif // SCHEMAPORT_H
