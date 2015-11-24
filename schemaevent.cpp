#include "schemaevent.h"
QEvent::Type SchemaEvent::Editor = QEvent::None;
SchemaEvent::SchemaEvent() : QEvent(SchemaEvent::type())
{

}

SchemaEvent::~SchemaEvent()
{

}

