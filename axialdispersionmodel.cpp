#include "axialdispersionmodel.h"

AxialDispersionModel::AxialDispersionModel(SchemaData *Data) : SchemaModel(Data), p_Lambda(nullptr)
{

}

AxialDispersionModel::~AxialDispersionModel()
{

}

/*!
 * \brief Находит первые m_N положительных корней трансцендентного уравнения tan(lambda) = 4*Pe*lambda / (4*lambda^2 - Pe^2)
 *
 * Используется метод сканирования совместно с методом Ньютона
 */
void AxialDispersionModel::CalcLambda()
{

    const gsl_root_fdfsolver_type *T;
    gsl_root_fdfsolver *s;
    double x0, x = 0.021f, root;
    gsl_function_fdf FDF;
    struct lambda_eqn_params params = { m_Pe };


    FDF.f = &lambda_eqn;
    FDF.df = &lambda_eqn_deriv;
    FDF.fdf = &lambda_eqn_fdf;
    FDF.params = &params;
    int max_iter = 100;



    m_N = 150;
    int N = m_N;

    if(p_Lambda)
        delete p_Lambda;

    p_Lambda = new QVector<qreal>;
    p_Lambda->push_back(0.0f);

    int k = 1;
    qreal h = 0.0151f;
    while(k < N)
    {

        root = x;
        int status;
        int iter = 0;
        T = gsl_root_fdfsolver_newton;
        s = gsl_root_fdfsolver_alloc (T);
        gsl_root_fdfsolver_set (s, &FDF, root);

        do
          {
            iter++;
            status = gsl_root_fdfsolver_iterate (s);
            x0 = root;
            root = gsl_root_fdfsolver_root (s);
            status = gsl_root_test_delta (root, x0, 0, 1e-20);


          }
        while (status == GSL_CONTINUE && iter < max_iter);
        gsl_root_fdfsolver_free (s);

        if(fabs(p_Lambda->last() - root) > 0.1 && root > 0){
            p_Lambda->push_back(root); k++;
            qDebug() << p_Lambda->at(k-1);
        }
        x += h;
    }

}
/*!
* \brief Определяет значение критерия Pe
* \param[in] simplified Принудительное использование упрощенного метода расчета
*
* Определяет значение критерия Pe:
*  - по вероятностным характеристикам путем решения алгебраического управнения относительно числа Pe;
*  - методом Левенберга-Марквардта путем решения задачи нелинейного МНК.
*
*/
void AxialDispersionModel::CalcPe(const bool simplified)
{


    if(simplified) {
        const gsl_root_fdfsolver_type *T;
        gsl_root_fdfsolver *s;
        struct Pe_eqn_params params = {p_Data->getSigma2theta()};
        double x0, x = p_Data->getSigma2theta() / 2;
        gsl_function_fdf FDF;

        FDF.f = &Pe_eqn;
        FDF.df = &Pe_eqn_deriv;
        FDF.fdf = &Pe_eqn_fdf;
        FDF.params = &params;

        int status;
        int iter = 0, max_iter = 100;
        T = gsl_root_fdfsolver_newton;
        s = gsl_root_fdfsolver_alloc (T);
        gsl_root_fdfsolver_set (s, &FDF, x);

        /*
        printf ("using %s method\n",
                gsl_root_fdfsolver_name (s));

        printf ("%-5s %10s %10s\n",
                "iter", "root", "err");
                */
        do
          {
            iter++;
            status = gsl_root_fdfsolver_iterate (s);
            x0 = x;
            x = gsl_root_fdfsolver_root (s);
            status = gsl_root_test_delta (x, x0, 0, 1e-6);

       /*     if (status == GSL_SUCCESS)
              printf ("Converged:\n");

            printf ("%5d %10.7f %10.7f\n",
                    iter, x, x - x0); */
          }
        while (status == GSL_CONTINUE && iter < max_iter);
        gsl_root_fdfsolver_free (s);
        qDebug() << QString("Pe = ") << x;
        m_Pe = x;

    } else {

        const gsl_multifit_fdfsolver_type *T;
        gsl_multifit_fdfsolver *s;
        int status;
        unsigned int iter = 0;

        const QVector<qreal> &SConc = p_Data->getDimConc();
        const QVector<qreal> &DimTime = p_Data->getDimTime();

        const size_t n = SConc.size();
        const size_t p = 1;

        gsl_matrix *covar = gsl_matrix_alloc (p, p);
        QVector<qreal> sigma;
        sigma.fill(1e-6, n);
        struct data d = { n, SConc.data(), DimTime.data(), sigma.data()};
        gsl_multifit_function_fdf f;
        double x_init[p] = { p_Data->getSigma2theta() / 2 };
        gsl_vector_view x = gsl_vector_view_array (x_init, p);

        f.f = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_vector*)> (&C_theta_f);
        f.df = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_matrix*)> (&C_theta_df);
        f.fdf = reinterpret_cast <int(*)(const gsl_vector*, void*, gsl_vector*, gsl_matrix*)> (&C_theta_fdf);
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

      //      printf ("status = %s\n", gsl_strerror (status));

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
     //       double chi = gsl_blas_dnrm2(s->f);
     //       double dof = n - p;
     //       double c = GSL_MAX_DBL(1, chi / sqrt(dof));

     //       printf("chisq/dof = %g\n",  pow(chi, 2.0) / dof);

     //       printf ("Pe      = %.5f +/- %.5f\n", FIT(0), c*ERR(0));


          }

     //     printf ("status = %s\n", gsl_strerror (status));

          m_Pe = FIT(0);
          qDebug() << QString("Pe = ") << m_Pe;

          gsl_multifit_fdfsolver_free (s);
          gsl_matrix_free (covar);


    }

}
/*!
 * \brief Осуществляет расчет диффузионной модели
 * \param[in] simplified Принудительное использование упрощенного метода
 *
 * Рассчитывает значение функции отклика для диффузионной модели и помещает результат контейнер связанного объекта SchemaData
 */
