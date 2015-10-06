#include "schemadata.h"
#include <stdio.h>
#include <gsl/gsl_fit.h>

SchemaData::SchemaData(PFDControl* Ctrl)
{
    ExpDataTime = &Ctrl->Time;
    ExpDataConductivity = &Ctrl->Conductivity;
    Flowrate = &Ctrl->PlaybackFlowrate;
}

SchemaData::~SchemaData()
{

}

void SchemaData::calcConc()
{
    t0 = *t_0();
    qreal C1 = Calibrate(ExpDataConductivity->at(ExpDataTime->indexOf( t0 )));
    tend = *t_last();
    qreal C2 = Calibrate(ExpDataConductivity->at(ExpDataTime->indexOf( tend )));

    qDebug() << QObject::tr("C1 = %1, C2 = %2").arg(QString::number(C1),QString::number(C2));
    qreal R = (C1-C2) / (tend - t0);

    for(int i=ExpDataTime->indexOf( t0 ); i<ExpDataTime->size();i++){
        Conc.push_back(Calibrate(ExpDataConductivity->at(i)) - C1 - (ExpDataTime->at(i) - t0)* R);
    //    qDebug() << QString::number(Conc.last());
    }

}

void SchemaData::calcDimConc()
{

}

void SchemaData::calcDimTime()
{
    qreal tau = 0.84*5 / *Flowrate * 3600;
    for(int i=ExpDataTime->indexOf( t0 ); i<ExpDataTime->size();i++){
        DimTime.push_back((ExpDataTime->at(i)- t0)/tau);
  //      qDebug() << DimTime.last();
    }
}

void SchemaData::estimateNumCells()
{
    int i, n = 4;
      double x[4] = { 1970, 1980, 1990, 2000 };
      double y[4] = {   12,   11,   14,   13 };
      double w[4] = {  0.1,  0.2,  0.3,  0.4 };

      double c0, c1, cov00, cov01, cov11, chisq;

      gsl_fit_wlinear (x, 1, w, 1, y, 1, n,
                       &c0, &c1, &cov00, &cov01, &cov11,
                       &chisq);

      qDebug() << QObject::tr("# best fit: Y = %1 + %2 X\n").arg(QString::number(c0), QString::number(c1));
      qDebug() << QObject::tr("# covariance matrix:\n");
//      printf("# [ %g, %g\n#   %g, %g]\n",
//              cov00, cov01, cov01, cov11);
      qDebug() << QObject::tr("# chisq = %1\n").arg(chisq);

      for (i = 0; i < n; i++)
        qDebug() << QObject::tr("data: %1 %2 %3\n").arg(
                       QString::number(x[i]), QString::number(y[i]),
                        QString::number(1/sqrt(w[i])));

      qDebug() << QObject::tr("\n");

      for (i = -30; i < 130; i++)
        {
          double xf = x[0] + (i/100.0) * (x[n-1] - x[0]);
          double yf, yf_err;

          gsl_fit_linear_est (xf,
                             c0, c1,
                             cov00, cov01, cov11,
                             &yf, &yf_err);

     //     qDebug() << QObject::tr("fit: %g %g\n", xf, yf);
     //     qDebug() << QObject::tr("hi : %g %g\n", xf, yf + yf_err);
      //    qDebug() << QObject::tr("lo : %g %g\n", xf, yf - yf_err);
        }
}
qreal* SchemaData::t_0()
{
    // Simple method to detect the bypass resistor disconnection time
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

qreal* SchemaData::t_last()
{
    ExpDataTime->pop_back();
    return &ExpDataTime->last();
}
qreal SchemaData::Calibrate(qreal x){
    x *= 1000; // Conversion to mkS/cm
    return 2.3480623E-18 * pow(x, 5) - 1.3123250E-14 * pow(x, 4) + 2.7014011E-11 * pow(x, 3)
           - 2.4703301E-08 * x * x + 1.7735139E-05 * x;
}

