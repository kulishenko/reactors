#ifndef SCHEMACONFIG_H
#define SCHEMACONFIG_H
#include <QMap>
#include <QString>
#include <QList>
#include <schemaitem.h>
#include <schemacstr.h>
#include <schemaflowmeter.h>
#include <schemastream.h>
#include <schemacell.h>
#include <schemavalve.h>
#include <typeinfo>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QMetaProperty>

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
    bool serializeObject(QObject *object, QIODevice *output);
    template<class T>
    T* deserialize(QDomElement *element){
        T* object = new T();
        if(_deserializeObject(element, object))
            return object;
        object->deleteLater();
        return NULL;
    }
    QList<SchemaItem *> deserializeSchema(QIODevice* input);
private:
    void ParseXMLConfig();
    bool _deserializeObject(QDomElement *element, QObject *object);
};

#endif // SCHEMACONFIG_H
