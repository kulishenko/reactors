#ifndef SCHEMACSTR_H
#define SCHEMACSTR_H

#include <QGraphicsPathItem>
#include <QLinearGradient>
#include <schemaitem.h>
#include <QDebug>
#include <QColor>
#include <modelcstr.h>
#include <schemaevent.h>

class SchemaCSTR : public SchemaItem, public QGraphicsPathItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(int PosX MEMBER m_PosX WRITE setPosX)
    Q_PROPERTY(int PosY MEMBER m_PosY WRITE setPosY)
    Q_PROPERTY(qreal LiquidLevel MEMBER m_LiquidLevel)
    Q_PROPERTY(qreal numInCascade MEMBER m_numInCascade) // To be removed (see ModelCSTR)
    friend class ModelCSTR;
public:
    SchemaCSTR(int Width = 90, int Height = 120, int xPos = 0, int yPos = 0, qreal StartLevel = 0.1, int Index = 0);
    ~SchemaCSTR();
private:
    QSize m_Size;
    int m_PosX, m_PosY;
    QLinearGradient* p_Gradient;
    QPainterPath* p_path;
    QPainterPath* p_Stirpath;
    qreal m_LiquidLevel;
    qreal m_LiquidLevelSet;
    qreal* p_tau;
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
    bool m_isWorking;
    bool m_isFeeding;
    bool m_isReady;
    qreal m_MixerAngle;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
    void test();
    void clicked();
public slots:
    void changeLevel();
    void fill();
    void activateMotor();
    void startFeed();
private slots:
    void setLevel(const qreal Level, const int TransTime);
    void animLevel(const qreal Value);
    void animMotor(const qreal Value);
    void animFinished();
    void setPosX(const int Value);
    void setPosY(const int Value);
};


#endif // SCHEMACSTR_H
