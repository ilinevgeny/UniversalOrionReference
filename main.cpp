#include "mainwindow.h"
#include "qrtablemodel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    qRTableModel tblm;
    tblm.show();
    return a.exec();
}
