#ifndef SCHEMAVIEW_H
#define SCHEMAVIEW_H
#include <QGraphicsView>
#include <QTimeLine>
#include <QWheelEvent>

class SchemaView : public QGraphicsView
{
    Q_OBJECT
    int _numScheduledScalings;
    void wheelEvent (QWheelEvent * event);
public:
    SchemaView();
    ~SchemaView();
    qreal scale_factor;
    QPoint lastPos;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
public slots:
    void animFinished();
    void scalingTime(qreal x);
};

#endif // SCHEMAVIEW_H
