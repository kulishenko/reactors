#ifndef SCHEMADATA_H
#define SCHEMADATA_H
#include <QVector>
#include <QDebug>
#include <QObject>
#include <pfdcontrol.h>
#include <gsl/gsl_math.h>

class SchemaData
{
public:
    SchemaData(PFDControl *Ctrl);
    ~SchemaData();
    PFDControl* Control;
    QVector<qreal> *ExpDataConductivity;
    QVector<qreal> *ExpDataTime;
    QVector<qreal> Conc; // Tracer concentration
    QVector<qreal> DimConc; // Dimensionless tracer concentration
    QVector<qreal> DimTime; // Dimensionless time
    void calcConc();
    void calcDimConc();
    void calcDimTime();
    qreal Calibrate(qreal x);
private:
    qreal* t_0();
    qreal* t_last();
};

#endif // SCHEMADATA_H
