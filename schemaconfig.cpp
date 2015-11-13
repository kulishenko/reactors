#include "schemaconfig.h"
#include <QBuffer>

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

int SchemaConfig::getIntType(const SchemaConfig::SchemaItemType t)
{
    switch(t){
        case SchemaItemType::SchemaCSTR:
            return 0;
            break;
        case SchemaItemType::SchemaPFR:
            return 1;
            break;
    }
    return 0;
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
bool SchemaConfig::serializeObject(QObject* object, QIODevice *output){
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

    QTextStream stream(output);
    doc.save(stream, 2);
    return true;
}

bool SchemaConfig::_deserializeObject(QIODevice *input, QObject *object)
{
    QDomDocument doc;
    if (!doc.setContent(input))
        return false;
    QDomElement element = doc.documentElement();
    for(int i = 0; i < object->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = object->metaObject()->property(i);
        QString propName = prop.name();
        if(propName == "objectName")
            continue;
        QDomNodeList nodeList = element.elementsByTagName(propName);
        if(nodeList.length() < 1)
            continue;
        QDomNode node = nodeList.at(0);
        QVariant value = object->property(propName.toLocal8Bit().data());
        QString v = node.toElement().text();
        object->setProperty(propName.toLocal8Bit().data(), QVariant(v));
    }
    return true;
}

