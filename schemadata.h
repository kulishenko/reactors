#ifndef SCHEMADATA_H
#define SCHEMADATA_H
#include <QVector>
#include <QDebug>
#include <QObject>
#include <pfdcontrol.h>

class TISModel;
/*!
  \brief Класс, предназначенный для обработки экспериментальных данных и хранения результатов моделирования

  Обеспечивает определение времени начала и конца эксперимента, пересчет электропроводности раствора
  в концентрацию трассера, сглаживание экспериментальных данных медианным фильтром и фильтром скользящего
  среднего, расчет моментов концентрации трассера на выходе из аппарата как случайной величины,
  определение на их основе среднего времени пребывания вещества в аппарате

 */
class SchemaData
{
    friend class TISModel;
    friend class AxialdispersionModel;
public:
    SchemaData(PFDControl *Ctrl);
    ~SchemaData();
    void calcM0();
    qreal getM0();
    qreal getAvgTau();
    qreal getTau() const;
    qreal getNc() const;
    qreal getM2theta() const;
    qreal getSigma2theta() const;
    void calcAvgTau();
    void calcConc();
    void calcDimConc();
    void calcDimTime();
    void calcM2theta();
    void estimateNumCells();
    void SmoothData();
    qreal getConcAt(const size_t i) const;
    qreal getSConcAt(const size_t i) const;
    qreal getDimTimeAt(const size_t i) const;
    const QVector<qreal> &getSConc() const;
    const QVector<qreal> &getDimConc() const;
    const QVector<qreal> &getDimExpConc() const;
    const QVector<qreal> &getDimTime() const;
    const QVector<qreal> &getSimConc(const size_t i) const;
    size_t getSimConcCount() const;
    void calcDimExpConc();
    qreal getDimExpConcAt(const size_t i) const;
    const QString &getSimMethod(const int i) const;
    void addSimData(const QVector<qreal> & simData, const QString& name = QString());
private:
    void setSimMethod(QString name); //!< Задает название метода расчета
    const QVector<qreal> &p_ExpDataTime; //!< Ссылка на массив времени, соответствующего экспериментальным данным
    const QVector<qreal> &p_ExpDataConductivity; //!< Ссылка на массив экспериментальных данных
    int m_NumCascade; //!< Количество аппаратов в каскаде (по заданию)
    qreal m_Flowrate; //!< Оъемный расход вещества на входе экспериментальной установки, л/ч
    size_t m_DataRes; //!< Разрешение данных
    int i_t0; //!< Индекс элемента, соответствующего началу опыта
    qreal t0; //!< Время начала эксперимента, с
    qreal tend; //!< Время конца эксперимента, с
    qreal m_tau; //!< Расчетное время пребывания вещества в схеме, с
    qreal *avg_tau; //!< Среднее время пребывания вещества в схеме, с
    qreal *M0; //!< Начальный момент 0-го порядка
    qreal M2t, M2theta; //!< Размерный и безразмерный центральные моменты 2го порядка
    qreal sigma2theta; //!< Дисперсия безразмерного времени
    qreal Nc; //!< Число ячеек
    qreal Calibrate(qreal x) const; //!< Калибровка (пересчет электропроводности раствора в мСм/м в концентрацию трассера, моль/л)
    qreal dt(const size_t i) const; //!< Текущий шаг размерного времени, с
    qreal dim_dt(const size_t i) const; //!< Текущий шаг безразмерного времени
    QVector<qreal>::const_iterator t_0() const;
    QVector<qreal>::const_iterator t_last() const;
    QVector<qreal> Conc, SConc; //!< Концетрация трассера, моль/л
    QVector<qreal> DimTime; //!< Безразмерное время (a.k.a. theta)
    QVector<qreal> DimConc; //!< Безразмерная концентрация трассера (a.k.a. C_theta)
    QVector<qreal> DimExpConc; //!< Безразмерная экспериментальная (несглаженная) концентрация трассера
    QVector<QVector<qreal>> SimConc; //!< Результаты моделирования - концентрация трассера
    QVector<QString> SimMethod; //!< Название методов расчета
};

#endif // SCHEMADATA_H
