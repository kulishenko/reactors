#include "schemaconfig.h"

SchemaConfig::SchemaConfig(const SchemaItem &sItem)
{






    ElementItemTypes.insert("SchemaCSTR", SchemaCSTR);
    ElementItemTypes.insert("SchemaPFR", SchemaPFR);

    ElementType *item = new ElementType;


    item->insert("type", ElementItemTypes.find(typeid(sItem).name()).value());
    Config.append(item);
}

SchemaConfig::~SchemaConfig()
{

}

int SchemaConfig::getIntType(SchemaConfig::SchemaItemType t)
{
    switch(t){
        case SchemaCSTR:
            return 0;
            break;
        case SchemaPFR:
            return 1;
            break;

    }

}
void SchemaConfig::ParseXMLConfig(){
    QFile* file = new QFile("config.xml");
       if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
       {
           qDebug()<< QObject::tr("Unable to open XML-config");
           return;
       }
       QXmlStreamReader xml(file);
}