void AxialDispersionModel::Sim(const bool simplified)
{

    qreal simp_threshold = 4.0f;
    qreal acc_threshold = 0.01f;
    QVector<qreal> *CalcConc = new QVector<qreal>();
    const QVector<qreal> &DimTime = p_Data->getDimTime();


    if(!simplified && m_Pe <= simp_threshold)
        CalcLambda();

    auto end = DimTime.constEnd();
    for(auto iter = DimTime.constBegin(); iter != end; ++iter)
        if(simplified || m_Pe >= simp_threshold)
            CalcConc->push_back(ConcSimp(*iter));
        else if((*iter) > acc_threshold)
            CalcConc->push_back(Conc(*iter));
        else
            CalcConc->push_back(0.0f);

    QString calcType;
    if(simplified || m_Pe >= simp_threshold)
        calcType.append(QObject::tr("Normal Distribution"));
    else
        calcType.append(QObject::tr("Series Approximation"));


    p_Data->addSimData(*CalcConc,
                QString(QObject::tr("Axial Dispersion Model (Pe = %1), %2")).arg(QString::number(m_Pe, 'f', 3), calcType));
}
/*!
 * \brief Рассчитывает значение ошибки трансцендентного уравнения
 * \param[in] lambda Текущее приближение корня
 * \param[in] params Указатель на структуру, содержащую коэффициенты уравнения
 * \return Значение ошибки трансцендентного уравнения
 */
double AxialDispersionModel::lambda_eqn(double lambda, void *params)
{

    struct lambda_eqn_params *p = static_cast<struct lambda_eqn_params *> (params);
    double Pe = p->Pe;

    return tan(lambda) - 4*Pe*lambda / (4*lambda*lambda - Pe*Pe);
}
/*!
 * \brief Рассчитывает значение производной функции ошибки трансцендентного уравнения в точке lambda
 * \param[in] lambda Текущее приближение корня
 * \param[in] params Указатель на структуру, содержащую коэффициенты уравнения
 * \return Значение производной функции ошибки трансцендентного уравнения в точке lambda
 */
