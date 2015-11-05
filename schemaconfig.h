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
    enum SchemaItemType {SchemaCSTR, SchemaPFR};
    int getIntType(SchemaItemType t);
    QMap<QString, SchemaItemType> ElementItemTypes;
    bool SerializeObject(QObject* object);
private:
    void ParseXMLConfig();
};

#endif // SCHEMACONFIG_H
