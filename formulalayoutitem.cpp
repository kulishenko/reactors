#include "formulalayoutitem.h"

FormulaLayoutItem::FormulaLayoutItem(QGraphicsItem *parent) :
    FormulaItem(parent),
    QGraphicsLayoutItem()
{
    setGraphicsItem( this );
}

FormulaLayoutItem::~FormulaLayoutItem()
{

}

QSizeF FormulaLayoutItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{

    return boundingRect().size();
}

void FormulaLayoutItem::setGeometry(const QRectF &rect)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(rect);
    setPos(rect.topLeft());
}

