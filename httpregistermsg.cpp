#include "httpregistermsg.h"
#include "gdatafactory.h"
#include "QsLog.h"
#include "QJsonDocument"
HttpRegisterMsg::HttpRegisterMsg()
{

}

void HttpRegisterMsg::process_http_msg(QJsonObject jsonObject,HttpResponse& response)
{
    QJsonValue lineName,macAddress,machineStyle;
    if(jsonObject.contains("lineName") && (jsonObject.value("lineName").toString().trimmed() != ""))
    {
        lineName = jsonObject.value("lineName");
    }
    else
    {
        QLOG_WARN()<<"register lineName is null!";
        QJsonObject reJson;
        reJson.insert("code","ERROR");
        reJson.insert("machineName",u8"");

        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
        return;
    }

    if(jsonObject.contains("macAddress") && (jsonObject.value("macAddress").toString().trimmed() != ""))
    {
        macAddress = jsonObject.value("macAddress");
    }
    else
    {
        QLOG_WARN()<<"register macAddress is null!";
        QJsonObject reJson;
        reJson.insert("code","ERROR");
        reJson.insert("machineName",u8"");

        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
        return;
    }

    if(jsonObject.contains("machineStyle") && (jsonObject.value("machineStyle").toString().trimmed() != ""))
    {
        machineStyle = jsonObject.value("machineStyle");
    }
    else
    {
        QLOG_WARN()<<"register machineStyle is null!";
        QJsonObject reJson;
        reJson.insert("code","ERROR");
        reJson.insert("machineName",u8"");

        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
        return;
    }

    QList<QString> lstpara;
    lstpara.push_back(lineName.toString());
    lstpara.push_back(macAddress.toString());
    lstpara.push_back(machineStyle.toString());

    QString tmpMachineName;
    if(GDataFactory::get_factory()->check_mac_exist(macAddress.toString(),&tmpMachineName))
    {
        QLOG_WARN()<<"current mac address has already been registered!";
        QJsonObject reJson;
        reJson.insert("code","OK");
        reJson.insert("machineName",tmpMachineName);

        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
    else
    {
        QString maName = GDataFactory::get_factory()->register_new_mac_machine(lstpara);
        if(maName == "")
        {
            QLOG_WARN()<<"register new mac for machine failed!";
            QJsonObject reJson;
            reJson.insert("code","ERROR");
            reJson.insert("machineName",u8"");

            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
        else
        {
            QLOG_INFO()<<"register new mac for machine success!";
            QJsonObject reJson;
            reJson.insert("code","OK");
            reJson.insert("machineName",maName);

            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
    }
}
