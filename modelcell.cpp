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
    double *arg = ((struct data *) data)->x;

    double Num = gsl_vector_get (x, 0);
    double Cin = gsl_vector_get (x, 1);

    for (int i = 0; i < n; i++)
      {
        /* Model Yi = A * exp(-lambda * i) + b */
        double t = arg[i];
        // double Yi = pow(Num, Num) * exp(Num - 1) * exp(-Num * t) /  gsl_sf_fact(ceil(Num)-1);
        // Attempt to use Gamma-function instead of factorial
        //double Yi = pow(Num, Num) * exp(Num - 1) * exp(-Num * t) /  gsl_sf_gamma(Num);
        //qDebug() << QString::number(Num);
        double Yi = Cin / gsl_sf_gamma(Num) * pow(t*Num,Num-1)* exp(-t*Num);
        gsl_vector_set (f, i, (Yi - y[i])/sigma[i]);
      }

    return GSL_SUCCESS;
}

int ModelCell::N_df (const gsl_vector * x, void *data,
         gsl_matrix * J)
{
  size_t n = ((struct data *)data)->n;
  double *sigma = ((struct data *) data)->sigma;
  double *arg = ((struct data *) data)->x;

  double Num = gsl_vector_get (x, 0);
  double Cin = gsl_vector_get (x, 1);



  for (int i = 0; i < n; i++)
    {
      /* Jacobian matrix J(i,j) = dfi / dxj, */
      /* where fi = (Yi - yi)/sigma[i],      */
      /*       Yi = A * exp(-lambda * i) + b  */
      /* and the xj are the parameters (A,lambda,b) */
      double t = arg[i];
      double s = sigma[i];
      double dCdn = -Cin * exp(-Num*t) * pow(Num*t,Num) * (Num*gsl_sf_psi(Num) - Num*log(Num*t) - Num + Num * t + 1)
              / (Num*Num * t * gsl_sf_gamma(Num));
      double dCdCin = exp(-Num * t)*pow(Num*t,Num-1)/gsl_sf_gamma(Num);


    gsl_matrix_set (J, i, 0, dCdn/s);
    gsl_matrix_set (J, i, 1, dCdCin/s);

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

      qDebug() << QObject::tr("iter: %1 x = %2 %3"
              "|f(x)| = %4\n").arg(
              QString::number(iter),
              QString::number(gsl_vector_get (s->x, 0)),
              QString::number(gsl_vector_get (s->x, 1)),
           //   QString::number(gsl_vector_get (s->x, 2)),
              QString::number(gsl_blas_dnrm2 (s->f)));


}

void ModelCell::EstimateNumCells()
{
    const gsl_multifit_fdfsolver_type *T;
    gsl_multifit_fdfsolver *s;
    int status;
    unsigned int i, iter = 0;
    const size_t n = Data->Conc.size() - 1;
    const size_t p = 2;

    gsl_matrix *covar = gsl_matrix_alloc (p, p);
    double y[n], sigma[n], t[n];
    struct data d = { n, y, t, sigma};
    gsl_multifit_function_fdf f;
    double x_init[2] = { 1, 1e-4};
    gsl_vector_view x = gsl_vector_view_array (x_init, p);

    // Temp K0CTbI/\b to use C library with their types inside class
    f.f = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_vector*)> (&N_f);
    f.df = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_matrix*)> (&N_df);
    f.fdf = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_vector*, gsl_matrix*)> (&N_fdf);
    f.n = n;
    f.p = p;
    f.params = &d;

    /* This is the data to be fitted */

    // y[i] - array of exp data - Conc(i)
    // sigma[i] - standard error (?)

    for (i = 0; i < n; i++)
      {
        //double t = i;
        y[i] = Data->Conc.at(i);
        sigma[i] = 1e-6;
        t[i] = Data->DimTime.at(i);
       //qDebug() << QObject::tr("data: %1 %2 %3\n").arg(QString::number(i), QString::number(y[i]), QString::number(sigma[i]));
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

        printf ("N      = %.5f +/- %.5f\n", FIT(0), c*ERR(0));
        printf ("Cin = %.5f +/- %.5f\n", FIT(1), c*ERR(1));

      }

      printf ("status = %s\n", gsl_strerror (status));

      Num = FIT(0);
      Cin = FIT(1);

      gsl_multifit_fdfsolver_free (s);
      gsl_matrix_free (covar);
}

void ModelCell::Sim()
{
    QVector<qreal> *CalcConc = new QVector<qreal>();
    for(int i = 0; i<Data->DimTime.size();i++)
        CalcConc->push_back(Conc(Data->DimTime.at(i)));

    Data->SimConc.push_back(CalcConc);
}

qreal ModelCell::Conc(qreal theta)
{

  return Cin / gsl_sf_gamma(Num) * pow(theta*Num,Num-1)* exp(-theta*Num);

}

