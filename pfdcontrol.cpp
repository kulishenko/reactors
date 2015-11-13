#include "pfdcontrol.h"

PFDControl::PFDControl(QObject *parent) : QObject(parent)
{
    TimeNow = 0;
    isStarted = false;
    isFlowrateSet = false;
}

PFDControl::~PFDControl()
{

}
void PFDControl::tick(){
    if(isStarted) {
        TimeNow += 1.0f;
        emit doSim();
    }
}
void PFDControl::flowrate_increase(){
    qDebug() << tr("Flowrate Increased: V = %1 (waiting for %2)").arg(QString::number(Flowrate), QString::number(PlaybackFlowrate));

    if(!isFlowrateSet && fabs(PlaybackFlowrate - Flowrate) < 0.001) {
        isFlowrateSet = true;
        emit setLevel();
  //      emit startSim(); Moved to SchemaCSTR::animFinished()
    }

}
void PFDControl::flowrate_decrease(){
    qDebug() << "Flowrate Decreased";
}

void PFDControl::Start()
{
    isStarted = true;
    emit started();
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
       if(res.first()==PlaybackFlowrate) {
           Tau = res; Tau.removeFirst();
       }
       result.append(res);
    }

}
void PFDControl::setFlowrate(qreal Value){
    Flowrate = Value;
}

void PFDControl::setPlaybackFlowrate(qreal Value)
{
    PlaybackFlowrate = Value;
}
void PFDControl::setNumCascade(int Value)
{
    NumCascade = Value;
}
void PFDControl::addItem(SchemaItem *item)
{
    item->PFD = this;
}

qreal PFDControl::getCurrentTime()
{
    return TimeNow;
}


qreal PFDControl::getTauAt(const size_t i) const
{
    return Tau.at(i);
}
