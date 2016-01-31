#ifndef QGRAPHICSPIXMAPLAYOUTITEM_H
#define QGRAPHICSPIXMAPLAYOUTITEM_H


#include <QGraphicsPixmapItem>
#include <QGraphicsLayoutItem>

/*!
 * \brief Класс-обертка, предназначенный для вывода QGraphicsPixmapItem в Layout
 */
class QGraphicsPixmapLayoutItem : public QGraphicsPixmapItem, public QGraphicsLayoutItem
{
public:
    QGraphicsPixmapLayoutItem(QGraphicsItem* parent = 0);
    ~QGraphicsPixmapLayoutItem();
protected:
   virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF()) const;
   virtual void setGeometry(const QRectF & rect);
};

#endif // QGRAPHICSPIXMAPLAYOUTITEM_H
