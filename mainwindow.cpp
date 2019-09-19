#include "mainwindow.h"
#include "ui_mainwindow.h"
#define  MODEL_CONF 35
#include <QStandardItem>
#include <QJsonDocument>
#include <QMap>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AppSettings *dbSettings = new AppSettings();
    dbSettings->setSettings("QPSQL", "localhost", "orionconcept", "5432", "postgres", "orion");
    DBOrionEngine *db = new DBOrionEngine(dbSettings);
    db->Connect();
    this->on_refSearchButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    AppSettings *s = new AppSettings(this);
    s->show();
}

void MainWindow::on_refSearchButton_clicked()
{
    ui->statusBar->showMessage("");
    ui->statusBar->setStyleSheet("color: unset");

    QString input = ui->lineEdit->text();
    QString strQuery;
    QSqlQuery query;
    strQuery = "SELECT \"CFName\", \"CFConceptExt\", \"CFConf\" FROM \"ConfigX\" WHERE \"CFConcept\" = 1 AND \"CFName\" Ilike '%" + input + "%'";

    int columnCount = 1;
    //QStandardItemModel *model = new QStandardItemModel();
    this->model = new QStandardItemModel();
    this->model->setColumnCount(columnCount);
    this->model->setHorizontalHeaderItem(0, new QStandardItem(QString("Список справочников")));
    QMap <int, QVariant> queryMap;
    if (query.exec(strQuery))
    {
        if(query.size() > 0){

             this->model->setRowCount(query.size());
            int row = 0;
            while (query.next()) {

                this->model->setData(model->index(row, 0), query.value("CFName").toString());
                this->model->setData(model->index(row, 0), query.value("CFConceptExt").toInt(), Qt::UserRole);
                queryMap[query.value("CFConceptExt").toInt()] = query.value("CFConf");
                this->model->setData(model->index(row, 0), query.value("CFConf"), MODEL_CONF);
                //this->model->setItem(row, 0, new QStandardItem(query.value("CFConceptExt").toInt()));
                row++;
            }
        } else {
            ui->statusBar->showMessage("Запрос не вернул результат");
            statusBar()->setStyleSheet("color: red");
        }
    } else {
        ui->statusBar->showMessage("Ошибка БД: " + query.lastError().text());
        statusBar()->setStyleSheet("color: red");
    }
    ui->refsList_table->setModel(model);
    for (int i = 0; i < columnCount; i++ ) {
         ui->refsList_table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    ui->refsList_table->setStyleSheet("QHeaderView::section { background-color: #ccc }");

}

QString MainWindow::sqlStringCast(QString sqlquery)
{
    sqlquery.replace("'","\"");
    return sqlquery;
}

void MainWindow::on_lineEdit_returnPressed()
{
    this->on_refSearchButton_clicked();
}

void MainWindow::on_refsList_table_doubleClicked(const QModelIndex &index)
{

    QJsonDocument conf = QJsonDocument::fromJson(this->model->index(index.row(),0).data(MODEL_CONF).toByteArray());
    int refConceptExt = this->model->index(index.row(),0).data(Qt::UserRole).toInt();
    ReferenceWindow *refW = new ReferenceWindow(refConceptExt, conf);
    refW->setWindowTitle("Справочник " + this->model->index(index.row(),0).data().toString());
    refW->show();

}
