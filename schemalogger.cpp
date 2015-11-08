#include "schemalogger.h"
#include <QDateTime>

SchemaLogger::SchemaLogger() : outWidget(nullptr)
{

}

SchemaLogger::~SchemaLogger()
{

}
void SchemaLogger::setWidget(QListWidget* w) {
    outWidget = w;
}
SchemaLogger& SchemaLogger::operator<<(const QString &x) {
    if(outWidget){
        QDateTime EventTime(QDateTime::currentDateTime());
        outWidget->addItem(EventTime.toString("[hh:mm:ss.zzz]: ") + x);
        outWidget->scrollToBottom();
    }
    return *this;
}
