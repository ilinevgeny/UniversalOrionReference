#ifndef QRTABLEMODEL_H
#define QRTABLEMODEL_H

#include <QWidget>

namespace Ui {
class qRTableModel;
}

class qRTableModel : public QWidget
{
    Q_OBJECT

public:
    explicit qRTableModel(QWidget *parent = nullptr);
    ~qRTableModel();

private:
    Ui::qRTableModel *ui;
};

#endif // QRTABLEMODEL_H
