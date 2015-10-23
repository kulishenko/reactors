#include "schemacstr.h"
#include <QLinearGradient>
#include <QFont>
#include <QTimeLine>


SchemaCSTR::SchemaCSTR(int Height, int Width , int xPos, int yPos, qreal StartLevel, int Index)
{
    if(StartLevel!=0.0)
        LiquidLevel = LiquidLevelSet = StartLevel;
    // Initial Mixer Angle - adding some randomnicity
    MixerAngle=rand();
    isWorking = false;
    isReady = false;
    isFeeding = false;
    numInCascade = Index;
    InletPort = new SchemaPort(10+xPos,yPos+10, this);
    OutletPort = new SchemaPort(Width+xPos,yPos+40, this);

//  What color is the best?
//    LiquidBottomColor = QColor(Qt::darkBlue);
//    LiquidTopColor = QColor(Qt::blue);
    LiquidBottomColor.setRgb(102,204,255);
    LiquidTopColor = LiquidBottomColor.light();
//    LiquidTopColor.setRgb(194,235,255);
    GasColor = QColor(Qt::white);
//    GasColor = QColor::fromRgb(240,240,240);

    // Drawing Vessel
    path = new QPainterPath();
    path->lineTo(0,Height);
    path->quadTo(Width/2,Height+40,Width,Height);
    path->lineTo(Width,0);
    path->quadTo(Width/2,-40,0,0);

    // Drawing Liquid
    Gradient=new QLinearGradient(path->boundingRect().bottomLeft(),path->boundingRect().topLeft());
    Gradient->setColorAt(1,const_cast<QColor &> (GasColor));
    Gradient->setColorAt(LiquidLevel,const_cast<QColor &> (GasColor));
    Gradient->setColorAt(LiquidLevel-1e-10,const_cast<QColor &> (LiquidTopColor));
    Gradient->setColorAt(0,const_cast<QColor &> (LiquidBottomColor));
    //       path.setFillRule(Qt::WindingFill);
    setPath(*path);
    setBrush(*Gradient);
    setPos(xPos,yPos);

    // Drawing Rings
    Ring1 = new QGraphicsRectItem(this);
    Ring1->setRect(-5,20,Width+10,5);
    Ring2 = new QGraphicsRectItem(this);
    Ring2->setRect(-5,25,Width+10,5);

    // Drawing Motor Block
    Motor = new QGraphicsEllipseItem(this);
    int MotorRadius = 40;
    Motor->setRect(QRect(Width/2-MotorRadius/2,-80,MotorRadius,MotorRadius));

    MotorLabel = new QGraphicsSimpleTextItem("M",Motor);
    MotorLabel->setPos(Motor->boundingRect().topLeft());
    MotorLabel->moveBy(MotorLabel->boundingRect().width(),0);
    QFont MotorLabelFont("Calibri", 22, QFont::Bold);
    MotorLabel->setFont(MotorLabelFont);
//    MotorLabel->


    Stir = new QGraphicsPathItem(Motor);
    Stirpath = new QPainterPath();
    Stirpath->moveTo(Width/2,-40);
    Stirpath->lineTo(Width/2,100);
    Stir->setPath(*Stirpath);


    Mixer = new QGraphicsPolygonItem( QPolygonF( QVector<QPointF>() << QPointF( 0, 0 )  << QPointF( 0, 20 ) << QPointF( 15, 10 ) << QPointF(30,20)  << QPointF(30,0) << QPointF( 15, 10 ) ), this);
 //   Mixer->setPen( QPen(Qt::black) );
    Mixer->setBrush( Qt::gray );
    Mixer->setPos(Width/2-15,100-10);
    Mixer->setTransform(QTransform().translate(15,0).rotate(MixerAngle,Qt::YAxis).translate(-15,0));

}

SchemaCSTR::~SchemaCSTR()
{

}


void SchemaCSTR::setLevel(qreal Level, int TransTime) {

    LiquidLevelSet = Level;
//    if(TransTime==0) TransTime = 5000;// Transient time

    QTimeLine *anim = new QTimeLine(TransTime,this);
    anim->setUpdateInterval(30);
    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (animLevel(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();

}

void SchemaCSTR::animLevel(qreal Value){

    qreal CurrentFrameLevel = LiquidLevel + (LiquidLevelSet - LiquidLevel) * Value;

    setBrush(Qt::white);
    delete Gradient;
    Gradient = new QLinearGradient(path->boundingRect().bottomLeft(),path->boundingRect().topLeft());
    Gradient->setColorAt(1,const_cast<QColor &> (GasColor));
    Gradient->setColorAt(CurrentFrameLevel,const_cast<QColor &> (GasColor));
    Gradient->setColorAt(CurrentFrameLevel-1e-10,const_cast<QColor &> (LiquidTopColor));
    Gradient->setColorAt(0,const_cast<QColor &> (LiquidBottomColor));
    setBrush(*Gradient);

    if(!isFeeding &&  Value > 0.8 && this != PFD->reactorItems->last()) {
        SchemaCSTR* next = static_cast<SchemaCSTR*>(Descedant);
        emit next->fill();
        isFeeding = true;
    }

}

void SchemaCSTR::animMotor(qreal Value)
{
    // KOCTbI/\b
    Mixer->setTransform(QTransform().translate(15,0).rotate(MixerAngle+Value*180,Qt::YAxis).translate(-15,0));
}

void SchemaCSTR::animFinished()
{
    LiquidLevel = LiquidLevelSet;
    isReady = true;
    if(this == PFD->reactorItems->last()) emit PFD->startSim();
    sender()->~QObject();
}
void SchemaCSTR::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    qDebug() << "Custom item clicked.";
    // setLevel(0.5);

    clicked();
}

void SchemaCSTR::changeLevel(){
        setLevel(0.5, 0);
}
void SchemaCSTR::fill(){
    // Sorry about that... It was lazy to do it via SIGNAL-SLOT
       // PFDControl* Ctrl = static_cast <PFDControl *> (sender());

        qreal transTime = (PFD->Tau.at(numInCascade)-(numInCascade==0 ? 0 : PFD->Tau.at(numInCascade-1) ) ) *3600*1000;
        qDebug() << "Flowrate is "+ QString::number(PFD->Flowrate);
        qDebug() << tr("CSTR(%1), tau = %2").arg(QString::number(numInCascade),QString::number(transTime));
        setLevel(0.8, (int)transTime);
        if(!isWorking) activateMotor();

}

void SchemaCSTR::activateMotor()
{
    isWorking = true;
    QTimeLine *anim = new QTimeLine(500,this);
    anim->setLoopCount(0);
    anim->setUpdateInterval(30);
    anim->setCurveShape(QTimeLine::LinearCurve);
    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (animMotor(qreal)));
    anim->start();

}
void SchemaCSTR::startFeed() {

}
