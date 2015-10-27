#include "schemadata.h"


SchemaData::SchemaData(PFDControl* Ctrl)
{
    ExpDataTime = &Ctrl->Time;
    ExpDataConductivity = &Ctrl->Conductivity;
    Flowrate = &Ctrl->PlaybackFlowrate;
    NumCascade = &Ctrl->NumCascade;
    DataRes = 1;
}

SchemaData::~SchemaData()
{

}

void SchemaData::calcAvgTau()
{
    qreal Sum_C = 0, Sum_tC = 0, dt;

    for(int i = i_t0; i<ExpDataTime->size();i+=DataRes) {
        if(i == 0)
            dt = ExpDataTime->at(DataRes) - ExpDataTime->at(0);
        else            
            dt = ExpDataTime->at(i) - ExpDataTime->at(i-DataRes);
        Sum_tC += Conc.at(i - i_t0) * (ExpDataTime->at(i)- t0) * dt;
        Sum_C += Conc.at(i - i_t0) * dt;

    }
    avg_tau = Sum_tC / Sum_C;
}

void SchemaData::calcConc()
{
    t0 = *t_0();
    i_t0 = ExpDataTime->indexOf( t0 );
    qreal C1 = Calibrate(ExpDataConductivity->at(i_t0));
    tend = *t_last();
    qreal C2 = Calibrate(ExpDataConductivity->at(ExpDataTime->indexOf( tend )));

    qDebug() << QObject::tr("C1 = %1, C2 = %2").arg(QString::number(C1),QString::number(C2));
    qreal R = (C1 - C2) / (tend - t0);

    for(int i= i_t0; i<ExpDataTime->size();i+=DataRes){
        qreal C = Calibrate(ExpDataConductivity->at(i)) - C1 - (ExpDataTime->at(i) - t0)* R;
        // Added for robustness (check approximation formulas?)
        Conc.push_back((C > 0) ? C : 0);
    }

}

void SchemaData::calcDimConc()
{

}

void SchemaData::calcDimTime()
{
    tau = 0.84 * (*NumCascade) / (*Flowrate) * 3600;
    for(int i = i_t0; i<ExpDataTime->size();i+=DataRes)
        DimTime.push_back((ExpDataTime->at(i)- t0)/tau + 1e-6);

}


qreal* SchemaData::t_0() const
{
    // Simple method to detect the bypass resistor disconnection time
    // ToDo: FIX 1-50-1.xls file recognition
    int i = 0, j;
    while(i+5 < ExpDataTime->size()) {
        i++;
        if((ExpDataConductivity->first() - ExpDataConductivity->at(i)) > ExpDataConductivity->first() * 0.1
                && fabs(ExpDataConductivity->at(i) - ExpDataConductivity->at(i+5)) < ExpDataConductivity->at(i) * 0.03)
        {
            j = i;
            qDebug() << "t0_begin = " + QString::number(ExpDataTime->at(j));
            while(fabs(ExpDataConductivity->first() - ExpDataConductivity->at(j)) > ExpDataConductivity->first() * 0.1
                    && j < ExpDataTime->size())
                    j++;
            qDebug() << "t0_end = " + QString::number(ExpDataTime->at(j));
            return const_cast<qreal *> (&ExpDataTime->at(j));
        }
    }
    qDebug() << "Couldn't detect t0, using t0 = 0 instead";
    return &ExpDataTime->first();
}

qreal* SchemaData::t_last() const
{
    ExpDataTime->pop_back();
    return &ExpDataTime->last();
}
qreal SchemaData::Calibrate(qreal x){
    x *= 1000; // Conversion to mkS/cm
    return 2.3480623E-18 * pow(x, 5) - 1.3123250E-14 * pow(x, 4) + 2.7014011E-11 * pow(x, 3)
            - 2.4703301E-08 * x * x + 1.7735139E-05 * x + 1e-18;
}


void SchemaData::SmoothData()
{
    //  Simple median filter (fix N = N - 8 ?)
    int init_width = 8;
    int width = init_width;
    for(int i = 0; i<Conc.size(); i++) {
        if(i == Conc.size()-width) width--;
        QVector<qreal> window;
        // Get n=width points
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
