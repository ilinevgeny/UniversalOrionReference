#include "qrtablemodel.h"
#include "ui_qrtablemodel.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QList>
#define TABLE                   "\"ConfigX\""
#define ConceptX                "\"ConceptX\""
#define DEVICE_IP               "IP"
#define DEVICE_HOSTNAME         "Hostname"

void qRTableModel::setupModel(const QString &tableName, const QStringList &headers)
{

    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */

    model = new QSqlTableModel(this);
    model->setTable("\"ConfigX\"");
   // model->select();

    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
     qDebug() << "columns" << model->columnCount();
//    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
//        model->setHeaderData(i, Qt::Horizontal,headers[j]);
//        qDebug() << headers[j];
//    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    model->setSort(0,Qt::AscendingOrder);
}



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


//    QMessageBox::critical(0, tr("Error: Select failure!"),  model->lastError().text());

    this->setupMainModel(TABLE, QStringList() << tr("CFConceptExt") << tr("CFName"));
    this->setupConfigXModel(ConceptX, QStringList() << tr("CUUID") << tr("CName"));

//    this->setupTable(new QTableView(this));
//    modelMain->select(); // Делаем выборку данных из таблицы

    ui->tableViewDevice->setModel(mConfigx);     // Устанавливаем модель на TableView
    // Разрешаем выделение строк
    ui->tableViewDevice->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableViewDevice->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableViewDevice->resizeColumnsToContents();
    ui->tableViewDevice->setItemDelegate(new QSqlRelationalDelegate(ui->tableViewDevice));
    ui->tableViewDevice->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewDevice->horizontalHeader()->setStretchLastSection(true);
//    modelDevice->select();
}

void qRTableModel::initiateTable(QTableView &table, QList<QString> * hColumns = nullptr)
{
    for(QString columname : *hColumns)
    {
        table.setColumnHidden(this->modelMain->fieldIndex(columname), true);

    }

    table.setSelectionBehavior(QAbstractItemView::SelectRows);
    table.setSelectionMode(QAbstractItemView::SingleSelection);
    table.resizeColumnsToContents();
    table.setItemDelegate(new QSqlRelationalDelegate(&table));
    table.setEditTriggers(QAbstractItemView::NoEditTriggers);
    table.horizontalHeader()->setStretchLastSection(true);
}

qRTableModel::~qRTableModel()
{
    delete ui;
}


void qRTableModel::setupMainModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */
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

    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
//    for(int i = 0, j = 0; i < modelMain->columnCount(); i++, j++){
//        modelMain->setHeaderData(i,Qt::Horizontal,headers[j]);
//    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    this->modelMain->setSort(3, Qt::AscendingOrder);
    this->modelMain->setFilter("\"CFConceptExt\" BETWEEN 1000 AND 10000");
    this->modelMain->select(); // Делаем выборку данных из таблицы

    QTableView *tbn = this->setupTable("ConfigXTable");
    tbn->setModel(this->modelMain);

    QList<QString> *hColumnsList = new QList<QString>(QStringList() << "CFUUID" << "CFConcept" << "CFUUID1" << "CFFieldName" << "CFConf" <<  "CFEngName" << "CFFlag" << "CFIcon" << "CFParent");
    this->initiateTable(*tbn, hColumnsList);
    delete hColumnsList;

    /* Пример
        // Разрешаем выделение строк
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        // Устанавливаем режим выделения лишь одно строки в таблице
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        // Устанавливаем размер колонок по содержимому
        ui->tableView->resizeColumnsToContents();
        ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
       // ui->tableView->horizontalHeader()->setStretchLastSection(true);
    */
}


void qRTableModel::setupConfigXModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */
    this->mConfigx = new QSqlRelationalTableModel(this);
    this->mConfigx->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
//    for(int i = 0, j = 0; i < modelDevice->columnCount(); i++, j++){
//        modelDevice->setHeaderData(i,Qt::Horizontal,headers[j]);
//    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    this->mConfigx->setSort(0,Qt::AscendingOrder);
    this->mConfigx->select(); // Делаем выборку данных из таблицы

    QTableView *tbn = this->setupTable("ConfigXTable");
    tbn->setModel(this->modelMain);
}
