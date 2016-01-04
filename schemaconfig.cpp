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
    // Deprecated
    QDomDocument doc;
    QDomElement root;
    QString infile;

    if(output->open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream stream(output);
        infile.append(stream.readAll());
        output->close();
        doc.setContent(infile);
    }

    output->open(QIODevice::WriteOnly | QIODevice::Text);

    if(doc.elementsByTagName("SchemaItems").isEmpty()){
        root = doc.createElement("SchemaItems");
        doc.appendChild(root);
    }
    else
        root = doc.elementsByTagName("SchemaItems").at(0).toElement();

    QDomElement objectRoot = doc.createElement(object->metaObject()->className());
    root.appendChild(objectRoot);

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
        objectRoot.appendChild(el);
    }

    QTextStream stream(output);
    doc.save(stream, 2);
    output->close();
    return true;
}

bool SchemaConfig::_deserializeObject(QDomElement *element, QObject *object)
{

    for(int i = 0; i < object->metaObject()->propertyCount(); i++)
    {
        QMetaProperty prop = object->metaObject()->property(i);
        QString propName = prop.name();
        if(propName == "objectName")
            continue;
        QDomNodeList nodeList = element->elementsByTagName(propName);
        if(nodeList.length() < 1)
            continue;
        QDomNode node = nodeList.at(0);
 //       QVariant value = object->property(propName.toLocal8Bit().data());
        QString v = node.toElement().text();
        object->setProperty(propName.toLocal8Bit().data(), QVariant(v));
    }
    return true;
}

SchemaScene* SchemaConfig::deserializeScene(QIODevice *input, QObject *parent)
{
    input->open(QIODevice::ReadOnly | QIODevice::Text);
    SchemaScene* Scene = new SchemaScene(parent);
    QDomDocument doc;
    if (!doc.setContent(input))
        return Scene;

    QDomElement root = doc.elementsByTagName("SchemaItems").at(0).toElement();

    QDomNodeList nodeList = root.elementsByTagName("SchemaStream");
    for(int i = 0; i < nodeList.count(); i++) {
        QDomElement node = nodeList.at(i).toElement();
        Scene->addItem(this->deserialize<SchemaStream>(&node));
    }

    nodeList = root.elementsByTagName("SchemaCell");
    for(int i = 0; i < nodeList.count(); i++) {
        QDomElement node = nodeList.at(i).toElement();
        Scene->addItem(this->deserialize<SchemaCell>(&node));
    }

    nodeList = root.elementsByTagName("SchemaValve");
    for(int i = 0; i < nodeList.count(); i++) {
        QDomElement node = nodeList.at(i).toElement();
        Scene->addItem(this->deserialize<SchemaValve>(&node));
    }

    nodeList = root.elementsByTagName("SchemaCSTR");
    for(int i = 0; i < nodeList.count(); i++) {
        QDomElement node = nodeList.at(i).toElement();
        Scene->addItem(this->deserialize<SchemaCSTR>(&node));
    }

    nodeList = root.elementsByTagName("SchemaFlowmeter");
    for(int i = 0; i < nodeList.count(); i++) {
        QDomElement node = nodeList.at(i).toElement();
        Scene->addItem(this->deserialize<SchemaFlowmeter>(&node));
    }

    nodeList = root.elementsByTagName("SchemaPipeline");
    for(int i = 0; i < nodeList.count(); i++) {
        QDomElement node = nodeList.at(i).toElement();
        Scene->addItem(this->deserialize<SchemaPipeline>(&node, Scene));
    }

    input->close();
    return Scene;
}

bool SchemaConfig::serializeScene(SchemaScene *scene, QIODevice *output)
{
    QDomDocument doc;
    QDomElement root;

    output->open(QIODevice::WriteOnly | QIODevice::Text);

    root = doc.createElement("SchemaItems");
    doc.appendChild(root);

    foreach(SchemaItem* object, scene->schemaItems()){
        QDomElement objectRoot = doc.createElement(object->metaObject()->className());
        root.appendChild(objectRoot);

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
            objectRoot.appendChild(el);
        }
    }
    QTextStream stream(output);
    doc.save(stream, 2);
    output->close();
    return true;
}

