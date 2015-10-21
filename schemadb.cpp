#include "schemadb.h"

SchemaDB::SchemaDB(QObject *parent) : QObject(parent)
{
    isConnected = false;
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
    // ToDo: add connection over SSH (libssh) and move to a separate thread (create wrapper class for DB works)
     if(createConnection()){
         QSqlQuery query;
         db.transaction();
         query.exec(QString("INSERT INTO `Lab` (SchemaID, UserID, LabDateTime, LabFlowrate, LabComment) "
                                "VALUES (1, 1, NOW(), %1, '%2')").arg(QString::number(static_cast<int>(Control->PlaybackFlowrate)),
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
void SchemaDB::setData(PFDControl* data) {
    Control = data;
}
