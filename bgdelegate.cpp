#include "bgdelegate.h"

bgDelegate::bgDelegate(QObject *parent)
{

}

void bgDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
   // qDebug() << index.data().toString();
    QColor *colorEdit = new QColor(QColor::fromRgb(1, 200, 255));
    colorEdit->setAlpha(80);
    painter->fillRect(option.rect,  *colorEdit);
    QStyledItemDelegate::paint(painter, option, index);
}
