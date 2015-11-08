#include "schemacstr.h"
#include <QLinearGradient>
#include <QFont>
#include <QTimeLine>
#include <QGraphicsSceneMouseEvent>

SchemaCSTR::SchemaCSTR(int Width, int Height, int xPos, int yPos, qreal StartLevel, int Index) :
    SchemaItem(), QGraphicsPathItem(), m_Size(Width, Height),  m_PosX(xPos), m_PosY(yPos), m_numInCascade(Index)
{

    if(StartLevel!=0.0)
        m_LiquidLevel = m_LiquidLevelSet = StartLevel;
    // Initial Mixer Angle - adding some randomnicity
    m_MixerAngle=rand();
    m_isWorking = false;
    m_isReady = false;
    m_isFeeding = false;
    InletPort = new SchemaPort(Width/9,0.1*Height, this);
    OutletPort = new SchemaPort(Width,Height/3, this);

//  What color is the best?
//    m_LiquidBottomColor = QColor(Qt::darkBlue);
//    m_LiquidTopColor = QColor(Qt::blue);
    m_LiquidBottomColor.setRgb(102,204,255);
    m_LiquidTopColor = m_LiquidBottomColor.light();
//    m_LiquidTopColor.setRgb(194,235,255);
    m_GasColor = QColor(Qt::white);
//    m_GasColor = QColor::fromRgb(240,240,240);

    // Drawing Vessel
    p_path = new QPainterPath();
    p_path->lineTo(0,Height);
    p_path->quadTo(Width/2,Height*1.33,Width,Height);
    p_path->lineTo(Width,0);
    p_path->quadTo(Width/2,-Height*0.33,0,0);

    // Drawing Liquid
    p_Gradient=new QLinearGradient(p_path->boundingRect().bottomLeft(),p_path->boundingRect().topLeft());
    p_Gradient->setColorAt(1,const_cast<QColor &> (m_GasColor));
    p_Gradient->setColorAt(m_LiquidLevel,const_cast<QColor &> (m_GasColor));
    p_Gradient->setColorAt(m_LiquidLevel-1e-10,const_cast<QColor &> (m_LiquidTopColor));
    p_Gradient->setColorAt(0,const_cast<QColor &> (m_LiquidBottomColor));
    //       p_path.setFillRule(Qt::WindingFill);
    setPath(*p_path);
    setBrush(*p_Gradient);
    setPos(xPos,yPos);

    // Drawing Rings
    p_Ring1 = new QGraphicsRectItem(this);
    p_Ring1->setRect(-5,20,Width+10,5);
    p_Ring2 = new QGraphicsRectItem(this);
    p_Ring2->setRect(-5,25,Width+10,5);

    // Drawing Motor Block
    p_Motor = new QGraphicsEllipseItem(this);
    int MotorRadius = 40;
    p_Motor->setRect(QRect(Width/2-MotorRadius/2,-80,MotorRadius,MotorRadius));

    p_MotorLabel = new QGraphicsSimpleTextItem("M",p_Motor);
    p_MotorLabel->setPos(p_Motor->boundingRect().topLeft());
    p_MotorLabel->moveBy(p_MotorLabel->boundingRect().width(),0);
    QFont MotorLabelFont("Calibri", 22, QFont::Bold);
    p_MotorLabel->setFont(MotorLabelFont);

    p_Stir = new QGraphicsPathItem(p_Motor);
    p_Stirpath = new QPainterPath();
    p_Stirpath->moveTo(Width/2,-40);
    p_Stirpath->lineTo(Width/2,100);
    p_Stir->setPath(*p_Stirpath);

    p_Mixer = new QGraphicsPolygonItem( QPolygonF( QVector<QPointF>() << QPointF( 0, 0 )  << QPointF( 0, 20 ) << QPointF( 15, 10 ) << QPointF(30,20)  << QPointF(30,0) << QPointF( 15, 10 ) ), this);
    p_Mixer->setBrush( Qt::gray );
    p_Mixer->setPos(Width/2-15,100-10);
    p_Mixer->setTransform(QTransform().translate(15,0).rotate(m_MixerAngle,Qt::YAxis).translate(-15,0));

}

