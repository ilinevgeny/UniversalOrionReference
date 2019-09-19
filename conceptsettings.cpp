#include "conceptsettings.h"
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
ConceptSettings::ConceptSettings(QJsonDocument cfconfJson)
{
    this->paramsJson = cfconfJson;
}

void ConceptSettings::load()
{

    QStringList list, skeys;
    skeys = this->paramsJson.object().keys();

    //QVariantMap map = this->paramsJson.object().toVariantMap();
    //qDebug() << map["VISIBILITY"].toList();
    for(QString &type : skeys)
    {
        QJsonArray arr = this->paramsJson.object()[type].toArray();
        for(int i = 0; i < arr.size(); i++)
        {
            foreach(const QString& key, arr.at(i).toObject().keys()) {
                if(arr.at(i).toObject().value(key).isString())
                {
                    this->settings[type][key] = arr.at(i).toObject().value(key).toString();
                } else {
                    this->settingsArr[type][key] = arr.at(i).toObject().value(key);
                }
            }

        }
         // this->settings[type].s1 = this->paramsJson.object()[type].toArray().at(0).toObject().value("NAME").toString();
        //this->settings[type].s2 = this->paramsJson.object()[type].toArray().at(1).toObject().value("HEADERS").toArray();
    }
}

void ConceptSettings::update()
{

}
void ConceptSettings::setValue(QString type, QString name, QString value)
{
    this->settings[type][name] = value;
}

void ConceptSettings::setValue(QString type, QString name, QJsonArray value)
{
   this->settingsArr[type][name] = value;
}

QString ConceptSettings::save()
{
    QString query_str = "{";
     int countTypes = this->settingsArr.size();
    for (QString &type : this->settingsArr.keys())
    {
        query_str += "\"" + type +"\": [";
        int countAction = this->settingsArr[type].keys().count();
         for(QString &action : this->settingsArr[type].keys())
         {
              query_str += "{\"" + action + "\":[";
              int countArrAction = this->settingsArr[type][action].toArray().size();
              for (int j = 0; j < this->settingsArr[type][action].toArray().size(); j++) {
                   if(this->settingsArr[type][action][j].isString())
                   {
                       query_str += "\"" +this->settingsArr[type][action][j].toString() + "\"";
                   }
                   //FIELDS_TEXT SETTINGS
                   else if (this->settingsArr[type][action][j].isObject())
                   {
                       int countObj = this->settingsArr[type][action][j].toObject().size();
                       for(QString &key : this->settingsArr[type][action][j].toObject().keys())
                       {
                           if(this->settingsArr[type][action][j].toObject().value(key).isString())
                           {
                               query_str += "{\"" + key + "\":\"" + this->settingsArr[type][action][j].toObject().value(key).toString() + "\"}";
                           }
                           //FIELDS_GEO
                           else {
                               int geoCount = this->settingsArr[type][action][j].toObject().value(key).toArray().size();
                               query_str += "{\"" + key + "\":[";
                               for (int k = 0; k < this->settingsArr[type][action][j].toObject().value(key).toArray().size(); k++) {
                                   query_str += QString::number(this->settingsArr[type][action][j].toObject().value(key).toArray().at(k).toInt());
                                   if(geoCount > 1){
                                        query_str += ",";
                                        geoCount--;
                                   }
                               }
                                query_str += "]}";
                           }
                           if(countObj > 1){
                                query_str += ",";
                                countObj--;
                           }
                       }
                   }
                   //SETS SETTINGS
                   else  {
                       query_str += QString::number(this->settingsArr[type][action][j].toInt());
                   }
                   if(countArrAction > 1){
                      query_str  += ",";
                      countArrAction--;
                   }
              }
              query_str += "]}";
               if(countAction > 1) {
                   query_str += ",";
                   countAction--;
               }
         }
         query_str += "]";
         if(countTypes-- > 1) query_str += ",";
    }
    query_str += "}";
//    qDebug() << "Строка апдейта " <<  query_str;
    return query_str;
}
