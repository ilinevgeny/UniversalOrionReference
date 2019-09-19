#include "referencewindow.h"
#include "ui_referencewindow.h"
#include <QDebug>
ReferenceWindow::ReferenceWindow(int conceptId, QJsonDocument conf, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReferenceWindow)
{
    ui->setupUi(this);

    ConceptSettings *settings = new ConceptSettings(conf);
    settings->load();
    QVariantList visibility = settings->settingsArr["VISIBILITY"]["FIELDS"].toArray().toVariantList();
    qDebug() << visibility;

     for(int i = 0; i < settings->settingsArr["DESIGN"]["FIELDS_GEO"].toArray().size(); i++)
     {
         //settings->settingsArr["DESIGN"]["FIELDS_GEO"].toArray().at(i).toObject();
         for(QString key : settings->settingsArr["DESIGN"]["FIELDS_GEO"].toArray().at(i).toObject().keys())
         {
             if(visibility.contains(key)) {
                qDebug() << settings->settingsArr["DESIGN"]["FIELDS_GEO"].toArray().at(i).toObject().value(key).toArray();
                int x = settings->settingsArr["DESIGN"]["FIELDS_GEO"].toArray().at(i).toObject().value(key).toArray().at(0).toInt();
                int y = settings->settingsArr["DESIGN"]["FIELDS_GEO"].toArray().at(i).toObject().value(key).toArray().at(1).toInt();
                if(key == "CName" || key == "CInt1"){
                    QLineEdit *lineEdit = new QLineEdit(this);
                    lineEdit->setGeometry(x, y, 150, 20);
                    lineEdit->setPlaceholderText(key);
                    lineEdit->show();
                } else {
                    QLabel *label = new QLabel(this);
                    label->setGeometry(x, y, 150, 20);
                    label->setText(key);
                    label->setFrameShape(QFrame::Box);
                    label->show();
                }

             }
         }
     }


    QString strQuery;
    QSqlQuery query;
    strQuery = "SELECT * FROM \"ConceptX\" WHERE \"CConcept\" =" + QString::number(conceptId);

    if (query.exec(strQuery))
    {
        if(query.size() > 0){


            while (query.next()) {

                qDebug() << query.value("CName").toString();
            }
        } else {
          //  ui->statusBar->showMessage("Запрос не вернул результат");
          //  statusBar()->setStyleSheet("color: red");
        }
    } else {
        //ui->statusBar->showMessage("Ошибка БД: " + query.lastError().text());
        //statusBar()->setStyleSheet("color: red");
    }
}

ReferenceWindow::~ReferenceWindow()
{
    delete ui;
}
