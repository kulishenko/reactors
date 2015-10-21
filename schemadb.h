#ifndef SCHEMADB_H
#define SCHEMADB_H

#include <QObject>
#include <pfdcontrol.h>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>

class SchemaDB : public QObject
{
    Q_OBJECT
public:
    explicit SchemaDB(QObject *parent = 0);
    ~SchemaDB();
    void setData(PFDControl *data);
private:
    QSqlDatabase db;
    bool isConnected;
    PFDControl* Control;

    bool createConnection();

signals:
    void finished();
    void finishedResult(bool result);
public slots:
    void sendLabData();
};

#endif // SCHEMADB_H
