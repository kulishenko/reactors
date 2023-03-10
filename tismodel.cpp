#include "tismodel.h"

TISModel::TISModel(SchemaData *Data) : SchemaModel(Data)
{

}

TISModel::~TISModel()
{

}

int TISModel::N_f(const gsl_vector *x, void *data, gsl_vector *f)
{

    size_t n = (static_cast<struct data *> (data))->n;
    const double *y = (static_cast<struct data *> (data))->y;
    const double *sigma = (static_cast<struct data *> (data))->sigma;
    const double *arg = (static_cast<struct data *> (data))->x;

    double Num = gsl_vector_get (x, 0);
    double Cin = gsl_vector_get (x, 1);

    for (unsigned int i = 0; i < n; i++)
      {

        double t = arg[i];

        // Attempt to use Gamma-function instead of factorial

        double Yi = Cin / gsl_sf_gamma(Num) * pow(t * Num, Num - 1)* exp(-t * Num);
        gsl_vector_set (f, i, (Yi - y[i])/sigma[i]);
      }

    return GSL_SUCCESS;
}

int TISModel::N_df (const gsl_vector * x, void *data,
         gsl_matrix * J)
{

    /*!
     * \brief Рассчитывает значение Якобиана векторной функции N(x) в точках x
     * \param[in] x указатель на первый элемент массива x
     * \param[in] data указатель на стуктуру, содержащую коэффициенты функции N(x)
     * \param[in,out] J Якобиан системы N(x)
     * \return значение GSL_SUCCESS в случае успешного расчета
     */

    size_t n = (static_cast<struct data *> (data))->n;
    const double *sigma = (static_cast<struct data *> (data))->sigma;
    const double *arg = (static_cast<struct data *> (data))->x;

    double Num = gsl_vector_get (x, 0);
    double Cin = gsl_vector_get (x, 1);

    for (size_t i = 0; i < n; i++)
    {
      /* Jacobian matrix J(i,j) = dfi / dxj, */
      /* where fi = (Yi - yi)/sigma[i],      */
      /*       Yi   */
      /* and the xj are the parameters (Num,Cin) */
      double t = arg[i];
      double s = sigma[i];
      double dCdn = -Cin * exp(-Num * t) * pow(Num * t, Num)
              * (Num*gsl_sf_psi(Num) - Num * log(Num * t) - Num + Num * t + 1)
              / (Num*Num * t * gsl_sf_gamma(Num));
      double dCdCin = exp(-Num * t) * pow(Num * t, Num - 1)/gsl_sf_gamma(Num);

    gsl_matrix_set (J, i, 0, dCdn/s);
    gsl_matrix_set (J, i, 1, dCdCin/s);

    }
    return GSL_SUCCESS;
}


int TISModel::N_fdf(const gsl_vector * x, void *data,
                     gsl_vector * f, gsl_matrix * J)
{
    N_f (x, data, f);
    N_df (x, data, J);

    return GSL_SUCCESS;

}

void TISModel::print_state(size_t iter, gsl_multifit_fdfsolver *s)
{

    qDebug() << QObject::tr("iter: %1 x = %2 %3"
          "|f(x)| = %4\n").arg(
          QString::number(iter),
          QString::number(gsl_vector_get (s->x, 0)),
          QString::number(gsl_vector_get (s->x, 1)),
          QString::number(gsl_blas_dnrm2 (s->f)));


}

int TISModel::func_C (double t, const double y[], double f[],
      void *params)
{
    Q_UNUSED(t);
    qreal *arg = static_cast<qreal *> (params);
    int nCells = arg[0];
    qreal tau = arg[1];
    qreal C_in = 0;
    f[0] = 1 / tau * (C_in - y[0]);
    for(int i = 1; i < nCells; i++){
        f[i] = 1 / tau * (y[i-1] - y[i]);
    }
    return GSL_SUCCESS;
}

qreal TISModel::getCin() const
{
    return Cin;
}

qreal TISModel::getNum() const
{
    return Num;
}

unsigned int TISModel::getiNum() const
{
    return iNum;
}
int TISModel::jac_C (double t, const double y[], double *dfdy,
     double dfdt[], void *params)
{
    Q_UNUSED(t);
    Q_UNUSED(y);
    qreal *arg = static_cast<qreal *> (params);
    int nCells = arg[0];
    qreal tau = arg[1];


    gsl_matrix_view dfdy_mat
    = gsl_matrix_view_array (dfdy, nCells, nCells);
    gsl_matrix * m = &dfdy_mat.matrix;

    gsl_matrix_set (m, 0, 0, -1/tau);
    for(int i = 1; i < nCells; i++)
      for(int j = 1; j < nCells; j++)
      {
          gsl_matrix_set (m, i, j, 0.0f);
          if(i == j)
          {
              gsl_matrix_set (m, i, i, -1/tau);
              gsl_matrix_set (m, i, i-1, 1/tau);
          }
      }

    for(int i = 0; i < nCells; i++)
       dfdt[i] = 0.0;
    return GSL_SUCCESS;
}


