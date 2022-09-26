#include "httptvmsg.h"
#include "gdatafactory.h"
#include "QsLog.h"
#include <QJsonDocument>
#include <QJsonArray>
HttpTVMsg::HttpTVMsg()
{

}

void HttpTVMsg::process_http_msg(QJsonObject httpJsonObj, HttpResponse &response)
{
    QJsonValue instructionValue = httpJsonObj.value(QStringLiteral("reType"));
    if(instructionValue.toString() == "TV_TEST")
    {
        QLOG_INFO()<<"rev TV test request MSG!";
//        QList<QList<QString>> lstAll = GDataFactory::get_factory()->get_all_machine_change_info();

        QList<QList<QString>> lstAll;
        QString tmpAreaName = "T104";
        QString tmpMachineName = u8"机台1";
        QString tmpBoardStyle = "8796";
        QString tmpMachineStyleName = "J55F(DS)(BOM1)";
        QString tmpBomComponentName = "270797";
        QList<QString> tmpLst;
        tmpLst.push_back(tmpAreaName);
        tmpLst.push_back(tmpMachineName);
        tmpLst.push_back(tmpBoardStyle);
        tmpLst.push_back(tmpMachineStyleName);
        tmpLst.push_back(tmpBomComponentName);
        lstAll.push_back(tmpLst);
        if(lstAll.count() == 0)
        {
            QLOG_WARN()<<"there is no change info in the DB!";
            QJsonObject reJson;
            reJson.insert("code","ERROR");
            reJson.insert("data",u8"");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
        else
        {
            QJsonObject root;
            root.insert("code","OK");
            root.insert("requestType","TV_TEST");
            QJsonArray  ats_value;
            for (int i=0;i<lstAll.size();i++)
            {
                QJsonObject ats_models;
                QList<QString> lstChangeInfo = lstAll[i];
                ats_models.insert("areaName",lstChangeInfo[0]);//T104
                ats_models.insert("machineName",lstChangeInfo[1]);//机台1
                ats_models.insert("boardStyle",lstChangeInfo[2]);
                ats_models.insert("machineStyleName",lstChangeInfo[3]);//65E52G(DS)(BOM4)
                ats_models.insert("bomComponentName",lstChangeInfo[4]);//286050
                ats_value.append(ats_models);
            }
            root.insert("data",ats_value);
            QJsonDocument doc_data(root);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
            qDebug()<<response_data;
        }
    }
    else if(instructionValue.toString() == "TV_RESULT")
    {
        QLOG_WARN()<<"rev TV change production result!";
        QString machineName = httpJsonObj.value(QStringLiteral("machineName")).toString();
        if(machineName.trimmed() != "")
        {
            QLOG_INFO()<<"del "<<machineName<<"from the change production list";
            GDataFactory::get_factory()->del_change_production_machine_name(machineName);
            QJsonObject reJson;
            reJson.insert("code","OK");
            reJson.insert("data",u8"");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
        else {
            QLOG_WARN()<<"tv change production result exist ERROR!";
            QJsonObject reJson;
            reJson.insert("code","ERROR");
            reJson.insert("data",u8"");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
    }
    else
    {
        QLOG_WARN()<<"tv request msg data exist ERROR!";
        QJsonObject reJson;
        reJson.insert("code","ERROR");
        reJson.insert("data",u8"");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
}
