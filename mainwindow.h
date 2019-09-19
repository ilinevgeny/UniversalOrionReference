#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define APPDEBUG true

#include <QMainWindow>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>
#include "appsettings.h"
#include "dborionengine.h"
#include "referencewindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static QString sqlStringCast(QString sqlquery);
private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;

private slots:
    void on_action_triggered();
    void on_refSearchButton_clicked();
    void on_lineEdit_returnPressed();
    void on_refsList_table_doubleClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
