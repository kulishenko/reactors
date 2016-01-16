#ifndef FORMULALAYOUTITEM_H
#define FORMULALAYOUTITEM_H
#include <formulaitem.h>
#include <QGraphicsLayoutItem>

class FormulaLayoutItem : public FormulaItem, public QGraphicsLayoutItem
{
public:
    FormulaLayoutItem(QGraphicsItem* parent = 0);
    ~FormulaLayoutItem();
protected:
   virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF()) const;
   virtual void setGeometry(const QRectF & rect);
};

#endif // FORMULALAYOUTITEM_H
