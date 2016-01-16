#include "qgraphicspixmaplayoutitem.h"



QGraphicsPixmapLayoutItem::QGraphicsPixmapLayoutItem(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent), QGraphicsLayoutItem()
{
   setGraphicsItem(this);
}

QGraphicsPixmapLayoutItem::~QGraphicsPixmapLayoutItem()
{

}

QSizeF QGraphicsPixmapLayoutItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
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

void QGraphicsPixmapLayoutItem::setGeometry(const QRectF &rect)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(rect);
    setPos(rect.topLeft());
}
