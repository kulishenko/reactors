#ifndef SCHEMACONFIG_H
#define SCHEMACONFIG_H
#include <QMap>
#include <QString>
#include <QList>
#include <schemaitem.h>
#include <schemacstr.h>
#include <schemapfr.h>
#include <schemaflowmeter.h>
#include <schemastream.h>
#include <schemacell.h>
#include <schemavalve.h>
#include <schemapipeline.h>
#include <typeinfo>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QMetaProperty>
#include <schemascene.h>

class SchemaConfig
{
public:
    SchemaConfig();
    ~SchemaConfig();
    typedef QMap<QString, QVariant> ElementType;
    QList<ElementType*> Config;
    enum class SchemaItemType {SchemaCSTR, SchemaPFR};
    int getIntType(const SchemaItemType t);
    QMap<QString, SchemaItemType> ElementItemTypes;
    bool serializeObject(QObject *object, QIODevice *output); // Deprecated
    template<class T>
    T* deserialize(QDomElement *element, SchemaScene* parent = 0){
        T* object = new T();
        object->setParent(parent);
        if(_deserializeObject(element, object))
            return object;
        object->deleteLater();
        return NULL;
    }
    SchemaScene* deserializeScene(QIODevice* input, QObject *parent = 0);
    bool serializeScene(SchemaScene *scene, QIODevice *output);
private:
    void ParseXMLConfig();
    bool _deserializeObject(QDomElement *element, QObject *object);
};

#endif // SCHEMACONFIG_H
