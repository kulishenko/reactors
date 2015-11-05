#include "schemaconfig.h"

SchemaConfig::SchemaConfig()
{


/*
SchemaItem &sItem;


    ElementItemTypes.insert("SchemaCSTR", SchemaCSTR);
    ElementItemTypes.insert("SchemaPFR", SchemaPFR);

    ElementType *item = new ElementType;


    item->insert("type", ElementItemTypes.find(typeid(sItem).name()).value());
    Config.append(item);*/
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
bool SchemaConfig::SerializeObject(QObject* object){
    QDomDocument doc;
    QDomElement root = doc.createElement(object->metaObject()->className());
    doc.appendChild(root);

    for(int i = 0; i < object->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = object->metaObject()->property(i);
        QString propName = prop.name();
        if(propName == "objectName")
            continue;
        QDomElement el = doc.createElement(propName);
        QVariant value = object->property(propName.toLocal8Bit().data());
        QDomText txt = doc.createTextNode( value.toString() );
        qDebug() << propName + " " + value.toString();
        el.appendChild(txt);
        root.appendChild(el);
    }
    QFile output("c:/config.xml");
    QTextStream stream(&output);
    doc.save(stream, 2);
    return true;
}