void TISModel::EstimateNumCells()
{
    const gsl_multifit_fdfsolver_type *T;
    gsl_multifit_fdfsolver *s;
    int status;
    unsigned int iter = 0;

    const QVector<qreal> &SConc = p_Data->getSConc();
    const QVector<qreal> &DimTime = p_Data->getDimTime();

    const size_t n = SConc.size();
    const size_t p = 2;

    gsl_matrix *covar = gsl_matrix_alloc (p, p);
    QVector<qreal> sigma;
    sigma.fill(1e-6, n);
    struct data d = { n, SConc.data(), DimTime.data(), sigma.data()};
    gsl_multifit_function_fdf f;
    double x_init[2] = { 1, 1e-4};
    gsl_vector_view x = gsl_vector_view_array (x_init, p);

    f.f = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_vector*)> (&N_f);
    f.df = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_matrix*)> (&N_df);
    f.fdf = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_vector*, gsl_matrix*)> (&N_fdf);
    f.n = n;
    f.p = p;
    f.params = &d;

    T = gsl_multifit_fdfsolver_lmsder;
    s = gsl_multifit_fdfsolver_alloc (T, n, p);
    gsl_multifit_fdfsolver_set (s, &f, &x.vector);

    print_state (iter, s);



    do
      {
        iter++;
        status = gsl_multifit_fdfsolver_iterate (s);

        printf ("status = %s\n", gsl_strerror (status));

      //  print_state (iter, s);

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
      iNum = round(Num);

      gsl_multifit_fdfsolver_free (s);
      gsl_matrix_free (covar);
}

void TISModel::Sim(const bool dimensionless)
{
    QVector<qreal> *CalcConc = new QVector<qreal>();
    const QVector<qreal> &DimTime = p_Data->getDimTime();

    qreal tau = p_Data->getAvgTau();
    qreal M0 = p_Data->getM0();

    auto end = DimTime.constEnd();
    for(auto iter = DimTime.constBegin(); iter != end; ++iter)
        if(dimensionless)
            CalcConc->push_back(Conc(*iter) * tau / M0);
        else
            CalcConc->push_back(Conc(*iter));

    p_Data->addSimData(*CalcConc,
                QString(QObject::tr("Tanks-in-Series Model (N = %1), Exact Solution")).arg(Num));
}

void TISModel::SimODE(const bool dimensionless)
{
    qreal avg_tau = p_Data->getAvgTau();
    qreal params[3] = {static_cast<qreal>(iNum), avg_tau/iNum, Cin};
    gsl_odeiv2_system sys = {func_C, jac_C, iNum, &params};

    gsl_odeiv2_driver * d =
    gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rk8pd,
                  1e-6, 1e-6, 0.0);
    unsigned int i;
    QVector<qreal> *CalcConc = new QVector<qreal>;
    double t = 0.0;
    double y[iNum];
    y[0] = Cin;
    for (i = 1; i < iNum; i++)
      y[i] = 0.0f;

    qreal tau = p_Data->getAvgTau();
    qreal M0 = p_Data->getM0();

    const QVector<qreal> &DimTime = p_Data->getDimTime();

    unsigned int nP = DimTime.size() - 1;
    for (i = 1; i <= nP; i++)
    {
          qreal ti = DimTime.at(i) * avg_tau;
          int status = gsl_odeiv2_driver_apply (d, &t, ti, y);

          if (status != GSL_SUCCESS)
        {
          printf ("error, return value=%d\n", status);
          break;
        }
          if(dimensionless)
            CalcConc->push_back(y[iNum-1] * tau / M0);
          else
            CalcConc->push_back(y[iNum-1]);

        //  printf ("%.5e %.5e\n", t, y[iNum-1]);
    }
    p_Data->addSimData(*CalcConc,
                QString(QObject::tr("Tanks-in-Series Model (N = %1), Numerical ODE Solution")).arg(iNum));

    gsl_odeiv2_driver_free (d);
}

qreal TISModel::Conc(const qreal theta) const
{
    return Cin / gsl_sf_gamma(Num) * pow(theta * Num, Num - 1) * exp(-theta * Num);
}

