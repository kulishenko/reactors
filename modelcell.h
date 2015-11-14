#ifndef MODELCELL_H
#define MODELCELL_H
#include <schemadata.h>
#include <QDebug>

#define GSL_DLL
#include <gsl/gsl_math.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_sf.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>


class ModelCell
{
public:
    ModelCell(SchemaData *Data);
    ~ModelCell();
    struct data {
        size_t n;
        const double * y;
        const double * x;
        double * sigma;
    };
    static int N_f(const gsl_vector *x, void *data, gsl_vector *f);
    static int N_df(const gsl_vector * x, void *data, gsl_matrix * J);
    static int N_fdf(const gsl_vector *x, void *data, gsl_vector *f, gsl_matrix *J);
    void print_state (size_t iter, gsl_multifit_fdfsolver * s);
    void EstimateNumCells();
    void Sim();
    void SimODE();
    static int jac_C(double t, const double y[], double *dfdy, double dfdt[], void *params);
    static int func_C(double t, const double y[], double f[], void *params);
    qreal getCin() const;
    qreal getNum() const;
    unsigned int getiNum() const;
private:
    SchemaData* p_Data;
    qreal Cin; // Estimated initial tracer concentration, mol/l
    qreal Num; // Estimated number of cells
    unsigned int iNum; // Rounded number of cells
    qreal Conc(const qreal theta) const;
};

#endif // MODELCELL_H
