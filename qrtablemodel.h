#ifndef QRTABLEMODEL_H
#define QRTABLEMODEL_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QTableView>
#include <QBoxLayout>
#include <QModelIndex>
//#include <QSqlRelation>

#include "appsettings.h"
#include "dborionengine.h"

namespace Ui {
class qRTableModel;
}

class qRTableModel : public QWidget
{
    Q_OBJECT

public:
    explicit qRTableModel(QWidget *parent = nullptr);
    ~qRTableModel();
    QSqlRelationalTableModel    *modelMain;
    QSqlRelationalTableModel    *mRelated;
    QSqlTableModel  *model;
    QBoxLayout *centralLayout;
    QBoxLayout *mainTLayout;
    void initiateTable(QTableView *table, QSqlRelationalTableModel  *model, QMap<QString, QString> *headers);
private:
    Ui::qRTableModel *ui;
    QTableView * setupTable(QString tableName);
    QTableView * setupMainModel(const QString &tableName, QMap<QString, QString> *headers);
    QTableView * setupRelatedModel(const QString &tableName, QMap<QString, QString> *headers, const QString &filter = "");
    QTableView *relatedTbn;
private slots:
    void onTableClicked(const QModelIndex &);
    void on_tableViewTriggerSelectionModel_currentRowChanged(const QModelIndex &);

};

#endif // QRTABLEMODEL_H
