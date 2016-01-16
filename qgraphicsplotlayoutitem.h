#ifndef QGRAPHICSPLOTLAYOUTITEM_H
#define QGRAPHICSPLOTLAYOUTITEM_H
#include <qgraphicsplot.h>
#include <QGraphicsLayoutItem>

class QGraphicsPlotLayoutItem : public QGraphicsPlot, public QGraphicsLayoutItem
{
public:
    QGraphicsPlotLayoutItem(QGraphicsItem* parent = 0);
    ~QGraphicsPlotLayoutItem();

   virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF()) const;
   virtual void setGeometry(const QRectF & rect);
};

#endif // QGRAPHICSPLOTLAYOUTITEM_H
