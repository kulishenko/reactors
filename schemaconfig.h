#ifndef SCHEMACONFIG_H
#define SCHEMACONFIG_H
#include <QMap>
#include <QString>
#include <QList>
#include <schemaitem.h>
#include <schemacstr.h>
#include <typeinfo>
#include <QXmlStreamReader>

class SchemaConfig
{
public:
    SchemaConfig(const SchemaItem &sItem);
    ~SchemaConfig();
    typedef QMap<QString, QVariant> ElementType;
    QList<ElementType*> Config;
    enum SchemaItemType {SchemaCSTR, SchemaPFR};
    int getIntType(SchemaItemType t);
    QMap<QString, SchemaItemType> ElementItemTypes;
private:
    void ParseXMLConfig();
};

#endif // SCHEMACONFIG_H
