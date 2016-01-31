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

/*! \brief Класс, предназначенный для доступа к СУБД (импорт и экспорт экспериментальных данных)

    Используется в качестве обертки QObject для экспорта данных в отдельном потоке
*/
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
    static QSqlDatabase m_db; //!< Указатель на СУБД
    QSqlRelationalTableModel* p_LabsModel; //!< Указатель на объектную модель СУБД
    static bool isConnected; //!< Наличие подключения
    PFDControl* p_Control; //!< Указатель на управление схемой
    int LabID; //!< Идентификатор опыта
    qreal LabFlowrate; //!< Объемный расход опыта
    int LabNumCascade; //!< Число аппаратов в каскаде
    bool createConnection(); //!< Устаналивает соединение с СУБД, если оно отсутствует
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
