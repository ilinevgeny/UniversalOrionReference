#include "qrtablemodel.h"
#include "ui_qrtablemodel.h"

qRTableModel::qRTableModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::qRTableModel)
{
    ui->setupUi(this);
}

qRTableModel::~qRTableModel()
{
    delete ui;
}
