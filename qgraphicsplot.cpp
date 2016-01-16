#include "qgraphicsplot.h"

QGraphicsPlot::QGraphicsPlot(QGraphicsItem* parent) :
    QGraphicsItem(parent),
    p_plot(nullptr)
{

}

QGraphicsPlot::~QGraphicsPlot()
{

}

void QGraphicsPlot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED( option ) ;
    Q_UNUSED( widget ) ;

    QPicture pic ;
    static QCPPainter qcpPainter ;
    qcpPainter.begin( &pic ) ;
    p_plot->toPainter( &qcpPainter, boundingRect().width(), boundingRect().height() ) ;
    qcpPainter.end() ;
    painter->drawPicture( 0, 0, pic ) ;
}

QRectF QGraphicsPlot::boundingRect() const
{
    return p_plot->rect();
}

void QGraphicsPlot::setPlot(QCustomPlot *plot)
{
    p_plot = plot;
}

