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
    QVector<SchemaCSTR*> *reactorItems;
    void addItem(SchemaItem *item);
    void calcTau();
    void setNumCascade(int Value);
    qreal getCurrentTime();
    void addPoint(qreal Time, qreal Value);
    const QVector<qreal> &getTime() const;
    const QVector<qreal> &getParameter() const;
    qreal getTimeAt(const size_t i) const;
    qreal getParameterAt(const size_t i) const;
    qreal getTauAt(const size_t i) const; // TODO: Replace with ModelCSTR
    int getCount() const;
    qreal getFlowrate() const;
    qreal getPlaybackFlowrate() const;
    void setPlaybackFileName(const QString &str);
    const QString &getPlaybackFileName() const;
    int getNumCascade() const;
private:
    qreal m_Flowrate;
    qreal m_PlaybackFlowrate;
    QString m_PlaybackFileName;
    int m_NumCascade;
    QVector<qreal> m_Time;
    QVector<qreal> m_Parameter;
    bool isStarted;
    bool isFlowrateSet;
    qreal TimeNow;
    QVector<qreal> Tau; // Transient times for CSTRs (temp) -> to be moved to ModelCSTR
signals:
    void setLevel();
    void doSim();
    void startSim();
    void started();
public slots:
    void tick();
    void flowrate_increase();
    void flowrate_decrease();
    void Start();
    void setFlowrate(qreal Value);
    void setPlaybackFlowrate(qreal Value);
};

#endif // PFDCONTROL_H
