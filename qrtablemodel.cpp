#include "qrtablemodel.h"
#include "ui_qrtablemodel.h"

#define TABLE                   "ConfigX"
#define ConceptX                  "ConceptX"
#define DEVICE_IP               "IP"
#define DEVICE_HOSTNAME         "Hostname"

qRTableModel::qRTableModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qRTableModel)
{

    ui->setupUi(this);

    AppSettings *dbSettings = new AppSettings();
    dbSettings->setSettings("QPSQL", "localhost", "orionconcept", "5432", "postgres", "orion");
    DBOrionEngine *db = new DBOrionEngine(dbSettings);
    db->Connect();

    this->setupMainModel(TABLE,
                     QStringList() << tr("CFConceptExt")
                                   << tr("CFName")

               );

    this->setupDeviceModel(ConceptX,
                     QStringList() << tr("CUUID")
                                   << tr("CName")

               );
    ui->tableView->setModel(modelMain);     // Устанавливаем модель на TableView
//    ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    modelMain->select(); // Делаем выборку данных из таблицы

    ui->tableViewDevice->setModel(modelDevice);     // Устанавливаем модель на TableView
//    ui->tableViewDevice->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->tableViewDevice->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableViewDevice->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableViewDevice->resizeColumnsToContents();
    ui->tableViewDevice->setItemDelegate(new QSqlRelationalDelegate(ui->tableViewDevice));
    ui->tableViewDevice->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewDevice->horizontalHeader()->setStretchLastSection(true);

    modelDevice->select();


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
    modelMain = new QSqlRelationalTableModel(this);
    modelMain->setTable(tableName);
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
    for(int i = 0, j = 0; i < modelMain->columnCount(); i++, j++){
        modelMain->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    modelMain->setSort(0,Qt::AscendingOrder);
    modelMain->select(); // Делаем выборку данных из таблицы
}


void qRTableModel::setupDeviceModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */
    modelDevice = new QSqlRelationalTableModel(this);
    modelDevice->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < modelDevice->columnCount(); i++, j++){
        modelDevice->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    modelDevice->setSort(0,Qt::AscendingOrder);
    modelDevice->select(); // Делаем выборку данных из таблицы
}
