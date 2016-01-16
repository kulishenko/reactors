#include "qgraphicsplotlayoutitem.h"

QGraphicsPlotLayoutItem::QGraphicsPlotLayoutItem(QGraphicsItem *parent) :

    QGraphicsPlot(parent), QGraphicsLayoutItem()
{
    setGraphicsItem(this);
}

QGraphicsPlotLayoutItem::~QGraphicsPlotLayoutItem()
{

}

QSizeF QGraphicsPlotLayoutItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{

    switch ( which )
    {
        case Qt::MinimumSize:
        case Qt::PreferredSize:
            return this->boundingRect().size();

 //      case Qt::MaximumSize:
 //           return QSizeF(parentItem()->boundingRect().width(),
 //                  MyOptions::DefaultHeight);

        default:
            return this->boundingRect().size();
    }

    return constraint;
}

void QGraphicsPlotLayoutItem::setGeometry(const QRectF &rect)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(rect);
    setPos(rect.topLeft());
}

