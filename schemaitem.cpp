#include "schemaitem.h"
int SchemaItem::s_ElementId = 0;
SchemaItem::RunMode SchemaItem::SchemaMode = SchemaItem::RunMode::Offline;
SchemaItem::SchemaItem() : QObject(), PFD(nullptr), m_ElementId(s_ElementId++)
{

}

SchemaItem::~SchemaItem()
{

}
