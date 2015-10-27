#ifndef SCHEMADATA_H
#define SCHEMADATA_H
#include <QVector>
#include <QDebug>
#include <QObject>
#include <pfdcontrol.h>


class SchemaData
{
public:
    SchemaData(PFDControl *Ctrl);
    ~SchemaData();
    PFDControl* Control;
    QVector<qreal> *ExpDataConductivity;
    QVector<qreal> *ExpDataTime;
    int* NumCascade;
    qreal* Flowrate; // Volume flowrate, L/hr
    QVector<qreal> Conc, SConc; // Tracer concentration, kmol/m3
    QVector<qreal> DimConc; // Dimensionless tracer concentration
    QVector<qreal> DimTime; // Dimensionless time
    QVector<QVector<qreal>* > SimConc;
    qreal t0; // Zero time, s
    qreal tend; // End time, s
    qreal tau; // Residence time, s
    qreal avg_tau; // Average residence time, s
    qreal M2t;
    qreal Nc;
    void calcAvgTau();
    void calcConc();
    void calcDimConc();
    void calcDimTime();
    void calcM2t();
    void estimateNumCells();
    qreal Calibrate(qreal x);
    unsigned int DataRes; // Data resolution
    void SmoothData();
private:
    qreal* t_0() const;
    qreal* t_last() const;
    qreal dt(int i);
    int i_t0;
};

#endif // SCHEMADATA_H
