#include "schemadata.h"


SchemaData::SchemaData(PFDControl* Ctrl):
    p_ExpDataTime(Ctrl->getTime()),
    p_ExpDataConductivity(Ctrl->getParameter()),
    avg_tau(nullptr), M0(nullptr)
{
    m_Flowrate = Ctrl->getPlaybackFlowrate();
    m_NumCascade = Ctrl->getNumCascade();
    m_DataRes = 1;
}

SchemaData::~SchemaData()
{

}

void SchemaData::calcM0()
{
    if(M0) delete M0;
    M0 = new qreal(0);
    for(int i = i_t0; i < p_ExpDataTime.size(); i += m_DataRes)
        *M0 += SConc.at(i - i_t0) * dt(i - i_t0);
}
/*!
 * \brief Возвращает значение момента нулевого порядка
 * Запускает расчет, если необходимо
 * \return Значение момента нулевого порядка
 */
qreal SchemaData::getM0()
{
    if(!M0) calcM0();
    return (*M0);
}

/*!
 * \brief Возвращает значение среднего времени пребывания
 * Запускает расчет, если необходимо
 * \return Значение среднего времени пребывания, с
 */
qreal SchemaData::getAvgTau()
{
    if(!avg_tau) calcAvgTau();
    return (*avg_tau);
}

/*!
 * \brief Возвращает значение расчетного времени пребывания
 * \return Значение расчетного времени пребывания, с
 */
qreal SchemaData::getTau() const
{
    return m_tau;
}
/*!
 * \brief Возвращает значение числа элементов ячеечной модели
 * \return Число элементов ячеечной модели
 */
qreal SchemaData::getNc() const
{
    return Nc;
}

qreal SchemaData::getM2theta() const
{
    return M2theta;
}

qreal SchemaData::getSigma2theta() const
{
    return sigma2theta;
}
/*!
 * \brief Рассчитывает среднее время пребывания вещества в аппарате по вероятностным характеристикам
 */
void SchemaData::calcAvgTau()
{

    qreal Sum_tC = 0;
    for(int i = i_t0; i < p_ExpDataTime.size(); i += m_DataRes) {
        Sum_tC += SConc.at(i - i_t0) * (p_ExpDataTime.at(i)- t0) * dt(i - i_t0);
    }    
    if(avg_tau) delete avg_tau;
    avg_tau = new qreal(Sum_tC / getM0());
}
/*!
 * \brief Рассчитывает массив значений концетрации трассера в моль/л на основе данных об электропроводности в мСм/см
 */
void SchemaData::calcConc()
{
    t0 = *t_0();
    i_t0 = t_0() - p_ExpDataTime.constBegin();
    qreal C1 = Calibrate(p_ExpDataConductivity.at(i_t0));
    tend = *t_last();
    qreal C2 = Calibrate(p_ExpDataConductivity.at(t_last() - p_ExpDataTime.constBegin()));

    qDebug() << QObject::tr("C1 = %1, C2 = %2").arg(QString::number(C1),QString::number(C2));
    qreal R = (C1 - C2) / (tend - t0);

    for(int i = i_t0; i < p_ExpDataTime.size(); i += m_DataRes){
        qreal C = Calibrate(p_ExpDataConductivity.at(i)) - C1 - (p_ExpDataTime.at(i) - t0)* R;
        // Added for robustness (check approximation formulas?)
        Conc.push_back((C > 0) ? C : 0);
    }

}

/*!
 * \brief Рассчитывает массив значений безразмерной концентрации (сглаженные данные)
 */
void SchemaData::calcDimConc()
{
    qreal tau = getAvgTau();
    for(int i = 0; i < SConc.size(); i += m_DataRes)
        DimConc.push_back(SConc.at(i) * tau / getM0());
}

/*!
 * \brief Рассчитывает массив значений безразмерной концентрации (экспериментальные данные)
 */
void SchemaData::calcDimExpConc()
{
    qreal tau = getAvgTau();
    for(int i = 0; i < Conc.size(); i += m_DataRes)
        DimExpConc.push_back(Conc.at(i) * tau / getM0());
}

/*!
 * \brief Рассчитывает массив безразмерного времени
 */
void SchemaData::calcDimTime()
{
    m_tau = 0.84 * m_NumCascade / m_Flowrate * 3600; // ToDo: remove?
    qreal tau = getAvgTau();
    for(int i = i_t0; i < p_ExpDataTime.size(); i+= m_DataRes)
        DimTime.push_back((p_ExpDataTime.at(i) - t0) / tau + 1e-6);

}

/*!
 * \brief Рассчитывает значение безразмерного центрального момента 2го порядка, а также его дисперсию
 */
void SchemaData::calcM2theta()
{
    M2theta = 0;
    qreal tau = getAvgTau();
    for(int i = 0; i < DimTime.size(); i += m_DataRes) {
        M2theta +=  DimTime.at(i) *  DimTime.at(i) * DimConc.at(i) * dim_dt(i);
    }

    M2theta *= tau * tau;
    sigma2theta = M2theta / tau / tau - 1;
    Nc = 1 / sigma2theta;

}

