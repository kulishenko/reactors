#ifndef SCHEMASCENE_H
#define SCHEMASCENE_H
#include <QGraphicsScene>
#include <schemaitem.h>

class SchemaItem;
class SchemaScene : public QGraphicsScene
{
public:
    SchemaScene(QObject* parent = 0);
    ~SchemaScene();
    void addItem(SchemaItem *item);
    QList<SchemaItem* > schemaItems() const;
    SchemaItem* getItemByElementId(const int Value) const;
private:
    QList<SchemaItem* > m_schemaItems;
};

#endif // SCHEMASCENE_H
