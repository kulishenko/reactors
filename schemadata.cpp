#include "schemadata.h"


SchemaData::SchemaData(PFDControl* Ctrl)
{
    p_ExpDataTime = &Ctrl->Time;
    p_ExpDataConductivity = &Ctrl->Conductivity;
    p_Flowrate = &Ctrl->PlaybackFlowrate;
    p_NumCascade = &Ctrl->NumCascade;
    m_DataRes = 1;
}

SchemaData::~SchemaData()
{

}

void SchemaData::calcAvgTau()
{
    qreal Sum_C = 0, Sum_tC = 0;

    for(int i = i_t0; i<p_ExpDataTime->size();i+=m_DataRes) {
        Sum_tC += SConc.at(i - i_t0) * (p_ExpDataTime->at(i)- t0) * dt(i);
        Sum_C += SConc.at(i - i_t0) * dt(i);

    }
    avg_tau = Sum_tC / Sum_C;
}

void SchemaData::calcConc()
{
    t0 = *t_0();
    i_t0 = t_0() - p_ExpDataTime->begin();
    qreal C1 = Calibrate(p_ExpDataConductivity->at(i_t0));
    tend = *t_last();
    qreal C2 = Calibrate(p_ExpDataConductivity->at(t_last()-p_ExpDataTime->begin()));

    qDebug() << QObject::tr("C1 = %1, C2 = %2").arg(QString::number(C1),QString::number(C2));
    qreal R = (C1 - C2) / (tend - t0);

    for(int i= i_t0; i<p_ExpDataTime->size();i+=m_DataRes){
        qreal C = Calibrate(p_ExpDataConductivity->at(i)) - C1 - (p_ExpDataTime->at(i) - t0)* R;
        // Added for robustness (check approximation formulas?)
        Conc.push_back((C > 0) ? C : 0);
    }

}

void SchemaData::calcDimConc()
{
    qreal Sum_C = 0;

    for(int i = i_t0; i<p_ExpDataTime->size();i+=m_DataRes) {
        Sum_C += SConc.at(i - i_t0) * dt(i);
    }
    for(int i = i_t0; i<p_ExpDataTime->size();i+=m_DataRes)
        DimConc.push_back(SConc.at(i - i_t0) / Sum_C);
}

void SchemaData::calcDimTime()
{
    tau = 0.84 * (*p_NumCascade) / (*p_Flowrate) * 3600;
    for(int i = i_t0; i<p_ExpDataTime->size();i+=m_DataRes)
        DimTime.push_back((p_ExpDataTime->at(i)- t0)/avg_tau + 1e-6);

}

void SchemaData::calcM2t()
{
    M2t = 0;
    /*for(int i = 0; i<DimTime.size();i+=m_DataRes) {
        M2t += DimTime.at(i) * DimTime.at(i) * DimConc.at(i) * dim_dt(i);
    }*/
    for(int i = i_t0; i<p_ExpDataTime->size();i+=m_DataRes) {
        M2t += p_ExpDataTime->at(i) * p_ExpDataTime->at(i) * SConc.at(i - i_t0) * dt(i);
    }
    qreal sigma_theta_2 = M2t / avg_tau - 1;
    Nc = 1 / sigma_theta_2;

    // Temp
    qreal sigma_theta_22 = 0, t2_C = 0, C = 0;
    for(int i = i_t0; i<p_ExpDataTime->size();i+=m_DataRes) {
        t2_C += p_ExpDataTime->at(i) * p_ExpDataTime->at(i) * SConc.at(i - i_t0) * dt(i);
        C += Conc.at(i - i_t0) * dt(i);
    }
    sigma_theta_22 = t2_C / (C * avg_tau * avg_tau) - 1;
    qDebug() << sigma_theta_22;
    qDebug() << QString("N = %1").arg(1/sigma_theta_22);
//    Nc = 1/sigma_theta_22;

}

QVector<qreal>::const_iterator SchemaData::t_0() const
{
    // Simple method to detect the bypass resistor disconnection time
    int i = 0, j;
    QVector<qreal>::const_iterator iter = p_ExpDataTime->begin();
    while(i+5 < p_ExpDataTime->size()) {
        i++; iter++;
        if((p_ExpDataConductivity->first() - p_ExpDataConductivity->at(i)) > p_ExpDataConductivity->first() * 0.1
                && fabs(p_ExpDataConductivity->at(i) - p_ExpDataConductivity->at(i+5)) < p_ExpDataConductivity->at(i) * 0.03)
        {
            j = i;
            qDebug() << "t0_begin = " + QString::number(p_ExpDataTime->at(j));
            while(fabs(p_ExpDataConductivity->first() - p_ExpDataConductivity->at(j)) > p_ExpDataConductivity->first() * 0.1
                    && j < p_ExpDataTime->size()){
                    j++; iter++;}
            qDebug() << "t0_end = " + QString::number(p_ExpDataTime->at(j));
            return iter;
        }
    }
    qDebug() << "Couldn't detect t0, using t0 = 0 instead";
    return p_ExpDataTime->begin();
}

QVector<qreal>::const_iterator SchemaData::t_last() const
{
    p_ExpDataTime->pop_back();
    return p_ExpDataTime->end() - 1;
}

qreal SchemaData::dt(int i)
{
    if(i == 0)
        return p_ExpDataTime->at(m_DataRes) - p_ExpDataTime->at(0);
    else
        return p_ExpDataTime->at(i) - p_ExpDataTime->at(i-m_DataRes);
}
qreal SchemaData::dim_dt(int i)
{
    if(i == 0)
        return DimTime.at(m_DataRes) - DimTime.at(0);
    else
        return DimTime.at(i) - DimTime.at(i-m_DataRes);
}
qreal SchemaData::Calibrate(qreal x){
    x *= 1000; // Conversion to mkS/cm
    return 2.3480623E-18 * pow(x, 5) - 1.3123250E-14 * pow(x, 4) + 2.7014011E-11 * pow(x, 3)
            - 2.4703301E-08 * x * x + 1.7735139E-05 * x + 1e-18;
}


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
    for(int i = 1; i<SConc.size();i++){
        SConc.replace(i, SConc.at(i-1) + A*(SConc.at(i)-SConc.at(i-1)));
    }


}
