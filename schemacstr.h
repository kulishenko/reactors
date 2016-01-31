#ifndef SCHEMACSTR_H
#define SCHEMACSTR_H

#include <QGraphicsPathItem>
#include <QLinearGradient>
#include <schemaitem.h>
#include <QDebug>
#include <QColor>
#include <modelcstr.h>
#include <schemaevent.h>
/*!
 * \brief Класс, описывающий графический элемент, обозначающий аппарат с мешалкой
 *
 * Обеспечивается плавная анимация изменения уровня жидкости, а также вращения перемешивающего устройства;
 */
class SchemaCSTR : public SchemaItem {
    Q_OBJECT
    Q_PROPERTY(qreal LiquidLevel MEMBER m_LiquidLevel) //!< Относительный уровень жидкости в аппарате
    Q_PROPERTY(qreal numInCascade MEMBER m_numInCascade) // To be removed (see ModelCSTR)
    friend class ModelCSTR;
public:
    SchemaCSTR(int Width = 90, int Height = 120, int xPos = 0, int yPos = 0, qreal StartLevel = 0.1, int Index = 0);
    ~SchemaCSTR();
private:
    QSize m_Size;
    QLinearGradient* p_Gradient;
    QPainterPath* p_path;
    QPainterPath* p_Stirpath;
    qreal m_LiquidLevel;
    qreal m_LiquidLevelSet;
//    qreal* p_tau;
    QGraphicsEllipseItem* p_Motor;
    QGraphicsSimpleTextItem* p_MotorLabel;
    QGraphicsPathItem* p_Stir;
    QGraphicsPolygonItem* p_Mixer;
    QGraphicsRectItem* p_Ring1;
    QGraphicsRectItem* p_Ring2;
    QColor m_LiquidBottomColor;
    QColor m_LiquidTopColor;
    QColor m_GasColor;
    int m_numInCascade;
    bool m_isWorking; //!< Аппарат работает
    bool m_isFeeding; //!< Аппарат имеет ненулевой расход выходного потока
    bool m_isReady; //!< Аппарат полностью заполнен
    qreal m_MixerAngle; //!< Текущий угол поворота перемешивающего устройства относительно плоскости экрана
    ModelCSTR* p_Model; //!< Указатель на модель, описывающую логику работы элемента
signals:
    void test();
    void clicked(); //!< Генерируется при щелчке на элементе
    void filled(); //!< Элемент заполнен
    void startedFeed(); //!< Уровень жидкости достиг точки перелива
public slots:
    void changeLevel();
    void fill(); //!< Начать заполнение
    void activateMotor(); //!< Активировать перемешивающее устройство
    void startFeed();
    void setFlowrate(const qreal Value); //!< Установить объемный расход
private slots:
    void setLevel(const qreal Level, const int TransTime); //!< Запустить анимированное изменение уровня жидкости
    void animLevel(const qreal Value); //!< Шаг анимации уровня
    void animMotor(const qreal Value); //!< Шаг анимации вращения перемешивающего устройства
    void animFinished(); //!< Заглушка для завершения анимации
};


#endif // SCHEMACSTR_H
