#ifndef DDEPROXYMODEL_H
#define DDEPROXYMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QColor>
#include "set"
#include <QDebug>

struct editableItemStruct
{
    int row;
    int col;
    bool operator<(const editableItemStruct& rv) const
    {
        return row < rv.row || (row == rv.row && col < rv.col);
    }
};

class ddeProxyModel : public QSqlTableModel
{
public:
    explicit ddeProxyModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    static std::set<editableItemStruct> editableSet;
};

#endif // DDEPROXYMODEL_H