double AxialDispersionModel::lambda_eqn_deriv(double lambda, void *params)
{
    struct lambda_eqn_params *p = static_cast<struct lambda_eqn_params *> (params);
    double Pe = p->Pe;
    double lambda2 = lambda * lambda;
    double Pe2 = Pe * Pe;

    return 4*Pe / (Pe2 - 4*lambda2) + tan(lambda) * tan(lambda) + 32 * Pe * lambda2 / ((Pe2 - 4*lambda2) * (Pe2 - 4*lambda2)) + 1;
}
/*!
 * \brief Обертка для lambda_eqn и lambda_eqn_deriv, используемая GSL
 * \param[in] lambda Текущее приближение корня
 * \param[in] params Указатель на структуру, содержащую коэффициенты уравнения
 * \param[out] y Значение ошибки трансцендентного уравнения
 * \param[out] dy Значение производной функции ошибки трансцендентного уравнения в точке lambda
 */
void AxialDispersionModel::lambda_eqn_fdf(double lambda, void *params, double *y, double *dy)
{
    *y = lambda_eqn(lambda, params);
    *dy = lambda_eqn_deriv(lambda, params);
}

/*!
 * \brief Рассчитывает значение ошибки уравнения для определения числа Pe
 * \param[in] Pe Текущее приближение значения Pe
 * \param[in] params Указатель на структуру, содержащую коэффициенты уравнения
 * \return Значение ошибки уравнения
 */
double AxialDispersionModel::Pe_eqn(double Pe, void *params)
{
    struct Pe_eqn_params *p = static_cast<struct Pe_eqn_params *>(params);
    double sigma2theta = p->sigma2theta;

    return sigma2theta - 2 / (Pe * Pe) * (Pe - 1 + exp(-Pe));
}
/*!
 * \brief Рассчитывает значение производной функции ошибки ошибки уравнения для определения числа Pe в точке Pe
 * \param[in] Pe Текущее приближение корня
 * \param[in] params Указатель на структуру, содержащую коэффициенты уравнения
 * \return Значение производной функции ошибки уравнения для определения числа Pe
 */
double AxialDispersionModel::Pe_eqn_deriv(double Pe, void *params)
{

    Q_UNUSED(params)
    double exp_negPe = exp(-Pe);
    return (4 * exp_negPe + Pe * (2 * exp_negPe + 2) - 4) / (Pe * Pe * Pe);
}

/*!
 * \brief Обертка для Pe_eqn и Pe_eqn_deriv, используемая GSL
 * \param[in] Pe Текущее приближение корня
 * \param[in] params Указатель на структуру, содержащую коэффициенты уравнения
 * \param[out] y Значение ошибки трансцендентного уравнения
 * \param[out] dy Значение производной функции ошибки трансцендентного уравнения в точке lambda
 */
void AxialDispersionModel::Pe_eqn_fdf(double Pe, void *params, double *y, double *dy)
{
    *y = Pe_eqn(Pe, params);
    *dy = Pe_eqn_deriv(Pe, params);
}

/*!
 * \param[in] theta Значение безразмерного времени
 * \return Значение функции отклика однопараметрической модели, рассчитанное путем суммирования первых m_N членов бесконечного ряда
 */
qreal AxialDispersionModel::Conc(const qreal theta) const
{
    qreal C = 0;
    qreal Pe2 = m_Pe * m_Pe;
    for(int i = 1; i < m_N; i++){
        qreal A = p_Lambda->at(i);
        qreal A2 = A * A;
        C +=  2 * A * sin(A) * (Pe2 + 4*A2) * exp(-(Pe2 + 4 * A2 * theta) / (4 * m_Pe)) /
                (m_Pe * (Pe2 + 4*m_Pe + 4*A2));
    }
 //   qDebug() << "C(" << theta << ") = " << exp(m_Pe / 2) * C;
    return exp(m_Pe / 2) * C;
}

/*!
 * \param[in] theta Значение безразмерного времени
 * \return Значение функции отклика однопараметрической модели, рассчитанное как значение функции плотности вероятности для нормального закона распределения
 */
