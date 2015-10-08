#ifndef SCHEMADATA_H
#define SCHEMADATA_H
#include <QVector>
#include <QDebug>
#include <QObject>
#include <pfdcontrol.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>
#define GSL_DLL


class SchemaData
{
    struct data {
      size_t n;
      double * y;
      double * sigma;
    };

    int
    expb_f (const gsl_vector * x, void *data,
            gsl_vector * f)
    {
      size_t n = ((struct data *)data)->n;
      double *y = ((struct data *)data)->y;
      double *sigma = ((struct data *) data)->sigma;

      double A = gsl_vector_get(x, 0);
      double lambda = gsl_vector_get(x, 1);
      double b = gsl_vector_get(x, 2);

      size_t i;

      for (i = 0; i < n; i++)
        {
          /* Model Yi = A * exp(-lambda * i) + b */
          double t = i;
          double Yi = A * exp(-lambda * t) + b;
          gsl_vector_set(f, i, (Yi - y[i])/sigma[i]);
        }

      return GSL_SUCCESS;
    }
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
    qreal t0; // Zero time, s
    qreal tend; // End time, s
    void calcConc();
    void calcDimConc();
    void calcDimTime();
    void estimateNumCells();
    qreal Calibrate(qreal x);
private:
    qreal* t_0();
    qreal* t_last();
};

#endif // SCHEMADATA_H
