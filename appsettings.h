#ifndef APPSETTINGS_H
#define APPSETTINGS_H
#define  APPDEBUG true

#include <QWidget>
#include <QDialog>
#include <QSettings>

namespace Ui {
class AppSettings;
}

class AppSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AppSettings(QWidget *parent = nullptr);
    ~AppSettings();
    bool correctSettings;
    QString getSetting(QString setName);
    void setSettings(QString driver, QString server, QString dbName, QString port, QString userName, QString password);

private slots:
    void on_pushButton_clicked();
    void saveButton();


private:
    Ui::AppSettings *ui;
    QSettings *dbSettings;
};

#endif // APPSETTINGS_H
