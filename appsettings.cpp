#include "appsettings.h"
#include "ui_appsettings.h"
#include <QString>
#include <QDebug>
#include "dborionengine.h"

AppSettings::AppSettings(QWidget *parent) :
    QWidget (parent),
    ui(new Ui::AppSettings)
{
    this->dbSettings = new QSettings("BOLID", "searchApp");

    ui->setupUi(this);

    ui->driver->setText(this->dbSettings->value("DB/Driver").toString());
    ui->server->setText(this->dbSettings->value("DB/Server").toString());
    ui->dbname->setText(this->dbSettings->value("DB/DBName").toString());
    ui->port->setText(this->dbSettings->value("DB/Port").toString());
    ui->user->setText(this->dbSettings->value("DB/User").toString());
    ui->password->setText(this->dbSettings->value("DB/Password").toString());

    this->correctSettings = false;
    this->setWindowFlags(Qt::Window);

    QObject::connect(ui->pushButton_2, SIGNAL(clicked()),this, SLOT(saveButton()));
}

AppSettings::~AppSettings()
{
    delete ui;
}
void AppSettings::setSettings(QString driver, QString server, QString dbname, QString port, QString user, QString pass)
{
    this->dbSettings->setValue("DB/Driver", driver);
    this->dbSettings->setValue("DB/Server", server);
    this->dbSettings->setValue("DB/DBName", dbname);
    this->dbSettings->setValue("DB/Port", port);
    this->dbSettings->setValue("DB/User", user);
    this->dbSettings->setValue("DB/Password", pass);
}
QString AppSettings::getSetting(QString setName)
{
    return dbSettings->value(setName).toString();
}

void AppSettings::on_pushButton_clicked()
{
    this->close();
}

void AppSettings::saveButton()
{
    this->setSettings(ui->driver->text(), ui->server->text(), ui->dbname->text(), ui->port->text(), ui->user->text(), ui->password->text());
    DBOrionEngine *db = new DBOrionEngine(this);
    db->Connect();
    if(db->statusConnect)
    {
        this->close();
    }
    if(APPDEBUG) {
        qDebug() << this->dbSettings->value("DB/Driver").toString();
    }
   this->correctSettings = true;
   if(!this->correctSettings) {
        ui->textBrowser->setHtml("<b style=\"color: red\">Заданы неправильные параметры</b>");
   } else {

   }

}


