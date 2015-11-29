#include "schemascene.h"

SchemaScene::SchemaScene(QObject *parent) : QGraphicsScene(parent), m_List(nullptr)
{
    setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));
}

SchemaScene::~SchemaScene()
{

}

void SchemaScene::addItem(SchemaItem *item)
{

   addItemToMap(item->getItemType(), item);
   item->setParent(this);
   m_schemaItems.append(item);
   QGraphicsScene::addItem(item);
}

QList<SchemaItem *> SchemaScene::schemaItems() const
{
    return m_schemaItems;
}

SchemaItem *SchemaScene::getItemByElementId(const int Value) const
{
    foreach(SchemaItem* item, m_schemaItems)
        if(item->property("ElementId").toInt() == Value)
            return item;

    qDebug() << "Error: Couldn't find item with ElementId: " + QString::number(Value);
    return 0;
}

int SchemaScene::getValveItemElementId() const
{
    //return m_valveItemElementId;
    return m_ItemTypes.values("SchemaValve").first();
}

int SchemaScene::getFlowmeterItemElementId() const
{
    //return m_flowmeterItemElementId;
    return m_ItemTypes.values("SchemaFlowmeter").first();
}


QList<int> *SchemaScene::getReactorItemsList()
{
    if(!m_List) {
        m_List = new QList<int>(m_ItemTypes.values("SchemaCSTR"));
        qSort(*m_List);
    }

    return m_List;

}

void SchemaScene::addItemToMap(const QString &ItemType, const SchemaItem* item)
{
    m_ItemTypes.insert(ItemType,item->property("ElementId").toInt());
}
