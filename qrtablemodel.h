#ifndef QRTABLEMODEL_H
#define QRTABLEMODEL_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
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
    QSqlRelationalTableModel    *modelDevice;

private:
    Ui::qRTableModel *ui;
    void setupMainModel(const QString &tableName, const QStringList &headers);
    void setupDeviceModel(const QString &tableName, const QStringList &headers);
};

#endif // QRTABLEMODEL_H
