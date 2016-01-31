#ifndef SCHEMAFLOWMETER_H
#define SCHEMAFLOWMETER_H
#include <QGraphicsRectItem>
#include <schemaitem.h>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QTimeLine>

/*!
 * \brief Класс, описывающий графический элемент, обозначающий расходомер постоянного перепада
 *
 * Обеспечивается плавное изменение состояний при изменении значений объемного расхода вышестоящим по цепочке сигналов элементом (вентилем)
 */
class SchemaFlowmeter : public SchemaItem
{
   Q_OBJECT
   Q_PROPERTY(qreal Pos MEMBER m_Pos) //!< Относительное положение поплавка
public:
    SchemaFlowmeter(qreal Width = 25.0f, qreal Height = 200.0f, qreal PosX = 0.0f, qreal PosY = 0.0f, qreal Pos = 0.0f, int MaxFlow = 100);
    ~SchemaFlowmeter();
private:
   QGraphicsPolygonItem* Floater;
   QVector<QGraphicsLineItem*> Rulers;
   QVector<QGraphicsTextItem*> Labels;
   QGraphicsLineItem* OutletPipe, *InletPipe;
   qreal m_Height, m_Width, m_FlowrateSet, m_FlowrateStepStart, m_FlowrateStepStop;
   bool isEnabled; //!< Элемент активирован
   int m_MaxFlow; //!< Максимальный объемный расход, л/ч
   int _numScheduledChanges; //!< Число оставшихся изменений положения поплавка
   qreal m_Pos; //!< Относительное положение поплавка
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
   void createAnim();
signals:
   void establishedFlowrate(qreal Value);
public slots:
   void setFlowrate(qreal Value);
   qreal getFlowrateSet();
private slots:
   void animFloater(qreal Value);
   void animFinished();
};

#endif // SCHEMAFLOWMETER_H
