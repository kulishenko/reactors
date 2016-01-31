#ifndef SCHEMAVIEW_H
#define SCHEMAVIEW_H
#include <QGraphicsView>
#include <QTimeLine>
#include <QWheelEvent>
#include "schemaevent.h"
#include "schemaitem.h"

/*!
 * \brief Класс, предназначенный для вывода сцены
 */
class SchemaView : public QGraphicsView
{
    Q_OBJECT
public:
    SchemaView(QWidget* parent = 0);
    ~SchemaView();
private:
    int _numScheduledScalings;
    qreal scale_factor; //!< Масштаб окна просмотра
    QPoint lastPos;
    SchemaItem *p_ItemFrom, *p_ItemTo;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent (QWheelEvent * event);
public slots:
    void animFinished();
    void scalingTime(qreal x);
};

#endif // SCHEMAVIEW_H
