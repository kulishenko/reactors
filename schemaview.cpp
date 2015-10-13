#include "schemaview.h"
#include <QDebug>

SchemaView::SchemaView() : QGraphicsView()
{
    _numScheduledScalings = 0;
    scale_factor = 1.0;
}

SchemaView::~SchemaView()
{

}
void SchemaView::mousePressEvent(QMouseEvent *event)
{
      //  qDebug() << "Custom view clicked.";
    setDragMode(QGraphicsView::ScrollHandDrag);
    lastPos = event->globalPos();
    //event->accept();
    QGraphicsView::mousePressEvent(event);
}

void SchemaView::mouseReleaseEvent(QMouseEvent *event)
{
    setDragMode(QGraphicsView::NoDrag);
    update();
    QGraphicsView::mouseReleaseEvent(event);
}
void SchemaView::mouseMoveEvent(QMouseEvent *event){
    float dx = (event->globalPos().x()-lastPos.x());
    float dy = (event->globalPos().y()-lastPos.y());

    if(event->buttons() == Qt::LeftButton){
       QPointF viewCenter = mapToScene(width() / 2, height() / 2);
       viewCenter -= QPointF(dx, dy);
     //  translate(dx,dy);
      centerOn(viewCenter);
       update();
    }
    lastPos = event->globalPos();
   // QGraphicsView::mouseMoveEvent(event);
}
void SchemaView::wheelEvent(QWheelEvent *event) {

    if(event->modifiers().testFlag(Qt::ControlModifier)) {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15; // see QWheelEvent documentation
        _numScheduledScalings += numSteps;
        if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
        _numScheduledScalings = numSteps;

        QTimeLine *anim = new QTimeLine(350, this);
        anim->setUpdateInterval(20);

        connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
        connect(anim, SIGNAL (finished()), SLOT (animFinished()));
        anim->start();
     }
}
void SchemaView::scalingTime(qreal x)
{
        qreal factor = 1.0+ qreal(_numScheduledScalings) / 300.0;
        scale(factor, factor);
}
void SchemaView::animFinished()
{
        if (_numScheduledScalings > 0)
        _numScheduledScalings--;
        else
        _numScheduledScalings++;
        sender()->~QObject();
}
