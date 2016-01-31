#include "formulaitem.h"
#include "qwt_mml_document.h"


FormulaItem::FormulaItem(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    d_fontSize( 8 ),
    d_transformation( true ),
    d_scale( false ),
    d_rotation( 0 ),
    d_docRect(0, 0, 0, 0)

{
    //painterCache = toPixmap();
}

FormulaItem::~FormulaItem()
{

}

void FormulaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->fillRect( boundingRect(), Qt::white );
    renderFormula( painter );

}

QRectF FormulaItem::boundingRect() const
{
    QwtMathMLDocument doc;
    doc.setContent( d_formula );
    QRectF docRect; docRect.setSize( doc.size() );
   // docRect.moveCenter( boundingRect().center() );

    return docRect;
}

QString FormulaItem::formula() const
{
    return d_formula;
}

void FormulaItem::setFormula( const QString &formula )
{
    d_formula = formula;
    update();
}

void FormulaItem::setFontSize( const qreal &fontSize )
{
    d_fontSize = fontSize;
    update();
}

void FormulaItem::setTransformation( const bool &transformation )
{
    d_transformation = transformation;
    update();
}

void FormulaItem::setScale( const bool &scale )
{
    d_scale = scale;
    update();
}

void FormulaItem::setRotation( const qreal &rotation )
{
    d_rotation = rotation;
    update();
}

void FormulaItem::setDrawFrames( const bool &drawFrames )
{
    d_drawFrames = drawFrames;
    update();
}

void FormulaItem::setColors( const bool &colors )
{
    d_colors = colors;
    update();
}
/*!
 * \brief Осуществляет прорисовку формулы, заданной через setFormula()
 * \param[out] painter указатель на устройство вывода
 */
void FormulaItem::renderFormula( QPainter *painter ) const
{
    QwtMathMLDocument doc;
    doc.setContent( d_formula );
    if ( d_colors )
    {
        doc.setBackgroundColor( Qt::white );
        doc.setForegroundColor( Qt::black );
    }
    /* else
    {
        doc.setBackgroundColor( this->palette().color(QPalette::Background) );
        doc.setForegroundColor( this->palette().text().color());
    } */
    doc.setBaseFontPointSize( d_fontSize );
//#ifdef MML_TEST
//    doc.setDrawFrames( d_drawFrames );
//#endif

    QRectF docRect;
    docRect.setSize( doc.size() );
    docRect.moveCenter( boundingRect().center() );

    if ( d_transformation )
    {
        const double scaleF = d_scale ? 2.0 : 1.0;

        painter->save();

        painter->translate( docRect.center() );
        painter->rotate( d_rotation );
        painter->scale( scaleF, scaleF );
        painter->translate( docRect.topLeft() - docRect.center() );
        doc.paint( painter, QPointF( 0.0, 0.0 ) );

        painter->restore();
    }
    else
    {
        doc.paint( painter, docRect.topLeft() );
    }

}
/*!
 * \brief Сохраняет блок формул в объект QPixmap
 * \return Объект QPixmap, содержащий блок формул
 */
QPixmap FormulaItem::toPixmap() const
{
    QRectF r = boundingRect();
    QPixmap pixmap(r.width(), r.height());
    pixmap.fill(QColor(0, 0, 0, 0));
    QPainter painter(&pixmap);
    painter.fillRect( r, Qt::white );

    //painter.setBrush(QBrush(QColor(0, 0, 0, 0)));
    //painter.drawRect(r);
    renderFormula(&painter);
    painter.end();
    return pixmap;
}
