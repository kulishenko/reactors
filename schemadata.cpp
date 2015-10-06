#include "schemadata.h"

SchemaData::SchemaData(PFDControl* Ctrl)
{
    ExpDataTime = &Ctrl->Time;
    ExpDataConductivity = &Ctrl->Conductivity;
}

SchemaData::~SchemaData()
{

}

void SchemaData::calcConc()
{
    qreal t0 = *t_0();

    qreal C1= Calibrate(ExpDataConductivity->at(ExpDataTime->indexOf( t0 )));

    qreal tend = *t_last() ;

    qreal C2 = Calibrate(ExpDataConductivity->at(ExpDataTime->indexOf( tend )));

    qDebug() << QObject::tr("C1 = %1, C2 = %2").arg(QString::number(C1),QString::number(C2));

    qreal R = (C1-C2)/tend;

    for(int i=0; i<ExpDataTime->size();i++){
        Conc.push_back(Calibrate(ExpDataConductivity->at(i)) - C1 - ExpDataTime->at(i) * R);
        qDebug() << QString::number(Conc.at(i));
    }

}

void SchemaData::calcDimConc()
{

}

void SchemaData::calcDimTime()
{

}

qreal* SchemaData::t_0()
{
    return &ExpDataTime->first();
}

qreal* SchemaData::t_last()
{
    ExpDataTime->pop_back();
    return &ExpDataTime->last();
}
qreal SchemaData::Calibrate(qreal x){
    return 2.3480623E-18 * pow(x,5) - 1.3123250E-14 * pow(x,4) + 2.7014011E-11*pow(x,3)
           - 2.4703301E-08 * x * x + 1.7735139E-05 * x;
}

