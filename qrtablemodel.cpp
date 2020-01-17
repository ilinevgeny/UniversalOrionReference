#include "qrtablemodel.h"
#include "ui_qrtablemodel.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QList>

#include <QPushButton>
#include <QSpacerItem>
#include <QStyledItemDelegate>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QHeaderView>

#define ConfigX                   "\"ConfigX\""
//#define TABLE                   "\"testuuidqtmodel3\""
#define ConceptX                "\"ConceptX\""

void qRTableModel::keyPressEvent(QKeyEvent *event) {
  //qDebug() << event->key();
   if (event->type()==QEvent::KeyPress)
   {
       if ((event->key()==Qt::Key_Return) && (event->modifiers()==Qt::CTRL))
       {
           //qDebug() << "Pressed!";
           //this->modelMain->submitAll();

           this->modelMain->database().transaction();
           if (this->modelMain->submitAll()) {
               this->modelMain->database().commit();

               std::set<editableItemStruct>::iterator itr;
               for (itr = ddeProxyModel::editableSet.begin(); itr != ddeProxyModel::editableSet.end(); ++itr)
               {
//                   qDebug() << itr->row;
                   this->mainTbn->setItemDelegateForRow(itr->row, new QStyledItemDelegate);
               }
               ddeProxyModel::editableSet.clear();

           } else {
               this->modelMain->database().rollback();
               QMessageBox::warning(this, tr("Cached Table"),
                                    tr("The database reported an error: %1")
                                   .arg(this->modelMain->lastError().text()));
           }
       }
   }
  //emit keyCaught(event);
}
QTableView * qRTableModel::setupTable(QString tableName)
{
    QTableView *table;
    QTableView *tmp = this->findChild<QTableView *>(tableName);
    delete  tmp;

    table = new QTableView(this);
    table->setObjectName(tableName);
   return table;
}

qRTableModel::qRTableModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qRTableModel)
{
    ui->setupUi(this);
    this->setWindowTitle("Прототип редактора");

    AppSettings *dbSettings = new AppSettings();
    dbSettings->setSettings("QPSQL", "localhost", "orionconcept", "5432", "postgres", "orion");
    DBOrionEngine *db = new DBOrionEngine(dbSettings);
    db->Connect();

    this->centralLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    this->setLayout(centralLayout);
    this->mainTLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    this->resize(1200, 500);

//    QMessageBox::critical(0, tr("Error: Select failure!"),  model->lastError().text());

    QMap<QString, QString> *mainHeaders = new QMap<QString, QString>();
    mainHeaders->insert("Имя концепта", "CFName");
    mainHeaders->insert( "Концепт", "CFConceptExt");
    mainHeaders->insert( "ID концепта", "CFUUID");

    mainTbn = this->setupMainModel(ConfigX, mainHeaders);
    this->setStyleSheet("* { background-color: rgb(50, 50, 50); border: 0px solid #ccc; padding: 0px; margin: 0px} ");
    this->setBackgroundRole(QPalette::Dark);

    //QSpacerItem *spaser = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //relTLayout->addItem(spaser);

    this->mainTLayout->addWidget(mainTbn);
    //this->mainTLayout->addWidget(relatedTbn);
    centralLayout->addLayout(mainTLayout);

    QItemSelectionModel *sm = mainTbn->selectionModel();
    connect(sm, SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(on_tableViewTriggerSelectionModel_currentRowChanged(const QModelIndex &)));

    //connect( mainTbn, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(editCell(const QModelIndex&)) );
    //connect(mainTbn, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
//   connect(this->modelMain, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
//            mainTbn, SLOT(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));

    connect(this->modelMain, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));
}

void qRTableModel::onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{

    //qDebug() << "the cell has changed" << topLeft.model()->columnCount() << topLeft.data()<< bottomRight.data().toString();
    bgDelegate *delegate = new bgDelegate(this->mainTbn);
    this->mainTbn->setItemDelegateForRow(topLeft.row(), delegate);
    ddeProxyModel::editableSet.insert({topLeft.row(), topLeft.column()});

}

void qRTableModel::on_tableViewTriggerSelectionModel_currentRowChanged(const QModelIndex &index)
{
   this->onTableClicked(index);
}
void qRTableModel::onTableClicked(const QModelIndex &index)
{
    this->mainTLayout->removeWidget(this->relatedTbn);
    if (index.isValid()) {
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
    QHeaderView *verticalHeader = table->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(20);
    table->setStyleSheet(QString("QTableCornerButton::section {background-color: #555; } "
                                       "QHeaderView::section {background-color: #555555; color: #ffffff; } "
                                       "QTableView {color: #ffffff;} "
                                       "QTableView::item {color: #ff6666;} "
                                       "QTableView::item::selected {background: #e5e5e5; color: #000000} "
                                       "QLineEdit {background: #ff6666; color: #ffffff}"));
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
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
}

QTableView * qRTableModel::setupMainModel(const QString &tableName, QMap<QString, QString> *mainHeaders)
{

    this->modelMain = new ddeProxyModel(this);
    this->modelMain->clear();
    this->modelMain->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->modelMain->setTable(tableName);
    this->modelMain->setSort(3, Qt::AscendingOrder);
//    this->modelMain->setFilter("\"CFConceptExt\" = 1000 OR \"CFConceptExt\" = 1300 OR \"CFConceptExt\" = 1400" );
    this->modelMain->setFilter("\"CFConceptExt\" BETWEEN 1000 AND 10000" );
    this->modelMain->select();

    QTableView *tbn = this->setupTable("ConfigXTable");

    this->initiateTable(tbn, this->modelMain, mainHeaders);
    return tbn;

}


QTableView * qRTableModel::setupRelatedModel(const QString &tableName, QMap<QString, QString> *headers, const QString &filter)
{

    this->mRelated = new ddeProxyModel(this);
    this->mRelated->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->mRelated->setTable(tableName);
    //this->mRelated->setRelation(2, QSqlRelation(TABLE, "\"CFConceptExt\"", "\"CFName\""));
    this->mRelated->setSort(0, Qt::AscendingOrder);
    this->mRelated->setFilter(filter);
    this->mRelated->select();
    QTableView *tbn = this->setupTable("ConceptXTable");

    this->initiateTable(tbn, this->mRelated, headers);
    return tbn;
}

qRTableModel::~qRTableModel()
{
    delete ui;
}
