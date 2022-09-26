#include "httptconmsg.h"
#include "gdatafactory.h"
#include "QsLog.h"
#include <QJsonDocument>
#include <QJsonArray>
HttpTconMsg::HttpTconMsg()
{

}

void HttpTconMsg::process_http_msg(QJsonObject httpJsonObj,HttpResponse& response)
{
    QJsonValue instructionValue = httpJsonObj.value(QStringLiteral("reType"));
    if(instructionValue.toString() == "TCON_TEST")
    {
//        QLOG_WARN()<<"rev TCON request MSG!";
        QList<QList<QString>> lstAll = GDataFactory::get_factory()->get_all_machine_change_info();
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
            root.insert("requestType","TCON_TEST");
            QJsonArray  ats_value;
            for (int i=0;i<lstAll.size();i++)
            {
                QJsonObject ats_models;
                QList<QString> lstChangeInfo = lstAll[i];
                //**just for test ,delete later
    //                if(lstChangeInfo[6] != "T101")
    //                    continue;
               //**just for test ,delete later
                ats_models.insert("areaName",lstChangeInfo[6]);//T104
                ats_models.insert("machineName",lstChangeInfo[3]);//»úÌ¨1
                //10545TCON-ZJ is the original board style,but the test station needed is 10545
                // we need to cut the string
                QString tmpBoardStyle="";
                for (int i=0;i<5;i++)
                {
                    tmpBoardStyle.append(lstChangeInfo[0][i]);
                }
                ats_models.insert("boardStyle",tmpBoardStyle);
                //the same as board style,the machine style name also needs to be cut
                QString tmpMachineStyleName = "";
                for (int i=0;i<2;i++)
                {
                    tmpMachineStyleName.append(lstChangeInfo[1][i]);
                }
                ats_models.insert("machineStyleName","");//65E52G(DS)(BOM4)
                ats_models.insert("halfMaterialNumber",lstChangeInfo[2]);//286050
                ats_value.append(ats_models);
            }
            root.insert("data",ats_value);
            QJsonDocument doc_data(root);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
            QLOG_INFO()<<response_data;
        }
    }
    else if(instructionValue.toString() == "TCON_RESULT")
    {
        QLOG_WARN()<<"rev TCON change production result!";
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
            QLOG_WARN()<<"tcon change production result exist ERROR!";
            QJsonObject reJson;
            reJson.insert("code","ERROR");
            reJson.insert("data",u8"");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
        }
    }
    else if(instructionValue.toString() == "TCON_NULL")
    {
        QLOG_WARN()<<"rev TCON change production NULL result!";
        QString machineName = httpJsonObj.value(QStringLiteral("machineName")).toString();
        QString lineName = httpJsonObj.value(QStringLiteral("lineName")).toString();
        if((machineName.trimmed() != "") && (lineName.trimmed() != ""))
        {
            QLOG_INFO()<<"del "<<machineName<<"from the change production list";
            GDataFactory::get_factory()->del_change_production_machine_name(machineName);
            QJsonObject reJson;
            reJson.insert("code","OK");
            reJson.insert("data",u8"");
            QJsonDocument doc_data(reJson);
            QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
            response.write(response_data,true);
            QLOG_INFO()<<"send change production fail info";
            connect(this,SIGNAL(signal_change_production_failed(QString,QString)),GDataFactory::get_factory(),SLOT(slot_change_production_failed(QString,QString)));
            emit signal_change_production_failed(machineName,lineName);
            disconnect(this,SIGNAL(signal_change_production_failed(QString,QString)),GDataFactory::get_factory(),SLOT(slot_change_production_failed(QString,QString)));
        }
        else {
            QLOG_WARN()<<"tcon change production result exist ERROR!";
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
        QLOG_WARN()<<"tcon request msg data exist ERROR!";
        QJsonObject reJson;
        reJson.insert("code","ERROR");
        reJson.insert("data",u8"");
        QJsonDocument doc_data(reJson);
        QByteArray response_data = doc_data.toJson(QJsonDocument::Compact);
        response.write(response_data,true);
    }
}
