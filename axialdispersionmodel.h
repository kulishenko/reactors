#ifndef AXIALDISPERSIONMODEL_H
#define AXIALDISPERSIONMODEL_H

#include <schemadata.h>
#include <schemamodel.h>
#include <QDebug>

#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>

#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>

#include <QtMath>

/*!
 * \brief Класс, предназначенный для расчета схемы с использованием однопараметрической диффузионной модели
 *
 * Содержит:
 * - модуль оценки значения числа Пекле по вероятностным характеристикам, а также методом Левенберга-Марквардта;
 * - модуль приближенного расчета значения функции отклика с использованием соотношения, представляющего
 * собой бесконечно медленно сходящийся ряд, а также с использованием нормального закона распределения;
 * - модуль поиска положительных корней трансцендентного уравнения методом сканирования совместно с методом Ньютона.
 *
 */

class AxialDispersionModel : public SchemaModel
{
public:
    AxialDispersionModel(SchemaData* Data);
    ~AxialDispersionModel();
    void CalcLambda();
    void CalcPe(const bool simplified = true);
    void Sim(const bool simplified = false);
private:
    struct lambda_eqn_params
    {
        double Pe;
    };
    struct Pe_eqn_params
    {
        double sigma2theta;
    };
    struct data {
        size_t n;
        const double * y;
        const double * x;
        double * sigma;
    };
    inline static double lambda_eqn(double lambda, void *params);
    inline static double lambda_eqn_deriv(double lambda, void *params);
    static void lambda_eqn_fdf(double lambda, void *params, double *y, double *dy);
    inline static double Pe_eqn(double Pe, void *params);
    inline static double Pe_eqn_deriv(double Pe, void *params);
    static void Pe_eqn_fdf(double Pe, void *params, double *y, double *dy);
    int m_N; //!< Число первых членов бесконечного ряда, использующихся при суммировании
    qreal m_Pe; //!< Значение числа Pe
    QVector<qreal> *p_Lambda; //!< Указатель на массив положительных корней трансцендентного уравнения
    qreal Conc(const qreal theta) const;
    qreal ConcSimp(const qreal theta) const;
    static int C_theta_f(const gsl_vector *x, void *data, gsl_vector *f);
    static int C_theta_df(const gsl_vector *x, void *data, gsl_matrix *J);
    static int C_theta_fdf(const gsl_vector *x, void *data, gsl_vector *f, gsl_matrix *J);
    static void print_state(size_t iter, gsl_multifit_fdfsolver *s);
};

#endif // AXIALDISPERSIONMODEL_H
