#include "dborionengine.h"
#include <QDebug>
#include <QSqlError>
DBOrionEngine::DBOrionEngine(QString driver, QString server, QString dbName, int port, QString userName, QString password)
{
    setDBParams(driver, server, dbName, port, userName, password);
    this->statusConnect = false;
}

DBOrionEngine::DBOrionEngine(AppSettings *settings)
{
    setDBParams(settings->getSetting("DB/Driver"), settings->getSetting("DB/Server"), settings->getSetting("DB/DBName"), settings->getSetting("DB/Port").toInt(),
                settings->getSetting("DB/User"), settings->getSetting("DB/Password"));
}

void DBOrionEngine::setDBParams(QString driver, QString server, QString dbName, int port, QString userName, QString password)
{
    DBParams.Driver = driver;
    DBParams.Server = server;
    DBParams.DBName = dbName;
    DBParams.Port = port;
    DBParams.UserName = userName;
    DBParams.Password = password;

}

void DBOrionEngine::Diconnect()
{
    db.close();
}

QSqlDatabase DBOrionEngine::Connect()
{

    db = QSqlDatabase::addDatabase(DBParams.Driver);
    db.setHostName(DBParams.Server);
    db.setPort(DBParams.Port);
    db.setDatabaseName(DBParams.DBName);
    db.setUserName(DBParams.UserName);
    db.setPassword(DBParams.Password);

    if(db.open())
    {
        this->statusConnect = true;
        if(APPDEBUG) qDebug() << "database open";
    } else {
       qDebug() << db.lastError();
    }

    return db;
}
