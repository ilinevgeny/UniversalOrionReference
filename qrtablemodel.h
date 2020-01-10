#ifndef QRTABLEMODEL_H
#define QRTABLEMODEL_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QTableView>
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
    QSqlRelationalTableModel    *mConfigx;
    QSqlTableModel  *model;
    void initiateTable(QTableView &table, QList<QString> * hColumns);
private:
    Ui::qRTableModel *ui;
    QTableView * setupTable(QString tableName);
    void setupMainModel(const QString &tableName, const QStringList &headers);
    void setupConfigXModel(const QString &tableName, const QStringList &headers);
    void setupModel(const QString &tableName, const QStringList &headers);
};

#endif // QRTABLEMODEL_H
