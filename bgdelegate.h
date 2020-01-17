#ifndef BGDELEGATE_H
#define BGDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QDebug>
#include <QTableView>
#include <QPushButton>

class bgDelegate : public QStyledItemDelegate
{
public:
    explicit bgDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
//       void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
//       void setModelData(QWidget *editor, QAbstractItemModel *modal, const QModelIndex &index) const Q_DECL_OVERRIDE;
//       void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool eventFilter(QObject *editor, QEvent *event) const;

private:
    QTableView *myView;
    QPushButton *btn;
    bool isOneCellInEditMode;
    QPersistentModelIndex currentEditedCellIndex;
};

#endif // BGDELEGATE_H
