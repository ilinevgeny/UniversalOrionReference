#include "ddeproxymodel.h"

std::set<editableItemStruct> ddeProxyModel::editableSet = {{-1,-1}};

ddeProxyModel::ddeProxyModel(QObject *parent, QSqlDatabase db) : QSqlTableModel(parent, db)
{

}

QVariant ddeProxyModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlTableModel::data(index, role);

    if (role == Qt::TextColorRole && index.column() == 2)
    {
            //return QVariant::fromValue(QColor(Qt::yellow));

    }
    if (role == Qt::BackgroundColorRole && index.column() == 2)
    {
           // return QVariant::fromValue(QColor(Qt::black));
    }

    std::set<editableItemStruct>::iterator itr;
    itr = ddeProxyModel::editableSet.find({index.row(), index.column()});
    if (itr != ddeProxyModel::editableSet.end() && role == Qt::BackgroundColorRole)
    {
        //qDebug() << "there are! " << endl << index.row() << "    " <<  index.column();
        return QVariant::fromValue(QColor(Qt::yellow));

    }
    return value;
}


bool ddeProxyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return QSqlTableModel::setData(index, value, role);
}
