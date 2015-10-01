#ifndef PFDCONTROL_H
#define PFDCONTROL_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QGraphicsPathItem>
#include <schemaitem.h>
#include <QVector>
#include <QFile>
#include <QApplication>

class PFDControl : public QObject
{
    Q_OBJECT

public:
    explicit PFDControl(QObject *parent = 0);
    ~PFDControl();
    qreal Flowrate;
    QVector<SchemaItem> PFDItems;
    QVector<qreal> Time;
    QVector<qreal> Conductivity;
    QVector<qreal> Tau;
    qreal TimeNow;
    bool isStarted;
signals:
    void setLevel();
    void doSim();
    void startSim();
public slots:
    void playback(QGraphicsSceneMouseEvent *event);
    void tick();
    void flowrate_increase();
    void flowrate_decrease();
    void Start();
    void setFlowrate(qreal Value);
private slots:
    void calcTau();
};

#endif // PFDCONTROL_H
