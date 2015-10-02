#include "pfdcontrol.h"

PFDControl::PFDControl(QObject *parent) : QObject(parent)
{
    TimeNow=0;
    isStarted = false;
}

PFDControl::~PFDControl()
{

}
void PFDControl::playback(QGraphicsSceneMouseEvent *event){
    qDebug() << "bebebe";
}
void PFDControl::tick(){
    if(isStarted) {
        TimeNow += 1.0f;
        doSim();
    }
}
void PFDControl::flowrate_increase(){
    qDebug() << "Flowrate Increased";
    if(!isStarted) {
        calcTau();
        setLevel();
        startSim();
    }

}
void PFDControl::flowrate_decrease(){
    qDebug() << "Flowrate Decreased";
}

void PFDControl::Start()
{
    isStarted=true;
}
void PFDControl::calcTau(){

    // Get experimental data from file
    QFile file(":/resources/tau.csv");

    if(false == file.open(QIODevice::ReadOnly))
      qDebug() << file.fileName();
    QTextStream stream(&file);
    QString str = stream.readAll();
    str.remove(QChar(' '));
    str.remove(QChar('\t'));
    QStringList strings = str.split("\n");
    QList<QVector<qreal> > result;
    foreach(const QString & curStr, strings){
       QStringList items = curStr.split(",");
       QVector<double> res;
       foreach(const QString &item, items){
           res.push_back(item.toDouble());
       }
       if(res.first()==Flowrate) {
           Tau = res; Tau.removeFirst();
       }
       result.append(res);
    }


 /*   Tau = result.at(5);
    Tau.removeFirst(); */

   // qDebug() << result;

}
void PFDControl::setFlowrate(qreal Value){
    Flowrate = Value;
}
