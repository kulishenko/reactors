#ifndef LOGGER_H
#define LOGGER_H
#include <QListWidget>

class SchemaLogger
{
public:
    SchemaLogger();
    ~SchemaLogger();
    QListWidget* outWidget;
    void setWidget(QListWidget *w);
//    SchemaLogger &operator <<(const char *x);
    SchemaLogger &operator <<(const QString &x);
};

#endif // LOGGER_H
