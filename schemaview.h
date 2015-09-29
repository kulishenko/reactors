#ifndef SCHEMAVIEW_H
#define SCHEMAVIEW_H
#include <QGraphicsView>
#include <QDebug>

class SchemaView : public QGraphicsView
{
public:
    SchemaView();
    ~SchemaView();
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // SCHEMAVIEW_H
