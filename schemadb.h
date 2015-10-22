#ifndef SCHEMADB_H
#define SCHEMADB_H

#include <QObject>
#include <pfdcontrol.h>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>

class SchemaDB : public QObject
{
    Q_OBJECT
public:
    explicit SchemaDB(QObject *parent = 0);
    ~SchemaDB();
    void setData(PFDControl *data);
    bool getLabsTable();
    QSqlTableModel* LabsModel;
private:
    static QSqlDatabase db;
    static bool isConnected;
    PFDControl* Control;


    bool createConnection();

signals:
    void finished();
    void finishedResult(bool result);
public slots:
    void sendLabData();
};

#endif // SCHEMADB_H
