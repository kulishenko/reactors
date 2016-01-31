#ifndef SCHEMASCENE_H
#define SCHEMASCENE_H
#include <QGraphicsScene>
#include <schemaitem.h>
#include <QDebug>
#include <QMultiMap>

class SchemaItem;
/*!
 * \brief Класс, описывающий графическую сцену
 *
 * Содержит метаданные графических элементов схемы, а также функции для определения цепочки событий
 */
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
    QPixmap toPixmap();
private:
    void addItemToMap(const QString &ItemType, const SchemaItem *item);
    QList<int> *m_List; //!< Указатель на список элементов схемы
    QList<SchemaItem* > m_schemaItems; //!< Массив указателей на элементы схемы
    QMap<int, QString> m_ItemTypes; //!< Ассоциативный массив идентификаторов и типов графических элементов схемы
    QVector<int> *m_schemaItemChain; //!< Последовательность работы элементов схемы
    SchemaItem *p_ItemFrom, *p_ItemTo; //!< Указатель на графический элемент схемы
    int getNextPipelineElementId(const int prevPipelineItemElementId) const;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCHEMASCENE_H
