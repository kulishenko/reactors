#include "modelcell.h"

ModelCell::ModelCell(SchemaData *Data)
{
    this->Data = Data;
}

ModelCell::~ModelCell()
{

}

int ModelCell::N_f(const gsl_vector *x, void *data, gsl_vector *f)
{
    size_t n = ((struct data *)data)->n;
    double *y = ((struct data *)data)->y;
    double *sigma = ((struct data *) data)->sigma;

    double A = gsl_vector_get (x, 0);
    double lambda = gsl_vector_get (x, 1);
    double b = gsl_vector_get (x, 2);

    size_t i;

    for (i = 0; i < n; i++)
      {
        /* Model Yi = A * exp(-lambda * i) + b */
        double t = i;
        double Yi = A * exp (-lambda * t) + b;
        gsl_vector_set (f, i, (Yi - y[i])/sigma[i]);
      }

    return GSL_SUCCESS;
}

int ModelCell::N_df (const gsl_vector * x, void *data,
         gsl_matrix * J)
{
  size_t n = ((struct data *)data)->n;
  double *sigma = ((struct data *) data)->sigma;

  double A = gsl_vector_get (x, 0);
  double lambda = gsl_vector_get (x, 1);

  size_t i;

  for (i = 0; i < n; i++)
    {
      /* Jacobian matrix J(i,j) = dfi / dxj, */
      /* where fi = (Yi - yi)/sigma[i],      */
      /*       Yi = A * exp(-lambda * i) + b  */
      /* and the xj are the parameters (A,lambda,b) */
      double t = i;
      double s = sigma[i];
      double e = exp(-lambda * t);
      gsl_matrix_set (J, i, 0, e/s);
      gsl_matrix_set (J, i, 1, -t * A * e/s);
      gsl_matrix_set (J, i, 2, 1/s);
    }
  return GSL_SUCCESS;
}


int ModelCell::N_fdf(const gsl_vector * x, void *data,
                     gsl_vector * f, gsl_matrix * J)
{

    N_f (x, data, f);
    N_df (x, data, J);

    return GSL_SUCCESS;


}

void ModelCell::print_state(size_t iter, gsl_multifit_fdfsolver *s)
{

      qDebug() << QObject::tr("iter: %1 x = %2 %3 %4"
              "|f(x)| = %5\n").arg(
              QString::number(iter),
              QString::number(gsl_vector_get (s->x, 0)),
              QString::number(gsl_vector_get (s->x, 1)),
              QString::number(gsl_vector_get (s->x, 2)),
              QString::number(gsl_blas_dnrm2 (s->f)));


}

void ModelCell::EstimateNumCells()
{
    const gsl_multifit_fdfsolver_type *T;
    gsl_multifit_fdfsolver *s;
    int status;
    unsigned int i, iter = 0;
    const size_t n = N;
    const size_t p = 3;

    gsl_matrix *covar = gsl_matrix_alloc (p, p);
    double y[N], sigma[N];
    struct data d = { n, y, sigma};
    gsl_multifit_function_fdf f;
    double x_init[3] = { 1.0, 0.0, 0.0 };
    gsl_vector_view x = gsl_vector_view_array (x_init, p);
    const gsl_rng_type * type;
    gsl_rng * r;

    gsl_rng_env_setup();

    type = gsl_rng_default;
    r = gsl_rng_alloc (type);

    // Temp K0CTbI/\b to use C library with their types inside class
    f.f = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_vector*)> (&N_f);
    f.df = reinterpret_cast <int(*)(const gsl_vector *, void*, gsl_matrix*)> (&N_df);
    f.fdf = reinterpret_cast <int(*)(const gsl_vector *, void*, gsl_vector*, gsl_matrix*)> (&N_fdf);
    f.n = n;
    f.p = p;
    f.params = &d;

    /* This is the data to be fitted */

    // y[i] - array of exp data - Conc(i)
    // sigma[i] - standard error (?)

    for (i = 0; i < n; i++)
      {
        double t = i;
        y[i] = 1.0 + 5 * exp (-0.1 * t)
                   + gsl_ran_gaussian (r, 0.1);
        sigma[i] = 0.1;
       qDebug() << QObject::tr("data: %1 %2 %3\n").arg(QString::number(i), QString::number(y[i]), QString::number(sigma[i]));
      }

    T = gsl_multifit_fdfsolver_lmsder;
    s = gsl_multifit_fdfsolver_alloc (T, n, p);
    gsl_multifit_fdfsolver_set (s, &f, &x.vector);

    print_state (iter, s);

    do
      {
        iter++;
        status = gsl_multifit_fdfsolver_iterate (s);

        printf ("status = %s\n", gsl_strerror (status));

        print_state (iter, s);

        if (status)
          break;

        status = gsl_multifit_test_delta (s->dx, s->x,
                                          1e-4, 1e-4);
      }
    while (status == GSL_CONTINUE && iter < 500);

    gsl_multifit_covar (s->J, 0.0, covar);

    #define FIT(i) gsl_vector_get(s->x, i)
    #define ERR(i) sqrt(gsl_matrix_get(covar,i,i))

      {
        double chi = gsl_blas_dnrm2(s->f);
        double dof = n - p;
        double c = GSL_MAX_DBL(1, chi / sqrt(dof));

        printf("chisq/dof = %g\n",  pow(chi, 2.0) / dof);

        printf ("A      = %.5f +/- %.5f\n", FIT(0), c*ERR(0));
        printf ("lambda = %.5f +/- %.5f\n", FIT(1), c*ERR(1));
        printf ("b      = %.5f +/- %.5f\n", FIT(2), c*ERR(2));
      }

      printf ("status = %s\n", gsl_strerror (status));

      gsl_multifit_fdfsolver_free (s);
      gsl_matrix_free (covar);
      gsl_rng_free (r);
}

