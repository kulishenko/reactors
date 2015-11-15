#include "schemadb.h"

SchemaDB::SchemaDB(QObject *parent) : QObject(parent), p_LabsModel(nullptr), p_Control(nullptr)
{

}

SchemaDB::~SchemaDB()
{

}

bool SchemaDB::createConnection(){
    if(!isConnected){
        m_db = QSqlDatabase::addDatabase("QMYSQL");
        m_db.setHostName("sa.lti-gti.ru");
        m_db.setPort(13306);
        m_db.setDatabaseName("reactors");
        m_db.setUserName("reactors");
        m_db.setPassword("Dfl2cR38prF2vbT");
        if (!m_db.open()) {
            qDebug() << "Database error occurred";
            perror("Database connection error");
            return false;
        }
        isConnected = true;
        return true;
    } else return true;
}
void SchemaDB::sendLabData(){
    // ToDo: add connection over SSH (libssh) (Or develop PHP JSON frontend?)
     if(createConnection()){
         QSqlQuery query;
         m_db.transaction();
         query.exec(QString("INSERT INTO `Lab` (SchemaID, UserID, LabDateTime, LabNumCascade, LabFlowrate, LabComment) "
                                "VALUES (1, 1, NOW(), %1 ,%2, '%3')").arg(QString::number(p_Control->getNumCascade()),
                                                                                          QString::number(static_cast<int>(p_Control->getPlaybackFlowrate())),
                                                                                          p_Control->getPlaybackFileName()));

         int LabID = query.lastInsertId().toInt();


         query.prepare("INSERT INTO `Point` (ParameterID, LabID, PointTime, PointValue) "
                       "VALUES (:ParameterID, :LabID, :PointTime, :PointValue)");
         query.bindValue(":ParameterID", 1);
         query.bindValue(":LabID", LabID);
         for(int i = 0; i < p_Control->getCount(); i++){
             query.bindValue(":PointTime", p_Control->getTimeAt(i));
             query.bindValue(":PointValue", p_Control->getParameterAt(i));
             query.exec();
         }
         m_db.commit();
         emit finishedResult(true);
     } else emit finishedResult(false);
}

void SchemaDB::getLabID(QModelIndex index)
{
    if(createConnection() && p_LabsModel){
        LabID = index.sibling(index.row(),0).data().toInt();
        LabNumCascade = index.sibling(index.row(),4).data().toInt();
        LabFlowrate = index.sibling(index.row(),5).data().toDouble();
        getLabData();
    }
}

void SchemaDB::getLabData()
{
    if(createConnection()) {
        QSqlQueryModel model;
        model.setQuery("SELECT PointTime, PointValue FROM Point "
                       "WHERE LabID = " + QString::number(LabID));
        if(p_Control) p_Control->deleteLater();
        p_Control = new PFDControl();
        for (int i = 0; i < model.rowCount() && model.record(i).value("PointTime").toDouble() != 0.0; ++i)
            p_Control->addPoint(model.record(i).value("PointTime").toDouble(),
                                model.record(i).value("PointValue").toDouble());

        p_Control->setPlaybackFlowrate(LabFlowrate);
        p_Control->setNumCascade(LabNumCascade);
        emit getLabDataFinished();
        emit getLabDataFinishedResult(true);
    } else emit getLabDataFinishedResult(false);
}
void SchemaDB::setData(PFDControl* data) {
    p_Control = data;
}
PFDControl* SchemaDB::getData() {
    return p_Control;
}

QSqlRelationalTableModel* SchemaDB::getLabsModel() const
{
    return p_LabsModel;
}
bool SchemaDB::getLabsTable(){
    if(createConnection()){
        p_LabsModel = new QSqlRelationalTableModel(this, m_db);
        p_LabsModel->setTable("Lab");
    //    LabsModel->setRelation(1, QSqlRelation("Schema", "SchemaID", "name"));
        p_LabsModel->setRelation(2, QSqlRelation("User", "UserID", "UserName"));
        p_LabsModel->setHeaderData(2, Qt::Horizontal, tr("User"));
        p_LabsModel->setHeaderData(3, Qt::Horizontal, tr("Lab date"));
        p_LabsModel->setHeaderData(4, Qt::Horizontal, tr("Number of Reactors"));
        p_LabsModel->setHeaderData(5, Qt::Horizontal, tr("Flowrate, L/hr"));
        p_LabsModel->setHeaderData(6, Qt::Horizontal, tr("Comment"));
        p_LabsModel->sort(6, Qt::AscendingOrder);
        p_LabsModel->select();

        return true;
    } else return false;
}
bool SchemaDB::isConnected = false;
QSqlDatabase SchemaDB::m_db;
