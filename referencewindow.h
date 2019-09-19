#ifndef REFERENCEWINDOW_H
#define REFERENCEWINDOW_H

#include <QWidget>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include "conceptsettings.h"

namespace Ui {
class ReferenceWindow;
}

class ReferenceWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ReferenceWindow(int conceptId, QJsonDocument conf, QWidget *parent = nullptr);
    ~ReferenceWindow();

private:
    Ui::ReferenceWindow *ui;
};

#endif // REFERENCEWINDOW_H
