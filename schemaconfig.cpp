#include "schemaconfig.h"
#include <QBuffer>

SchemaConfig::SchemaConfig()
{
}

SchemaConfig::~SchemaConfig()
{

}
/*!
 * \brief Сериализует в формат XML объект, унаследованный от QObject
 * \param[in] object Указатель на объект
 * \param[in,out] output Указатель на устройство ввода
 * \deprecated Данная функция будет удалена в дальнейшем
 * \return Булево значение результата
 */
bool SchemaConfig::serializeObject(QObject* object, QIODevice *output){

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
/*!
 * \brief Десериализует объект QObject
 * \param[in] element Указатель на элемент DOM XML, содержащий описание объекта
 * \param[out] object Указатель на десериализованный объект
 * \return Булево значение результата
 */
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

/*!
 * Десериализует схему из файла формата XML
 * \param[in] input Указатель на файл схемы
 * \param[in] parent Указатель на родительский объект
 * \return Указатель на десериализованный объект SchemaScene
 */
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

    nodeList = root.elementsByTagName("SchemaPFR");
    for(int i = 0; i < nodeList.count(); i++) {
        QDomElement node = nodeList.at(i).toElement();
        Scene->addItem(this->deserialize<SchemaPFR>(&node));
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

/*!
 * Сериализует схему в файла формата XML
 * \param[in] scene Указатель на сцену схемы
 * \param[out] output Указатель на устройство вывода
 * \return булево значение результата
 */
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

