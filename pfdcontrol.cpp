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
    qDebug() << tr("Flowrate Increased: V = %1 (waiting for %2)").arg(QString::number(m_Flowrate), QString::number(m_PlaybackFlowrate));

    if(!isFlowrateSet && fabs(m_PlaybackFlowrate - m_Flowrate) < 0.001) {
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
       if(res.first() == m_PlaybackFlowrate) {
           Tau = res; Tau.removeFirst();
       }
       result.append(res);
    }

}
void PFDControl::setFlowrate(qreal Value){
    m_Flowrate = Value;
}

void PFDControl::setPlaybackFlowrate(qreal Value)
{
    m_PlaybackFlowrate = Value;
}
void PFDControl::setNumCascade(int Value)
{
    m_NumCascade = Value;
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


void PFDControl::addPoint(qreal Time, qreal Value)
{
    m_Time.push_back(Time);
    m_Parameter.push_back(Value);
}

const QVector<qreal> &PFDControl::getTime() const
{
    return m_Time;
}

qreal PFDControl::getTimeAt(const size_t i) const
{
    return m_Time.at(i);
}


int PFDControl::getCount() const
{
    return m_Time.size();
}


const QVector<qreal> &PFDControl::getParameter() const
{
    return m_Parameter;
}


qreal PFDControl::getParameterAt(const size_t i) const
{
    return m_Parameter.at(i);
}


qreal PFDControl::getPlaybackFlowrate() const
{
    return m_PlaybackFlowrate;
}


int PFDControl::getNumCascade() const
{
    return m_NumCascade;
}


qreal PFDControl::getFlowrate() const
{
    return m_Flowrate;
}


const QString &PFDControl::getPlaybackFileName() const
{
    return m_PlaybackFileName;
}


void PFDControl::setPlaybackFileName(const QString &str)
{
    m_PlaybackFileName = str;
}
