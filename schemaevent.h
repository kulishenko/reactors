#ifndef SCHEMAEVENT_H
#define SCHEMAEVENT_H
#include <QEvent>

class SchemaEvent : public QEvent
{
public:
    SchemaEvent();
    ~SchemaEvent();
    static QEvent::Type type()
    {
        QEvent::Type cEditor;
        if (Editor == QEvent::None)
        {

            int generatedType = QEvent::registerEventType();
            cEditor = static_cast<QEvent::Type>(generatedType);
        }
        return cEditor;
    }
    static QEvent::Type Editor;
};
#endif // SCHEMAEVENT_H
