#include "qrtablemodel.h"
#include "ui_qrtablemodel.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QList>

#include <QPushButton>
#include <QSpacerItem>
#define TABLE                   "\"ConfigX\""
//#define TABLE                   "\"testuuidqtmodel3\""
#define ConceptX                "\"ConceptX\""
#define DEVICE_IP               "IP"
#define DEVICE_HOSTNAME         "Hostname"

void qRTableModel::keyPressEvent(QKeyEvent *event) {
//  printf("\nkey event in board: %i", event->key());
  qDebug() << event->key();
   if (event->type()==QEvent::KeyPress)
   {
       if ((event->key()==Qt::Key_Return) && (event->modifiers()==Qt::CTRL))
       {
           qDebug() << "Pressed!";
           this->modelMain->submitAll();
       }

   }
  //emit keyCaught(event);
}
QTableView * qRTableModel::setupTable(QString tableName)
{
    QTableView *table;
    //qDebug() << this->findChild<QTableView *>(tableName);
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
//    mainTbn->setEditTriggers(QAbstractItemView::DoubleClicked);

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

    connect(this->modelMain, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));
}

void qRTableModel::onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{

    qDebug() << "the cell has changed" << topLeft.model()->columnCount() << topLeft.data()<< bottomRight.data().toString();
    QColor rowColor = Qt::blue;
    QBrush yellow;
    yellow.setColor(Qt::yellow);
    //bool result;
    //result = this->modelMain->setData(this->modelMain->index(0,1), QBrush(Qt::red),Qt::BackgroundRole);
    //QAbstractItemModel *aim =  this->modelMain;
    //aim->setData(aim->index(1, 2), topLeft.data().toString());
    //aim->submit();

    //qDebug() << this->modelMain->index(0,2).data().toString();
//    result = this->modelMain->setData(topLeft, topLeft.data());
   // result = this->modelMain->setData(this->modelMain->index(0,2), this->modelMain->index(0,2).data().toString());

    //qDebug() << result;
    //this->modelMain->submitAll();
    qDebug() << this->modelMain->lastError().text() << this->modelMain->lastError().number();

}

void qRTableModel::on_tableViewTriggerSelectionModel_currentRowChanged(const QModelIndex &index)
{
   this->onTableClicked(index);
}
void qRTableModel::onTableClicked(const QModelIndex &index)
{
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

void qRTableModel::initiateTable(QTableView *table, QSqlTableModel *model, QMap<QString, QString> *headers = nullptr)
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
            // table->setColumnHidden(model->fieldIndex(model->headerData(i, Qt::Horizontal).toString()), true);
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
//    this->modelMain = new QSqlRelationalTableModel(this);

    this->modelMain = new QSqlTableModel(this);
    this->modelMain->clear();
        this->modelMain->setEditStrategy(QSqlTableModel::OnManualSubmit);
//        this->modelMain->setEditStrategy(QSqlTableModel::OnFieldChange);
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
//     qDebug() << "rowCount() = " <<  this->modelMain->rowCount();


    QTableView *tbn = this->setupTable("ConfigXTable");

    this->initiateTable(tbn, this->modelMain, mainHeaders);
    return tbn;

}


QTableView * qRTableModel::setupRelatedModel(const QString &tableName, QMap<QString, QString> *headers, const QString &filter)
{

//    this->mRelated = new QSqlRelationalTableModel(this);
    this->mRelated = new QSqlTableModel(this);
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
