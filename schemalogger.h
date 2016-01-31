#ifndef LOGGER_H
#define LOGGER_H
#include <QListWidget>
/*!
 * \brief Класс, предназначенный для перенаправления событий
 */
class SchemaLogger
{
public:
    SchemaLogger();
    ~SchemaLogger();
    QListWidget* outWidget; //!< Указатель на виджет для вывода сообщений
    void setWidget(QListWidget *w); //!< Устанавливает виджет для вывода сообщений
//    SchemaLogger &operator <<(const char *x);
    SchemaLogger &operator <<(const QString &x); //!< Выводит сообщение в журнал событий
};

#endif // LOGGER_H
