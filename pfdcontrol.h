#ifndef PFDCONTROL_H
#define PFDCONTROL_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QGraphicsPathItem>
#include <schemaitem.h>
#include <QVector>
#include <QList>
#include <QFile>
#include <QApplication>
//#include <schemacstr.h>
class SchemaItem;
class SchemaCSTR; // Why doesn't it works from header?
class PFDControl : public QObject
{
    Q_OBJECT

public:
    explicit PFDControl(QObject *parent = 0);
    ~PFDControl();
    qreal Flowrate;
    qreal PlaybackFlowrate;
    int NumCascade;
    QVector<SchemaCSTR*> *reactorItems;
    QVector<qreal> Time;
    QVector<qreal> Conductivity;
    QVector<qreal> Tau;
    QString PlaybackFileName;
    qreal TimeNow;
    bool isStarted;
    bool isFlowrateSet;
    void addItem(SchemaItem *item);
    void calcTau();
    void setNumCascade(int Value);
signals:
    void setLevel();
    void doSim();
    void startSim();
public slots:
    void tick();
    void flowrate_increase();
    void flowrate_decrease();
    void Start();
    void setFlowrate(qreal Value);
    void setPlaybackFlowrate(qreal Value);
};

#endif // PFDCONTROL_H
