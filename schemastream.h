#ifndef SCHEMASTREAM_H
#define SCHEMASTREAM_H
#include <QGraphicsPolygonItem>
#include <schemaitem.h>

/*!
 * \brief Класс, описывающий графический элемент, обозначающий входной или выходной материальный поток
 */
class SchemaStream : public SchemaItem
{
    Q_OBJECT
public:
    SchemaStream(qreal Size = 50.0f, qreal PosX = 0.0f, qreal PosY = 0.0f, qreal Angle = 0.0f);
    ~SchemaStream();
};

#endif // SCHEMASTREAM_H
