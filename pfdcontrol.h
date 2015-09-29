#ifndef PFDCONTROL_H
#define PFDCONTROL_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QGraphicsPathItem>
#include <schemaitem.h>
#include <QVector>

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
    qreal TimeNow;
    bool isStarted;
signals:
    void setLevel1();
    void doSim();
    void startSim();
public slots:
    void playback(QGraphicsSceneMouseEvent *event);
    void tick();
    void test();
    void flowrate_increase();
    void flowrate_decrease();
    void Start();
};

#endif // PFDCONTROL_H