qreal AxialDispersionModel::ConcSimp(const qreal theta) const
{

    return 1/(2 * sqrt(M_PI/m_Pe)) * exp(-(1 - theta) * (1 - theta) * m_Pe / 4);
}

/*!
 * \brief Табулирует функцию C(x) в узлах сетки x
 * \param[in] x указатель на первый элемент массива x
 * \param[in] data указатель на стуктуру, содержащую коэффициенты функции C(x)
 * \param[out] f указатель на структуру, содержащую табулированные значения функции
 * \return значение GSL_SUCCESS в случае успешного расчета
 */
int AxialDispersionModel::C_theta_f(const gsl_vector *x, void *data, gsl_vector *f)
{


    size_t n = (static_cast<struct data *> (data))->n;
    const double *y = (static_cast<struct data *> (data))->y;
    const double *sigma = (static_cast<struct data *> (data))->sigma;
    const double *arg = (static_cast<struct data *> (data))->x;

    double Pe = gsl_vector_get (x, 0);


    for (unsigned int i = 0; i < n; i++)
      {

        double theta = arg[i];

        double Yi = 1/(2 * sqrt(M_PI/Pe)) * exp(-(1 - theta) * (1 - theta) * Pe / 4);
        gsl_vector_set (f, i, (Yi - y[i])/sigma[i]);
      }

    return GSL_SUCCESS;
}

/*!
 * \brief Рассчитывает значение Якобиана функции C(x) в узлах сетки x
 * \param[in] x указатель на первый элемент массива x
 * \param[in] data указатель на стуктуру, содержащую коэффициенты функции C(x)
 * \param[in,out] J указатель на структуру, содерщащую значения Якобиана системы С(x) в точках x
 * \return значение GSL_SUCCESS в случае успешного расчета
 */
int AxialDispersionModel::C_theta_df (const gsl_vector * x, void *data,
         gsl_matrix * J)
{

    size_t n = (static_cast<struct data *> (data))->n;
    const double *sigma = (static_cast<struct data *> (data))->sigma;
    const double *arg = (static_cast<struct data *> (data))->x;

    double Pe = gsl_vector_get (x, 0);


    for (size_t i = 0; i < n; i++)
    {
      /* Jacobian matrix J(i,j) = dfi / dxj, */
      /* where fi = (Yi - yi)/sigma[i],      */
      /*       Yi   */
      /* and the xj are the parameters (Num,Cin) */
      double theta = arg[i];
      double s = sigma[i];
      double dCdPe = -1 / (8 * M_SQRTPI * sqrt(1/Pe)) * exp(-Pe * (theta-1)*(theta-1) / 4) *
              (theta * theta - 2 * theta + 1 - 2/Pe);


    gsl_matrix_set (J, i, 0, dCdPe/s);


    }
    return GSL_SUCCESS;
}

/*!
 * \brief Обертка для одновременного вызова C_theta_f и C_theta_df
 * \param[in] x указатель на первый элемент массива x
 * \param[in] data указатель на стуктуру, содержащую коэффициенты функции C(x)
 * \param[out] f указатель на структуру, содержащую табулированные значения функции
 * \param[out] J указатель на структуру, содерщащую значения Якобиана системы С(x) в точках x
 * \return значение GSL_SUCCESS в случае успешного расчета
 */
int AxialDispersionModel::C_theta_fdf(const gsl_vector * x, void *data,
                     gsl_vector * f, gsl_matrix * J)
{

    C_theta_f (x, data, f);
    C_theta_df (x, data, J);

    return GSL_SUCCESS;

}

/*!
 * \brief Выводит в stdin состояние текущей итерации
 * \param[in] iter номер итерации
 * \param[in] s указатель на решатель
 */
void AxialDispersionModel::print_state(size_t iter, gsl_multifit_fdfsolver *s)
{

    qDebug() << QObject::tr("iter: %1 x = %2"
          "|f(x)| = %3\n").arg(
          QString::number(iter),
          QString::number(gsl_vector_get (s->x, 0)),
          QString::number(gsl_blas_dnrm2 (s->f)));


}
