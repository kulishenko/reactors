#include "schemascene.h"

SchemaScene::SchemaScene(QObject *parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));
}

SchemaScene::~SchemaScene()
{

}

void SchemaScene::addItem(SchemaItem *item)
{
   item->setParent(this);
   m_schemaItems.append(item);
   QGraphicsScene::addItem(item);
}

QList<SchemaItem* > SchemaScene::schemaItems() const
{
    return m_schemaItems;
}

SchemaItem *SchemaScene::getItemByElementId(const int Value) const
{
    foreach(SchemaItem* item, m_schemaItems)
        if(item->property("ElementId").toInt() == Value)
            return item;

    qDebug() << "Couldn't find item: ElementId: " + QString::number(Value);
    return 0;
}

