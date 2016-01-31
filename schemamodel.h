#ifndef SCHEMAMODEL_H
#define SCHEMAMODEL_H
#include <schemadata.h>

/*!
 * \brief Базовый класс моделей, описывающих логику работы графических элементов схемы
 */

class SchemaModel
{
public:
    SchemaModel(SchemaData* Data = 0);
protected:
    SchemaData* p_Data; //!< Указатель на данные
};

#endif // SCHEMAMODEL_H
