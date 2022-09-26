#include "mesprocess.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "QsLog.h"
#include "gdatafactory.h"
#include "imessagebox.h"
#include <QEventLoop>
MesProcess::MesProcess(QObject *parent) : QObject(parent)
{
    accessManager = new QNetworkAccessManager(this);
    connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(slot_urlReply(QNetworkReply*)));
}

void MesProcess::exec_http_post(int desSign)//0:Mes 1:AOI 2:register 3:visuallead
{
    QString urlDes;
    if(desSign == 0)//mes
    {
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
        urlDes.append("/post");
    }
    else if(desSign == 1)//AOI
    {
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("AOI_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("AOI_PORT"));
        urlDes.append("/post");
    }
    else if(desSign == 2)//register
    {
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("REGISTER_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("REGISTER_PORT"));
        urlDes.append("/post");
    }
    else if(desSign == 3)//visuallead
    {
        urlDes = "http://";
        urlDes.append(GDataFactory::get_factory()->get_config_para("VISUAL_LEAD_IP"));
        urlDes.append(":");
        urlDes.append(GDataFactory::get_factory()->get_config_para("VISUAL_LEAD_PORT"));
        urlDes.append("/post");
    }
    else
        urlDes = "http://127.0.0.1:8080/post";

    QNetworkRequest request;
    request.setUrl(QUrl(urlDes));
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    QJsonObject  root, data_value,ats_models;
    QJsonArray   ats_value;

    ats_models.insert("itemId","");
    ats_models.insert("itemName","C1");
    ats_models.insert("itemL_Limit","");
    ats_models.insert("itemH_Limit","");
    ats_models.insert("itemTsValue","");
    ats_models.insert("itemLength","");
    ats_models.insert("itemRes","OK");
    ats_models.insert("errCodes","SX;LB");

    ats_value.append(ats_models);
    root.insert("atsTsItemsModels", ats_value);

    root.insert("iType", "1");
    root.insert("pcbCode", "56CTA69");
    root.insert("workStationSn", "WS101A01");
    root.insert("tsRes","OK");
    root.insert("tsLength", "");
    root.insert("pcName", "sys001");
    root.insert("pcIp", "127.0.0.1");
    root.insert("devName",u8"德律AOI");
    root.insert("devModel", "DL2000");
    root.insert("devNo", "ZD_AOI001");
    root.insert("toolNo", "");
    root.insert("toolBitSn", "");
    root.insert("logData", "");

    data_value.insert("imeiInfo","11");
    data_value.insert("meidInfo","22");
    data_value.insert("macInfo","22");
    root.insert("data",data_value);

    QJsonDocument doc_data(root);
    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    accessManager->post(request, request_data);
}

void MesProcess::exec_http_get(QString productsn)
{
    QString tmpurl;
    tmpurl = "http://";
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_IP"));
    tmpurl.append(":");
    tmpurl.append(GDataFactory::get_factory()->get_config_para("MES_PORT"));
    tmpurl.append("/ProductionForCenterControl/GetProduction?sn=");
    tmpurl.append(productsn);

    QNetworkReply* reply;
    QEventLoop eventloop;
    QNetworkRequest* request = new QNetworkRequest();
    request->setUrl(QUrl(tmpurl));
    request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    request->setRawHeader("appId","CenterController");
    request->setRawHeader("publicKey",PUBLIC_KEY);
    reply = accessManager->get(*request);
    QLOG_INFO()<<"exec serial port http get request";
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    if(reply->isFinished())
    {
        QLOG_INFO()<<"serial port http get request reply is NORMAL!";
    }
    else
    {
        QLOG_WARN()<<"serial port http get request reply is TIMEOUT!";;
    }
}

void MesProcess::slot_urlReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray responseByte = reply->readAll();
        if(GDataFactory::get_test_plan_wgt()->isVisible())
            QLOG_WARN()<<"rev change production mes reply,the info is:" << QString(responseByte);
        else {
            QLOG_WARN()<<"rev history display mes reply,the info is:" << QString(responseByte);
        }
        QJsonDocument jsonDocument = QJsonDocument::fromJson(responseByte);

        QJsonObject jsonObject = jsonDocument.object();
        QString statusString = "";
        if(jsonObject.contains("status"))
        {
            QJsonValue instructionValue = jsonObject.value(QStringLiteral("status"));
            if(instructionValue.isString())
                statusString = instructionValue.toString();
            else
            {
                QLOG_WARN()<<"MES status para data format is ABNORMAL!";
                IMessageBox* msgBox = new IMessageBox(3);
                if(GDataFactory::get_test_plan_wgt()->isVisible())
                    msgBox->warning(u8"MES返回换产信息数据格式错误,换产失败!");
                else {
                    msgBox->warning(u8"MES返回换产信息数据格式错误,历史浏览无法进行!");
                }
                return;
            }
        }

        if(statusString == "success")
        {
            if(jsonObject.contains("dataInfo"))
            {
                QJsonValue arrayValue = jsonObject.value(QStringLiteral("dataInfo"));
                if(arrayValue.isArray())
                {
                    QJsonArray array = arrayValue.toArray();
                    for (int i=0;i<array.size();i++)
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();
                        QList<QString> tmpInfo;
                        tmpInfo.push_back(icon["projecT_ID"].toString());
                        tmpInfo.push_back(icon["modeL_CODE"].toString());//half material
                        tmpInfo.push_back(icon["mP_MODEL_CODE"].toString());
                        tmpInfo.push_back(icon["tpS_TEMPLATE"].toString());//board type
                        tmpInfo.push_back(icon["boarD_SIZE"].toString());
                        //when rev the board info from serial port,put the info into the tmp list
                        //in the data factory,when we press the save button on the UI,we save
                        //the formal serial port info to the formal list
                        if(GDataFactory::get_test_plan_wgt()->isVisible())
                            GDataFactory::get_factory()->set_AB_change_product_info(tmpInfo);
                        if(GDataFactory::get_history_display_wgt()->isVisible())
                            GDataFactory::get_history_display_wgt()->set_boardtype_halfmaterialnumber(icon["tpS_TEMPLATE"].toString(),icon["modeL_CODE"].toString());
                    }
                }
            }
        }
        else {
            QLOG_WARN()<<"MES status para is error!";
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"MES返回error,换产失败!");
            return;
        }
//    reply->deleteLater();
    }
}
