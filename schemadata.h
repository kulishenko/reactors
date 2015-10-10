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
    qreal* Flowrate; // Volume flowrate, L/hr
    QVector<qreal> Conc; // Tracer concentration, kmol/m3
    QVector<qreal> DimConc; // Dimensionless tracer concentration
    QVector<qreal> DimTime; // Dimensionless time
    QVector<QVector<qreal>* > SimConc;
    qreal t0; // Zero time, s
    qreal tend; // End time, s
    void calcConc();
    void calcDimConc();
    void calcDimTime();
    void estimateNumCells();
    qreal Calibrate(qreal x);
    unsigned int DataRes; // Data resolution
private:
    qreal* t_0();
    qreal* t_last();
};

#endif // SCHEMADATA_H
