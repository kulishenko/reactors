#ifndef MODELCELL_H
#define MODELCELL_H
#include <schemadata.h>
#include <gsl/gsl_math.h>
#include <QDebug>

#define GSL_DLL
#include <gsl/gsl_math.h>
//#include <gsl/gsl_rng.h>
//#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_sf.h>

//#include "expfit.c"
//#define N 40


class ModelCell
{
public:
    ModelCell(SchemaData *Data);
    ~ModelCell();
    // Needed to use in C-style functions
    struct data {
      size_t n;
      double * y;
      double * x;
      double * sigma;
    };
    SchemaData* Data;
    static int N_f(const gsl_vector *x, void *data, gsl_vector *f);
    static int N_df(const gsl_vector * x, void *data, gsl_matrix * J);
    static int N_fdf(const gsl_vector *x, void *data, gsl_vector *f, gsl_matrix *J);
    void print_state (size_t iter, gsl_multifit_fdfsolver * s);

    void EstimateNumCells();
    void Sim();
    qreal Cin;
    qreal Num;
private:
    qreal Conc(qreal theta);
};

#endif // MODELCELL_H
