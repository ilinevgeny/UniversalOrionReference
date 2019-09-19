#ifndef DBORIONENGINE_H
#define DBORIONENGINE_H
#define  APPDEBUG true

#include <QObject>
#include <QSqlDatabase>
#include "appsettings.h"

struct DBParams
{
    QString Driver;
    QString Server;
    QString DBName;
    int Port;
    QString UserName;
    QString Password;
};

class DBOrionEngine : public QObject
{
    Q_OBJECT
public:
    DBOrionEngine(QString driver, QString server, QString dbName, int port, QString userName, QString password);
    DBOrionEngine(AppSettings*);
    QSqlDatabase Connect();
    void Diconnect();
    bool statusConnect;

protected:
    DBParams DBParams;
    QSqlDatabase db;
    void setDBParams(QString, QString, QString, int, QString, QString);

};

#endif // DBORIONENGINE_H