SchemaCSTR::~SchemaCSTR()
{

}


void SchemaCSTR::setLevel(qreal Level, int TransTime) {

    m_LiquidLevelSet = Level;
//    if(TransTime==0) TransTime = 5000;// Transient time

    QTimeLine *anim = new QTimeLine(TransTime,this);
    anim->setUpdateInterval(30);
    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (animLevel(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();

}

void SchemaCSTR::animLevel(qreal Value){

    qreal CurrentFrameLevel = m_LiquidLevel + (m_LiquidLevelSet - m_LiquidLevel) * Value;

    setBrush(Qt::white);
    delete p_Gradient;
    p_Gradient = new QLinearGradient(p_path->boundingRect().bottomLeft(),p_path->boundingRect().topLeft());
    p_Gradient->setColorAt(1,const_cast<QColor &> (m_GasColor));
    p_Gradient->setColorAt(CurrentFrameLevel,const_cast<QColor &> (m_GasColor));
    p_Gradient->setColorAt(CurrentFrameLevel-1e-10,const_cast<QColor &> (m_LiquidTopColor));
    p_Gradient->setColorAt(0,const_cast<QColor &> (m_LiquidBottomColor));
    setBrush(*p_Gradient);

    if(!m_isFeeding &&  Value > 0.8 && this != PFD->reactorItems->last()) {
        SchemaCSTR* next = static_cast<SchemaCSTR*>(Descedant);
        emit next->fill();
        m_isFeeding = true;
    }

}

void SchemaCSTR::animMotor(qreal Value)
{
    // KOCTbI/\b
    p_Mixer->setTransform(QTransform().translate(15,0).rotate(m_MixerAngle+Value*180,Qt::YAxis).translate(-15,0));
}

void SchemaCSTR::animFinished()
{
    m_LiquidLevel = m_LiquidLevelSet;
    m_isReady = true;
    if(this == PFD->reactorItems->last()) emit PFD->startSim();
    sender()->~QObject();
}

void SchemaCSTR::setSize(QSize Value)
{

}

void SchemaCSTR::setPosX(int Value)
{
    m_PosX = Value;
    setPos(m_PosX, m_PosY);
}

void SchemaCSTR::setPosY(int Value)
{
    m_PosY = Value;
    setPos(m_PosX, m_PosY);
}
void SchemaCSTR::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::DragMoveCursor));
    _startPos = event->pos();
    clicked();
}
void SchemaCSTR::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF dP =  event->pos() - _startPos;
    moveBy(dP.x(),dP.y());
    emit moved();
}
void SchemaCSTR::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
}
void SchemaCSTR::changeLevel(){
    // For testing purposes only
        setLevel(0.5, 0);
}
void SchemaCSTR::fill(){

    qreal transTime = (PFD->Tau.at(m_numInCascade)-(m_numInCascade==0 ? 0 : PFD->Tau.at(m_numInCascade-1) ) ) *3600*1000;
    qDebug() << "Flowrate is "+ QString::number(PFD->Flowrate);
    qDebug() << tr("CSTR(%1), tau = %2").arg(QString::number(m_numInCascade),QString::number(transTime));
    setLevel(0.8, static_cast<int>(transTime));
    if(!m_isWorking) activateMotor();

}

void SchemaCSTR::activateMotor()
{
    m_isWorking = true;
    QTimeLine *anim = new QTimeLine(500,this);
    anim->setLoopCount(0);
    anim->setUpdateInterval(30);
    anim->setCurveShape(QTimeLine::LinearCurve);
    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (animMotor(qreal)));
    anim->start();

}
void SchemaCSTR::startFeed() {

}
