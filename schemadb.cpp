#include "schemadb.h"

SchemaDB::SchemaDB(QObject *parent) : QObject(parent)
{

}

SchemaDB::~SchemaDB()
{

}

bool SchemaDB::createConnection(){
    if(!isConnected){
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("sa.lti-gti.ru");
        db.setPort(13306);
        db.setDatabaseName("reactors");
        db.setUserName("reactors");
        db.setPassword("Dfl2cR38prF2vbT");
        if (!db.open()) {
            qDebug() << "Database error occurred";
            perror("Database connection error");
            //QMessageBox::warning(this, tr("DB connection failure"),
            //                     tr("Failed to connect to the database"));
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
         db.transaction();
         query.exec(QString("INSERT INTO `Lab` (SchemaID, UserID, LabDateTime, LabNumCascade, LabFlowrate, LabComment) "
                                "VALUES (1, 1, NOW(), %1 ,%2, '%3')").arg(QString::number(Control->NumCascade),
                                                                                          QString::number(static_cast<int>(Control->PlaybackFlowrate)),
                                                                                          Control->PlaybackFileName));

         int LabID = query.lastInsertId().toInt();


         query.prepare("INSERT INTO `Point` (ParameterID, LabID, PointTime, PointValue) "
                       "VALUES (:ParameterID, :LabID, :PointTime, :PointValue)");
         query.bindValue(":ParameterID", 1);
         query.bindValue(":LabID", LabID);
         for(int i = 0; i < Control->Conductivity.size(); i++){
             query.bindValue(":PointTime", Control->Time.at(i));
             query.bindValue(":PointValue", Control->Conductivity.at(i));
             query.exec();
         }
         db.commit();
         emit finishedResult(true);
     } else emit finishedResult(false);
}

void SchemaDB::getLabID(QModelIndex index)
{
    if(createConnection() && LabsModel != NULL){
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
        Control = new PFDControl();
        for (int i = 0; i < model.rowCount(); ++i) {
            Control->Time.push_back(model.record(i).value("PointTime").toDouble());
            Control->Conductivity.push_back(model.record(i).value("PointValue").toDouble());
        }
        Control->setPlaybackFlowrate(LabFlowrate);
        Control->setNumCascade(LabNumCascade);
        emit getLabDataFinished();
        emit getLabDataFinishedResult(true);
    } else emit getLabDataFinishedResult(false);
}
void SchemaDB::setData(PFDControl* data) {
    Control = data;
}
PFDControl* SchemaDB::getData() {
    return Control;
}
bool SchemaDB::getLabsTable(){
    if(createConnection()){
        LabsModel = new QSqlRelationalTableModel(this, db);
        LabsModel->setTable("Lab");
    //    LabsModel->setRelation(1, QSqlRelation("Schema", "SchemaID", "name"));
        LabsModel->setRelation(2, QSqlRelation("User", "UserID", "UserName"));
        LabsModel->setHeaderData(2, Qt::Horizontal, tr("User"));
        LabsModel->setHeaderData(3, Qt::Horizontal, tr("Lab date"));
        LabsModel->setHeaderData(4, Qt::Horizontal, tr("Number of Reactors"));
        LabsModel->setHeaderData(5, Qt::Horizontal, tr("Flowrate, L/hr"));
        LabsModel->setHeaderData(6, Qt::Horizontal, tr("Comment"));
        LabsModel->select();

        return true;
    } else return false;
}
bool SchemaDB::isConnected = false;
QSqlDatabase SchemaDB::db;
