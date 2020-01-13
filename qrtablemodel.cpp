#include "qrtablemodel.h"
#include "ui_qrtablemodel.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QList>
#include <QBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#define TABLE                   "\"ConfigX\""
#define ConceptX                "\"ConceptX\""
#define DEVICE_IP               "IP"
#define DEVICE_HOSTNAME         "Hostname"


QTableView * qRTableModel::setupTable(QString tableName)
{
   QTableView *table = new QTableView(this);
   table->setObjectName(tableName);
   return table;
}

qRTableModel::qRTableModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qRTableModel)
{
    ui->setupUi(this);

    AppSettings *dbSettings = new AppSettings();
    dbSettings->setSettings("QPSQL", "localhost", "orionconcept", "5432", "postgres", "orion");
    DBOrionEngine *db = new DBOrionEngine(dbSettings);
    db->Connect();

    /* @TODO: attach layout to Widget class and use in setup model function
     *
     */

//    QMessageBox::critical(0, tr("Error: Select failure!"),  model->lastError().text());

    QMap<QString, QString> *mainHeaders = new QMap<QString, QString>();
    mainHeaders->insert("columnName_1", "CFName");
    mainHeaders->insert( "columnName_2", "CFConceptExt");
    QTableView * mainTbn = this->setupMainModel(TABLE, mainHeaders);
    QBoxLayout *mainTLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    //QSpacerItem *spaser = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //relTLayout->addItem(spaser);

    mainTLayout->addWidget(mainTbn);

    QMap<QString, QString> *relatedHeaders = new QMap<QString, QString>();
    relatedHeaders->insert("RelcolumnName_1", "CName");
    relatedHeaders->insert( "RelcolumnName_2", "CFName");
    QTableView *relatedTbn = this->setupRelatedModel(ConceptX, relatedHeaders);

    mainTLayout->addWidget(relatedTbn);

    QBoxLayout *centralLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    centralLayout->addLayout(mainTLayout);
    this->setLayout(centralLayout);
    this->resize(900, 300);

    connect(mainTbn, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
}

void qRTableModel::onTableClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString cellText = index.data().toString();
        qDebug() << cellText;
    }
}

void qRTableModel::initiateTable(QTableView *table, QSqlRelationalTableModel *model, QMap<QString, QString> *headers = nullptr)
{

    table->setModel(model);
    qDebug() << model->columnCount();
    for(int i = 0; i < model->columnCount(); i++)
    {
         if(headers->key(model->headerData(i, Qt::Horizontal).toString()) != "")
         {
              model->setHeaderData(i, Qt::Horizontal, headers->key(model->headerData(i, Qt::Horizontal).toString()));
         }
         else
         {
             table->setColumnHidden(model->fieldIndex(model->headerData(i, Qt::Horizontal).toString()), true);
         }
    }

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->resizeColumnsToContents();
    table->setItemDelegate(new QSqlRelationalDelegate(table));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    table.horizontalHeader()->setStretchLastSection(true);
}

QTableView * qRTableModel::setupMainModel(const QString &tableName, QMap<QString, QString> *mainHeaders)
{
    this->modelMain = new QSqlRelationalTableModel(this);
    this->modelMain->setTable(tableName);

    /* Устанавливаем связи с таблицей устройств, по которым будет производится
     * подстановка данных
     * В метода setRelation указывается номер колонки, в которой будет
     * производится подстановка, а также с помощью класса
     * QSqlRelation указывается имя таблицы,
     * параметр, по которому будет произведена выборка строки
     * и колонка, из которой будут взяты данные
     * */

//    modelMain->setRelation(3, QSqlRelation(ConceptX, "C", DEVICE_HOSTNAME));
//    modelMain->setRelation(4, QSqlRelation(ConceptX, "id", DEVICE_IP));

    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    this->modelMain->setSort(3, Qt::AscendingOrder);
    this->modelMain->setFilter("\"CFConceptExt\" BETWEEN 1000 AND 11000");
    this->modelMain->select(); // Делаем выборку данных из таблицы

    QTableView *tbn = this->setupTable("ConfigXTable");

    this->initiateTable(tbn, this->modelMain, mainHeaders);
    return tbn;

}


QTableView * qRTableModel::setupRelatedModel(const QString &tableName, QMap<QString, QString> *headers)
{

    this->mRelated = new QSqlRelationalTableModel(this);
    this->mRelated->setTable(tableName);
    this->mRelated->setRelation(2, QSqlRelation(TABLE, "\"CFConceptExt\"", "\"CFName\""));
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    this->mRelated->setSort(0,Qt::AscendingOrder);
//    this->mRelated->setFilter("\"CConcept\" = 1300");
    this->mRelated->select(); // Делаем выборку данных из таблицы

    QTableView *tbn = this->setupTable("ConceptXTable");

    this->initiateTable(tbn, this->mRelated, headers);
    return tbn;
}

qRTableModel::~qRTableModel()
{
    delete ui;
}
