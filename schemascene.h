#ifndef SCHEMASCENE_H
#define SCHEMASCENE_H
#include <QGraphicsScene>
#include <schemaitem.h>
#include <QDebug>
#include <QMultiMap>

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
    void sequencing();
    QList<int> getItemsControlSeq(const QStringList &ItemType);
    void selectAllItems();
    void deSelectAllItems();
    bool isCorrect() const;
private:
    void addItemToMap(const QString &ItemType, const SchemaItem *item);
    QList<int> *m_List;
    QList<SchemaItem* > m_schemaItems;
    QMap<int, QString> m_ItemTypes;
    QVector<int> *m_schemaItemChain;
    SchemaItem *p_ItemFrom, *p_ItemTo;
    int getNextPipelineElementId(const int prevPipelineItemElementId) const;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCHEMASCENE_H
