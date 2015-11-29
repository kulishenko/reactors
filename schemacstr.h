#ifndef SCHEMACSTR_H
#define SCHEMACSTR_H

#include <QGraphicsPathItem>
#include <QLinearGradient>
#include <schemaitem.h>
#include <QDebug>
#include <QColor>
#include <modelcstr.h>
#include <schemaevent.h>

class SchemaCSTR : public SchemaItem {
    Q_OBJECT
    Q_PROPERTY(qreal LiquidLevel MEMBER m_LiquidLevel)
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
    ModelCSTR* p_Model;
signals:
    void test();
    void clicked();
    void filled();
    void startedFeed();
public slots:
    void changeLevel();
    void fill();
    void activateMotor();
    void startFeed();
    void setFlowrate(const qreal Value);
private slots:
    void setLevel(const qreal Level, const int TransTime);
    void animLevel(const qreal Value);
    void animMotor(const qreal Value);
    void animFinished();
};


#endif // SCHEMACSTR_H
