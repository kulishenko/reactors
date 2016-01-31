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
/*!
    \brief Базовый класс для всех графических элементов схемы

*/
class SchemaItem  : public QObject, public QGraphicsPathItem {

    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(qreal x READ x WRITE setX) //!< Положение графического элемента по оси OX
    Q_PROPERTY(qreal y READ y WRITE setY) //!< Положение графического элемента по оси OY
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation) //!< Угол поворота графического элемента
    Q_PROPERTY(int ElementId MEMBER m_ElementId) //!< Идентификатор графического элемента
public:
    SchemaItem();
    virtual ~SchemaItem();
    enum class RunMode {
        Online, //!< Онлайн
        Offline, //!< Оффлайн (воспроизведение записи эксперимента)
        Edit //!< Редактор
    }; //!< Режим работы схемы
    static int s_ElementId; //!< Счетчик элементов схемы
    static RunMode SchemaMode; //!< Режим работы со схемой
    static bool AttachMode; //!< Режим создания связей
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
    int m_ElementId; //!< Идентификатор элемента
    bool m_isActive; //!< Элемент схемы активен
    const QString m_ItemType; //!< Строковое наименование типа графического элемента
    SchemaPort* p_OutletPort; //!< Указатель на порт выхода
    SchemaPort *p_InletPort; //!< Указатель на порт входа
    bool m_isSelected; //!< Элемент схемы выделен пользователем
    qreal m_LineWidth; //!< Ширина линии
protected:
    SchemaItem* p_Descedant; //!< Указатель на дочерний элемент (устарело)
    SchemaScene *p_parent; //!< Указатель на схему
    QPointF _startPos; //!< Начальная позиция курсора при перемещении элемента схемы
    qreal m_Flowrate; //!< Объемный расход вещества через элемент схемы (устарело)
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    bool isActive() const;
    void setActive(bool active);
signals:
    void moved(); //!< Перемещение графического элемента на схеме
    void clicked(int ElementId); //!< Нажатие курсора на графический элемент
public slots:
    void setFlowrate(const qreal Value); //!< Устанавливает объемный расход в безразмерных единицах
    void activate(); //!< Активирует элемент
    void deactivate(); //!< Деактивирует элемент
};

#endif // SCHEMAITEM_H
