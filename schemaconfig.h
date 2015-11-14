#ifndef SCHEMACONFIG_H
#define SCHEMACONFIG_H
#include <QMap>
#include <QString>
#include <QList>
#include <schemaitem.h>
#include <schemacstr.h>
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
    T* deserialize(QIODevice *input){
        T* object = new T();
        if(_deserializeObject(input, object))
            return object;
        object->deleteLater();
        return NULL;
    }
private:
    bool _deserializeObject(QIODevice* input, QObject* object);
    void ParseXMLConfig();
};

#endif // SCHEMACONFIG_H
