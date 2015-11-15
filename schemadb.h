#ifndef SCHEMADB_H
#define SCHEMADB_H

#include <QObject>
#include <pfdcontrol.h>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlRelationalTableModel>

class SchemaDB : public QObject
{
    Q_OBJECT
public:
    explicit SchemaDB(QObject *parent = 0);
    ~SchemaDB();
    void setData(PFDControl *data);
    bool getLabsTable();
    PFDControl *getData();
    QSqlRelationalTableModel *getLabsModel() const;
private:
    static QSqlDatabase m_db;
    QSqlRelationalTableModel* p_LabsModel;
    static bool isConnected;
    PFDControl* p_Control;
    int LabID;
    qreal LabFlowrate;
    int LabNumCascade;
    bool createConnection();
signals:
    void finished();
    void finishedResult(bool result);
    void getLabDataFinishedResult(bool result);
    void getLabDataFinished();
public slots:
    void sendLabData();
    void getLabID(QModelIndex index);
    void getLabData();
};

#endif // SCHEMADB_H
