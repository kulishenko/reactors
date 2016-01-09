#include "schemascene.h"
#include <schemapipeline.h>
#include <QGraphicsView>

SchemaScene::SchemaScene(QObject *parent) : QGraphicsScene(parent), m_List(nullptr), m_schemaItemChain(nullptr),
    p_ItemFrom(nullptr), p_ItemTo(nullptr)
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
    return m_ItemTypes.keys("SchemaValve").first();
}

int SchemaScene::getFlowmeterItemElementId() const
{
    //return m_flowmeterItemElementId;
    return m_ItemTypes.keys("SchemaFlowmeter").first();
}


QList<int> *SchemaScene::getReactorItemsList()
{
    if(!m_List) {
        m_List = new QList<int>(m_ItemTypes.keys("SchemaCSTR"));
        qSort(*m_List);
    }

    return m_List;

}

void SchemaScene::addItemToMap(const QString &ItemType, const SchemaItem* item)
{
    m_ItemTypes.insert(item->property("ElementId").toInt(), ItemType);
}


void SchemaScene::sequencing()
{
    if(m_schemaItemChain) delete m_schemaItemChain;
    m_schemaItemChain = new QVector<int>;

    QList<int> pipelineItemsElementId = m_ItemTypes.keys("SchemaPipeline");
    QList<int> streamItemsElementId = m_ItemTypes.keys("SchemaStream");
    int firstPipelineItemElementId = 0;


    foreach(const int pipelineItemElementId, pipelineItemsElementId)
        foreach(const int streamItemElementId, streamItemsElementId)
            if(getItemByElementId(pipelineItemElementId)->property("FromElementId").toInt() == streamItemElementId) {
                firstPipelineItemElementId = pipelineItemElementId;
                m_schemaItemChain->push_back(streamItemElementId);
                break;
            }

    if(m_schemaItemChain->isEmpty()) return;

    SchemaItem *next = getItemByElementId(firstPipelineItemElementId);
    for(int i = 0; i < pipelineItemsElementId.size(); i++) {
        m_schemaItemChain->push_back(next->property("ToElementId").toInt());
        next = getItemByElementId(getNextPipelineElementId(next->property("ElementId").toInt()));
    }


    qDebug() << *m_schemaItemChain;
}


int SchemaScene::getNextPipelineElementId(const int prevPipelineItemElementId) const
{
    foreach(const int pipelineItemElementId, m_ItemTypes.keys("SchemaPipeline"))
        if(getItemByElementId(pipelineItemElementId)->property("FromElementId").toInt() == getItemByElementId(prevPipelineItemElementId)->property("ToElementId").toInt())
            return pipelineItemElementId;
    return 0;
}

void SchemaScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // KOCTbI/|b (move to SchemaItem's events?)
    if(SchemaItem::SchemaMode == SchemaItem::RunMode::Edit && SchemaItem::AttachMode){
        QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
        if(!item) return;

        SchemaItem* sItem = dynamic_cast<SchemaItem*>(item);
        if(!sItem)
            sItem = dynamic_cast<SchemaItem*>(item->parentItem());

        if(sItem){
            p_ItemFrom = sItem;
            qDebug() << p_ItemFrom->property("ElementId").toString();
            event->accept();
            return;
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void SchemaScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(SchemaItem::SchemaMode == SchemaItem::RunMode::Edit && SchemaItem::AttachMode){
        QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
        if(!item) return;

        SchemaItem* sItem = dynamic_cast<SchemaItem*>(item);
        if(!sItem)
            sItem = dynamic_cast<SchemaItem*>(item->parentItem());

        if(sItem && p_ItemFrom){
            p_ItemTo = sItem;
            if(p_ItemTo && p_ItemTo != p_ItemFrom) {
                addItem(new SchemaPipeline(p_ItemFrom, p_ItemTo));
                qDebug() << p_ItemTo->property("ElementId").toString();
            }
            event->ignore();
            p_ItemFrom = nullptr;
            return;
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}


QList<int> SchemaScene::getItemsControlSeq(const QStringList &ItemTypes)
{
    QList<int> Sequence;
    for(int i = 0; i < m_schemaItemChain->size(); i++)
        foreach(const QString& itemType, ItemTypes)
            if(m_ItemTypes.value(m_schemaItemChain->at(i)) == itemType)
                Sequence.append(m_schemaItemChain->at(i));
    return Sequence;
}

void SchemaScene::selectAllItems()
{
    foreach(SchemaItem *item, m_schemaItems)
        if(!item->isSelected()) item->setSelected(true);
}

void SchemaScene::deSelectAllItems()
{
    foreach(SchemaItem *item, m_schemaItems)
       // if(item->isSelected())
        item->setSelected(false);
}

bool SchemaScene::isCorrect() const
{

}


