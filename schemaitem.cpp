#include "schemaitem.h"
SchemaItem::RunMode SchemaItem::SchemaMode = SchemaItem::RunMode::Offline;
SchemaItem::SchemaItem() : QObject(), PFD(nullptr)
{

}

SchemaItem::~SchemaItem()
{

}
