#include "qrtablemodel.h"
#include "ui_qrtablemodel.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QList>

#include <QPushButton>
#include <QSpacerItem>
#define TABLE                   "\"ConfigX\""
#define ConceptX                "\"ConceptX\""
#define DEVICE_IP               "IP"
#define DEVICE_HOSTNAME         "Hostname"


QTableView * qRTableModel::setupTable(QString tableName)
{
    QTableView *table;
    qDebug() << this->findChild<QTableView *>(tableName);
    QTableView *tmp = this->findChild<QTableView *>(tableName);
    delete  tmp;

    table = new QTableView(this);
    table->setObjectName(tableName);


   /*int isExist = this->findChildren<QTableView>(tableName).count();
    qDebug() << isExist;
   if( 0 >= isExist)
   {
       qDebug() << tableName;
       table = new QTableView(this);
       table->setObjectName(tableName);
   }
   else
   {
       //qDebug() << this->findChild<QTableView *>(tableName);
       //return this->findChild<QTableView *>(tableName);
   } */
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

    this->centralLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    this->setLayout(centralLayout);
    this->mainTLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    this->resize(1100, 300);
    /* @TODO: attach layout to Widget class and use in setup model function
     *
     */

//    QMessageBox::critical(0, tr("Error: Select failure!"),  model->lastError().text());

    QMap<QString, QString> *mainHeaders = new QMap<QString, QString>();
    mainHeaders->insert("Имя концепта", "CFName");
    mainHeaders->insert( "Концепт", "CFConceptExt");
    QTableView * mainTbn = this->setupMainModel(TABLE, mainHeaders);

    mainTbn->setSelectionMode(QAbstractItemView::SingleSelection);
    mainTbn->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainTbn->setEditTriggers(QAbstractItemView::DoubleClicked);

            //setEditTriggers( QAbstractItemView::DoubleClicked );



    //QSpacerItem *spaser = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //relTLayout->addItem(spaser);

    this->mainTLayout->addWidget(mainTbn);



    //this->mainTLayout->addWidget(relatedTbn);
    centralLayout->addLayout(mainTLayout);

    QItemSelectionModel *sm = mainTbn->selectionModel();
    connect(sm, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(on_tableViewTriggerSelectionModel_currentRowChanged(const QModelIndex &)));

    //connect( mainTbn, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(editCell(const QModelIndex&)) );
    //connect(mainTbn, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));

//    connect(this->modelMain, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
//            mainTbn, SLOT(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));
}

void qRTableModel::on_tableViewTriggerSelectionModel_currentRowChanged(const QModelIndex &index)
{
   this->onTableClicked(index);
}
void qRTableModel::onTableClicked(const QModelIndex &index)
{
    qDebug() << "is SELECTED";
    this->mainTLayout->removeWidget(this->relatedTbn);
//192.168.0.50

    if (index.isValid()) {
//        QString cellText = index.data().toString();
        QString params = index.model()->index(index.row(), this->modelMain->fieldIndex("CFConceptExt")).data().toString();
        QString query = "\"CConcept\" = " + params;
        QMap<QString, QString> *relatedHeaders = new QMap<QString, QString>();
        relatedHeaders->insert("Значения концепта", "CName");
        relatedHeaders->insert( "Концепт", "CFName");
        this->relatedTbn = this->setupRelatedModel(ConceptX, relatedHeaders, query);
    }
    this->mainTLayout->addWidget(this->relatedTbn);
}

void qRTableModel::initiateTable(QTableView *table, QSqlRelationalTableModel *model, QMap<QString, QString> *headers = nullptr)
{

    table->setModel(model);
    //qDebug() << model->columnCount();
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
//    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    table.horizontalHeader()->setStretchLastSection(true);
}

QTableView * qRTableModel::setupMainModel(const QString &tableName, QMap<QString, QString> *mainHeaders)
{
    this->modelMain = new QSqlRelationalTableModel(this);
    //    this->modelMain->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
        this->modelMain->setEditStrategy(QSqlTableModel::OnRowChange);
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


QTableView * qRTableModel::setupRelatedModel(const QString &tableName, QMap<QString, QString> *headers, const QString &filter)
{

    this->mRelated = new QSqlRelationalTableModel(this);
    this->mRelated->setTable(tableName);
    //this->mRelated->setRelation(2, QSqlRelation(TABLE, "\"CFConceptExt\"", "\"CFName\""));
    this->mRelated->setSort(0, Qt::AscendingOrder);
//    this->mRelated->setFilter(filter);
    this->mRelated->setFilter(filter);
    this->mRelated->select(); // Делаем выборку данных из таблицы
    QTableView *tbn = this->setupTable("ConceptXTable");

    this->initiateTable(tbn, this->mRelated, headers);
    return tbn;
}

qRTableModel::~qRTableModel()
{
    delete ui;
}
