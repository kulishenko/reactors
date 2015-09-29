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
    qDebug() << "tick()";
    if(isStarted) {
        TimeNow += 1.0;
        doSim();
    }
}
void PFDControl::test(){
    qDebug() << "test()";
}
void PFDControl::flowrate_increase(){
    qDebug() << "Flowrate Increased";
    if(!isStarted) {
    setLevel1();
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
