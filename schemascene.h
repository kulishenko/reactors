#ifndef SCHEMASCENE_H
#define SCHEMASCENE_H
#include <QGraphicsScene>
#include <schemaitem.h>
#include <QDebug>
#include <QMultiMap>
//#include <schemacstr.h>
//#include <schemavalve.h>
//#include <schemaflowmeter.h>

class SchemaItem;
class SchemaScene : public QGraphicsScene
{
public:
    SchemaScene(QObject* parent = 0);
    ~SchemaScene();
    void addItem(SchemaItem *item);
    QList<SchemaItem* > schemaItems() const;
    SchemaItem* getItemByElementId(const int Value) const;
    int getValveItemElementId() const;
    int getFlowmeterItemElementId() const;
    QList<int> *getReactorItemsList();
private:
    void addItemToMap(const QString &ItemType, const SchemaItem *item);
    QList<int> *m_List;
    QList<SchemaItem* > m_schemaItems;
//    int m_valveItemElementId;
//    int m_flowmeterItemElementId;
    QList<int> m_reactorItemsElementId;
    QMultiMap<QString, int> m_ItemTypes;
};

#endif // SCHEMASCENE_H
