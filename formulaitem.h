#ifndef FORMULAITEM_H
#define FORMULAITEM_H
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPalette>
#include <QPaintEvent>
#include <QDebug>

class FormulaItem: public QGraphicsItem
{
public:
    FormulaItem( QGraphicsItem *parent = NULL);
    ~FormulaItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QPixmap toPixmap() const;
public slots:
    QString formula() const;
    void setFormula(const QString &formula);
    void setFontSize(const qreal &fontSize);
    void setTransformation(const bool &transformation);
    void setScale(const bool &scale);
    void setRotation(const qreal &rotation);
    void setDrawFrames(const bool &drawFrames);
    void setColors(const bool &colors);

protected:
  //  void paintEvent(QPaintEvent *event);
private:
    void renderFormula(QPainter *painter) const;
private:
    QString d_formula;
    qreal d_fontSize;
    bool d_transformation;
    bool d_scale;
    qreal d_rotation;
    bool d_drawFrames;
    bool d_colors;
    QRectF d_docRect;
    QPixmap painterCache;
    bool isPainted;
};

#endif // FORMULAITEM_H
