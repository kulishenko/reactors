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

/*! \brief Класс, предназначенный для сериализации и десериализации схемы из файла формата XML

  Для работы с файлами используется DOM XML, сериализация и десериализация обеспечивается
  работой с Q_PROPERTY с помощью шаблонной функции

*/

class SchemaConfig
{
public:
    SchemaConfig();
    ~SchemaConfig();
    bool serializeObject(QObject *object, QIODevice *output); //!< Устаревшая функция
    template<class T>
    T* deserialize(QDomElement *element, SchemaScene* parent = 0){
        /*!
         * \brief Десериализует объект шаблонного класса T, унаследованного от QObject
         * \param[in] element Указатель на элемент DOM XML, содержащий описание объекта
         * \param[in] parent Указатель на графическую сцену (объект SchemaScene)
         */
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
    bool _deserializeObject(QDomElement *element, QObject *object);
};

#endif // SCHEMACONFIG_H