/*!
 * \brief Распознает отрицательную ступень в экспериментальных данных (подключение шунта)
 * \return Константный итератор, указывающий на время начала эксперимента
 */
QVector<qreal>::const_iterator SchemaData::t_0() const
{
    // Simple method to detect the bypass resistor disconnection time
    int i = 0, j;
    QVector<qreal>::const_iterator iter = p_ExpDataTime.constBegin();
    while(i+5 < p_ExpDataTime.size()) {
        i++; ++iter;
        if((p_ExpDataConductivity.first() - p_ExpDataConductivity.at(i)) > p_ExpDataConductivity.first() * 0.1
                && fabs(p_ExpDataConductivity.at(i) - p_ExpDataConductivity.at(i+5)) < p_ExpDataConductivity.at(i) * 0.03)
        {
            j = i;
            qDebug() << "t0_begin = " + QString::number(p_ExpDataTime.at(j));
            while(fabs(p_ExpDataConductivity.first() - p_ExpDataConductivity.at(j)) > p_ExpDataConductivity.first() * 0.1
                    && j < p_ExpDataTime.size()){
                    j++; ++iter;}
            qDebug() << "t0_end = " + QString::number(p_ExpDataTime.at(j));
            return iter;
        }
    }
    qDebug() << "Couldn't detect t0, using t0 = 0 instead";
    return p_ExpDataTime.constBegin();
}

QVector<qreal>::const_iterator SchemaData::t_last() const
{
 //   p_ExpDataTime.pop_back();
    return p_ExpDataTime.constEnd() - 1;
}

qreal SchemaData::dt(const size_t i) const
{
    if(i == 0)
        return p_ExpDataTime.at(m_DataRes) - p_ExpDataTime.first();
    else
        return p_ExpDataTime.at(i) - p_ExpDataTime.at(i - m_DataRes);
}
qreal SchemaData::dim_dt(const size_t i) const
{
    if(i == 0)
        return DimTime.at(m_DataRes) - DimTime.first();
    else
        return DimTime.at(i) - DimTime.at(i - m_DataRes);
}
qreal SchemaData::Calibrate(qreal x) const
{
    x = (x < 10) ? x * 1000: x;
  //  x *= 1000; // Conversion to mkS/cm
    return 2.3480623E-18 * pow(x, 5) - 1.3123250E-14 * pow(x, 4) + 2.7014011E-11 * pow(x, 3)
            - 2.4703301E-08 * x * x + 1.7735139E-05 * x + 1e-18;
}

/*!
 * \brief Сглаживает экспериментальные данные медианным фильтром и фильтром скользящего среднего и формирует соответствующий массив
 */
void SchemaData::SmoothData()
{
    //  Simple median filter
    int width = 8;
    for(int i = 0; i < Conc.size(); i++) {
        if(i == Conc.size()-width) width--;
        QVector<qreal> window;
        for(int j=i; j<=i+width;j++)
            window.push_back(Conc.at(j));
        qSort(window);
        if(width % 2 == 0)
            SConc.push_back((window.at(ceil(width/2)) + window.at(floor(width/2))) / 2);
        else
            SConc.push_back(window.at(floor(width/2) + 1));
    }
    // Simple low-pass filter
    qreal A = 0.3;
    for(int i = 1; i < SConc.size(); i++){
        SConc.replace(i, SConc.at(i-1) + A * (SConc.at(i) - SConc.at(i-1)));
    }
}

qreal SchemaData::getConcAt(const size_t i) const
{
    return Conc.at(i);
}

qreal SchemaData::getSConcAt(const size_t i) const
{
    return SConc.at(i);
}

qreal SchemaData::getDimExpConcAt(const size_t i) const
{
    return DimExpConc.at(i);
}

void SchemaData::setSimMethod(QString name)
{
    if(name.isEmpty())
        name.append(QObject::tr("Simulated by method %1").arg(SimMethod.size()));

    SimMethod.push_back(name);
}

const QString &SchemaData::getSimMethod(const int i) const
{
    return SimMethod.at(i);
}

void SchemaData::addSimData(const QVector<qreal> &simData, const QString &name)
{
    SimConc.push_back(simData);
    setSimMethod(name);
}

qreal SchemaData::getDimTimeAt(const size_t i) const
{
    return DimTime.at(i);
}

const QVector<qreal> &SchemaData::getSConc() const
{
    return SConc;
}

const QVector<qreal> &SchemaData::getDimConc() const
{
    return DimConc;
}

const QVector<qreal> &SchemaData::getDimExpConc() const
{
    return DimExpConc;
}

const QVector<qreal> &SchemaData::getDimTime() const
{
    return DimTime;
}

const QVector<qreal> &SchemaData::getSimConc(const size_t i) const
{
    return SimConc.at(i);
}

size_t SchemaData::getSimConcCount() const
{
    return SimConc.size();
}
