#include "schemapfr.h"

SchemaPFR::SchemaPFR(qreal Width, qreal Height, QObject *parent) : SchemaItem()
{
    p_Path = new QPainterPath;
    p_Path->addRect(0, 0, Width, Height);
    setPath(*p_Path);

    QLinearGradient* Gradient = new QLinearGradient(0, 0, Width, 0);

    Gradient->setColorAt(1, QColor::fromRgb(245, 252, 255));
    Gradient->setColorAt(0.5, Qt::white);
    Gradient->setColorAt(0, QColor::fromRgb(209, 244, 255));

    setBrush(*Gradient);

}

SchemaPFR::~SchemaPFR()
{

}

