#ifndef CONCEPTSETTINGS_H
#define CONCEPTSETTINGS_H

#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#define FIELDCONF "CFConf"


/* old CFConf format = {"LABELS": {"Headers": ["Имя реквизита", "Название метки"]},
 * "ConfigView": {}, "VISIBILITY": {"Headers": ["Имя реквизита", "Видимость"]}, "AccesibilityActions": ["Find", "Edit"]}
 *
 * new CFConf format =
 * [
 *       {
 *        "NAME":"VISIBILITY",
 *        "HEADERS": ["Имя реквизита", "Видимость"]
 *      },
 *      {
 *         "NAME":"LABELS",
 *        "HEADERS": ["Имя реквизита", "Название метки"]
 *      }
 * ]
 *
 * public:
 * QStringList visibilityHeaders;
 * load()
 * {
 *  this->visibilityHeaders = (!cfconfObj.value.isNull()) : cfconfObj.value["HEADERS"] : "{}";
 * }
 */



template < typename  T, typename U >

struct Settings {
    QString name;
    QJsonArray headers;
    QJsonArray sets;
    QString stype;
    T s1;
    U s2;
    QMap<QString, QMap<T, U>> set;
};

struct VisibilitySettings {
    QString name;
    QJsonArray headers;
    QJsonArray fields;
};

class ConceptSettings
{
public:
    ConceptSettings(QJsonDocument cfconfJson);
    QMap<QString, QMap<QString, QString> > settings;
    QMap<QString, QMap<QString, QJsonValue>> settingsArr;

    QStringList stypes = {"VISIBILITY", "LABELS", "ACCESSIBILITY"};
    QStringList sactions = {"NAME", "HEADERS", "FIELDS", "SETS"};

    QString Header;
    QStringList visibilityHeaders;
    QStringList labelsList;

    QString configSectionName;
    QStringList configArray;
    QString configObj;
    QString  name;
    QJsonDocument paramsJson;
    QJsonValue paramsValueJson;

    //QJsonValue visibility;
//    QJsonValue labels;


    QString save();
    void load();
    void update();
    void setValue(QString type, QString name, QString value);
    void setValue(QString type, QString name, QJsonArray value);
private:
    QString nameFieldConf = FIELDCONF;
    VisibilitySettings visibility;

};



#endif // CONCEPTSETTINGS_H
