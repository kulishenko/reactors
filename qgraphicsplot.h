#ifndef QGRAPHICSPLOT_H
#define QGRAPHICSPLOT_H
#include <QGraphicsItem>
#include <QPicture>
#include <qcustomplot/qcustomplot.h>
/*!
 * \brief Промежуточный класс, необходимый для наследования QGraphicsPlotLayoutItem от QGraphicsItem
 */
class QGraphicsPlot : public QGraphicsItem
{
public:
    QGraphicsPlot(QGraphicsItem *parent = NULL);
    ~QGraphicsPlot();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void setPlot(QCustomPlot* plot);
protected:
    QCustomPlot* p_plot;
};

#endif // QGRAPHICSPLOT_H
