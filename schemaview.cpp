#include "schemaview.h"

SchemaView::SchemaView()
{

}

SchemaView::~SchemaView()
{

}
void SchemaView::mousePressEvent(QMouseEvent *event)
{
        qDebug() << "Custom view clicked.";
        QGraphicsView::mousePressEvent(event);
}

