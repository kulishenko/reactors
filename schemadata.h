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
    QVector<qreal> *p_ExpDataConductivity;
    QVector<qreal> *p_ExpDataTime;
    int* p_NumCascade;
    qreal* p_Flowrate; // Volume flowrate, L/hr
    QVector<qreal> Conc, SConc; // Tracer concentration, kmol/m3
    QVector<qreal> DimConc; // Dimensionless tracer concentration (a.k.a. C_theta)
    QVector<qreal> DimTime; // Dimensionless time (a.k.a. theta)
    QVector<QVector<qreal>* > SimConc;
    qreal t0; // Zero time, s
    qreal tend; // End time, s
    qreal tau; // Residence time, s
    qreal avg_tau; // Average residence time, s
    qreal M2t, M2theta; // 2-nd order moments
    qreal sigma2theta;
    qreal Nc;
    qreal *M0; // Initial moment
    void calcM0();
    qreal getM0();
    void calcAvgTau();
    void calcConc();
    void calcDimConc();
    void calcDimTime();
    void calcM2theta();
    void estimateNumCells();
    void SmoothData();
private:
    unsigned int m_DataRes; // Data resolution
    int i_t0;
    qreal Calibrate(qreal x);
    qreal dt(int i);
    qreal dim_dt(int i);
    QVector<qreal>::const_iterator t_0() const;
    QVector<qreal>::const_iterator t_last() const;
};

#endif // SCHEMADATA_H
