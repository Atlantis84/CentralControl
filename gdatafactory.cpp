#include "gdatafactory.h"
#include "QsLog.h"
#include <QHostInfo>
#include <QApplication>
#include <QDir>
#include "imessagebox.h"
#include <QSqlQuery>
#include <QtMath>
GDataFactory* GDataFactory::m_pGDataFactory=nullptr;
MainWindow* GDataFactory::m_pMainWindow = nullptr;
WelcomeWidget* GDataFactory::m_pWelcomeWgt = nullptr;
DMDBAccessInterface* GDataFactory::m_pDMDBAccess = nullptr;
CentralWidget* GDataFactory::m_pCentralWgt=nullptr;
LeftWidget* GDataFactory::m_pLeftWgt =nullptr;
RightHeadWidget* GDataFactory::m_pRightHeadWgt=nullptr;
RightMainWindow* GDataFactory::m_pRightMainWindow = nullptr;
RightMainWidget* GDataFactory::m_pRightMainWgt=nullptr;
RightQueryWidget* GDataFactory::m_pRightQueryWgt=nullptr;
TestWidget* GDataFactory::m_pTestWgt=nullptr;
MesProcess* GDataFactory::m_pMesProcess = nullptr;
TestPlanWgt* GDataFactory::m_pTestPlanWgt = nullptr;
BoardStyleWidget* GDataFactory::m_pBoardStyleWgtA = nullptr;
BoardStyleWidget* GDataFactory::m_pBoardStyleWgtB = nullptr;
QsLogWidget* GDataFactory::m_pQsLogWgt = nullptr;
TestPlanThread* GDataFactory::m_pTestPlanThread = nullptr;
TestDBThread* GDataFactory::m_pTestDBThread = nullptr;
HistoryDisplayWgt* GDataFactory::m_pHistoryDisplayWgt = nullptr;
ElecEquipTestPlanWgt* GDataFactory::m_pElecEquipWgt = nullptr;
StationTabWgt* GDataFactory::m_pStationTabWgt = nullptr;
UserTabWgt* GDataFactory::m_pUserTabWgt = nullptr;
StationMonitorWgt* GDataFactory::m_pStationMonitorWgt = nullptr;
TvPlanWgt* GDataFactory::m_pTvPlanWgt = nullptr;
PlcWakeUpDlg* GDataFactory::m_pPlcWakeUpDlg = nullptr;
QNetworkAccessManager* GDataFactory::m_pAccessManagerVision = nullptr;
QNetworkAccessManager* GDataFactory::m_pAccessManagerAOI = nullptr;
QNetworkAccessManager* GDataFactory::m_pAccessManagerPLC = nullptr;
GDataFactory::GDataFactory()
{
}

GDataFactory::~GDataFactory()
{
    m_pConfigFile.close();
    m_pConfigFile.flush();
}

void GDataFactory::read_serial_number()
{
    pComm = new DataCommSerial();
    connect(pComm,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number(const QByteArray&)));
    if(pComm)
    {
        QString portName = ConfigInfo["COM_PORT"];
        QString portRate = ConfigInfo["COM_BARD_RATE"];
        pComm->init(tagSerialPortInfo(portName,portRate.toInt()));
        QList<QByteArray> datas;
        QByteArray aa;
        aa.append(static_cast<char>(0x04));
        aa.append(static_cast<char>(0xE4));
        aa.append(static_cast<char>(0x04));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0xFF));
        aa.append(static_cast<char>(0x14));
        datas.append(aa);
        IResponseValidate* pF1 = new IResponseValidate_F1();
//        pComm->sendRequest_and_checkResponse(datas, 5000, pF1);
//        pComm->sendRequest_and_recvResponse(datas,500);

        pComm->recvResponse_only();
//        pComm->sendRequest_only(datas,500);
        pComm->start();
    }
}

void GDataFactory::stop_serial_port()
{
    pComm->terminate();
    pComm->quit();
    pComm->closeSerialPort();
    if(pComm)
        delete pComm;
}

void GDataFactory::connect_common_signal_slot()
{
    connect(get_left_wgt(),SIGNAL(signal_send_path(const QString)),get_right_query_wgt(),SLOT(slot_rev_path(const QString)));
    connect(get_right_query_wgt(),SIGNAL(signal_add_sub_window(const QString,const QString)),get_test_wgt(),
            SLOT(slot_rev_query_result(const QString,const QString)));
    connect(get_test_plan_wgt(),SIGNAL(signal_change_btn_icon()),get_test_wgt(),SLOT(slot_change_btn_icon()));
    connect(get_test_plan_wgt(),SIGNAL(signal_save_plan()),get_test_wgt(),SLOT(slot_save_plan()));
    connect(get_test_wgt(),SIGNAL(signal_kill_timeout_timer()),get_test_plan_wgt(),SLOT(slot_kill_timeout_timer()));
    connect(this,SIGNAL(signal_change_production_already_complete()),get_test_wgt(),SLOT(slot_change_production_already_complete()));
    connect(this,SIGNAL(signal_register_new_station_success()),get_test_plan_wgt(),SLOT(slot_refresh_treewgt()));
    connect(get_elec_equip_test_plan_wgt(),SIGNAL(signal_change_btn_icon()),get_test_wgt(),SLOT(slot_change_btn_icon()));

    m_pCurrentLineInfoValue = TCON_LINE;

}

void GDataFactory::load_json_config_1(char *filename)
{
    m_pConfigFile.setFileName(QString(filename));
    m_pConfigFileName=QString(filename);
    QString JsonValue;
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QLOG_INFO()<<"load json config file success!";
        JsonValue = m_pConfigFile.readAll();
        m_pConfigFile.close();

        QJsonParseError parseJsonErr;
        m_pJsonDocument = QJsonDocument::fromJson(JsonValue.toUtf8(),&parseJsonErr);
        if(!(parseJsonErr.error == QJsonParseError::NoError))
        {
            QLOG_INFO()<<"parse json file failed!";
            return;
        }
        else {
            QLOG_INFO()<<"parse json file success!";
        }

        m_pRootObj = m_pJsonDocument.object();
        if(m_pRootObj.contains("IPConfigs"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("IPConfigs"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue iconArray = array.at(i);
                    QJsonObject icon = iconArray.toObject();
                    ConfigInfo.insert("AOI_IP",icon["AOI_IP"].toString());
                    ConfigInfo.insert("PLC_IP",icon["PLC_IP"].toString());
                    ConfigInfo.insert("REGISTER_IP",icon["REGISTER_IP"].toString());
                    ConfigInfo.insert("VISUAL_LEAD_IP",icon["VISUAL_LEAD_IP"].toString());
                    ConfigInfo.insert("MES_IP",icon["MES_IP"].toString());
                    ConfigInfo.insert("CENTRAL_CONTROL_IP",icon["CENTRAL_CONTROL_IP"].toString());
                }
            }
        }

        if(m_pRootObj.contains("SN_A"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("SN_A"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue iconArray = array.at(i);
                    QJsonObject icon = iconArray.toObject();
                    for (int var = 0; var < 12; ++var) {
                        QString tmpName = QString("A_SN%1").arg(var+1);
                        m_pManualSN_A.insert(var,icon[tmpName].toString());
                    }

                    ConfigInfo.insert("A_SN1",icon["A_SN1"].toString());
                    ConfigInfo.insert("A_SN2",icon["A_SN2"].toString());
                    ConfigInfo.insert("A_SN3",icon["A_SN3"].toString());
                    ConfigInfo.insert("A_SN4",icon["A_SN4"].toString());
                    ConfigInfo.insert("A_SN5",icon["A_SN5"].toString());
                    ConfigInfo.insert("A_SN6",icon["A_SN6"].toString());
                    ConfigInfo.insert("A_SN7",icon["A_SN7"].toString());
                    ConfigInfo.insert("A_SN8",icon["A_SN8"].toString());
                    ConfigInfo.insert("A_SN9",icon["A_SN9"].toString());
                    ConfigInfo.insert("A_SN10",icon["A_SN10"].toString());
                    ConfigInfo.insert("A_SN11",icon["A_SN11"].toString());
                    ConfigInfo.insert("A_SN12",icon["A_SN12"].toString());
                }
            }
        }

        if(m_pRootObj.contains("SN_B"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("SN_B"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue iconArray = array.at(i);
                    QJsonObject icon = iconArray.toObject();
                    for (int var = 0; var < 10; ++var) {
                        QString tmpName = QString("B_SN%1").arg(var+1);
                        m_pManualSN_B.insert(var,icon[tmpName].toString());
                    }
                    ConfigInfo.insert("B_SN1",icon["B_SN1"].toString());
                    ConfigInfo.insert("B_SN2",icon["B_SN2"].toString());
                    ConfigInfo.insert("B_SN3",icon["B_SN3"].toString());
                    ConfigInfo.insert("B_SN4",icon["B_SN4"].toString());
                    ConfigInfo.insert("B_SN5",icon["B_SN5"].toString());
                    ConfigInfo.insert("B_SN6",icon["B_SN6"].toString());
                    ConfigInfo.insert("B_SN7",icon["B_SN7"].toString());
                    ConfigInfo.insert("B_SN8",icon["B_SN8"].toString());
                    ConfigInfo.insert("B_SN9",icon["B_SN9"].toString());
                    ConfigInfo.insert("B_SN10",icon["B_SN10"].toString());
                    ConfigInfo.insert("B_SN11",icon["B_SN11"].toString());
                    ConfigInfo.insert("B_SN12",icon["B_SN12"].toString());
                }
            }
        }

        if(m_pRootObj.contains("AlterProduction"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("AlterProduction"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    if(i==0)
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();

                        //load the pre board info
                        QString qStr[5];
                        qStr[0] = icon["PROJECT_ID_A"].toString();
                        qStr[1] = icon["HALF_MATERIAL_NUMBER_A"].toString();
                        qStr[2] = icon["MACHINE_STYLE_NAME_A"].toString();
                        qStr[3] = icon["BOARD_A_STYLE"].toString();
                        qStr[4] = icon["BOARD_SIZE_A"].toString();
                        for (int m=0;m<5;m++)
                            ListBoardInfoA.push_back(qStr[m]);

                        ConfigInfo.insert("BOARD_A_SN",icon["BOARD_A_SN"].toString());
                        ConfigInfo.insert("BOARD_A_STYLE",icon["BOARD_A_STYLE"].toString());
                        ConfigInfo.insert("MACHINE_STYLE_NAME_A",icon["MACHINE_STYLE_NAME_A"].toString());
                        ConfigInfo.insert("HALF_MATERIAL_NUMBER_A",icon["HALF_MATERIAL_NUMBER_A"].toString());
                        ConfigInfo.insert("PROJECT_ID_A",icon["PROJECT_ID_A"].toString());
                        ConfigInfo.insert("BOARD_SIZE_A",icon["BOARD_SIZE_A"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A1",icon["MACHINE_STATION_A1"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A2",icon["MACHINE_STATION_A2"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A3",icon["MACHINE_STATION_A3"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A4",icon["MACHINE_STATION_A4"].toString());
                        ConfigInfo.insert("MACHINE_STATION_A5",icon["MACHINE_STATION_A5"].toString());
                    }
                    else
                    {
                        QJsonValue iconArray = array.at(i);
                        QJsonObject icon = iconArray.toObject();

                        //load the pre board info
                        QString qStr[5];
                        qStr[0] = icon["PROJECT_ID_B"].toString();
                        qStr[1] = icon["HALF_MATERIAL_NUMBER_B"].toString();
                        qStr[2] = icon["MACHINE_STYLE_NAME_B"].toString();
                        qStr[3] = icon["BOARD_B_STYLE"].toString();
                        qStr[4] = icon["BOARD_SIZE_B"].toString();
                        for (int m=0;m<5;m++)
                            ListBoardInfoB.push_back(qStr[m]);

                        ConfigInfo.insert("BOARD_B_SN",icon["BOARD_B_SN"].toString());
                        ConfigInfo.insert("BOARD_B_STYLE",icon["BOARD_B_STYLE"].toString());
                        ConfigInfo.insert("MACHINE_STYLE_NAME_B",icon["MACHINE_STYLE_NAME_B"].toString());
                        ConfigInfo.insert("HALF_MATERIAL_NUMBER_B",icon["HALF_MATERIAL_NUMBER_B"].toString());
                        ConfigInfo.insert("PROJECT_ID_B",icon["PROJECT_ID_B"].toString());
                        ConfigInfo.insert("BOARD_SIZE_B",icon["BOARD_SIZE_B"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B1",icon["MACHINE_STATION_B1"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B2",icon["MACHINE_STATION_B2"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B3",icon["MACHINE_STATION_B3"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B4",icon["MACHINE_STATION_B4"].toString());
                        ConfigInfo.insert("MACHINE_STATION_B5",icon["MACHINE_STATION_B5"].toString());
                    }
                }
            }
        }

        if(m_pRootObj.contains("PortConfigs"))
        {
            QJsonValue portValue = m_pRootObj.value(QStringLiteral("PortConfigs"));
            if(portValue.isObject())
            {
                QJsonObject obj = portValue.toObject();
                if(obj.contains("AOI_PORT"))
                    ConfigInfo.insert("AOI_PORT",obj.value("AOI_PORT").toString());
                if(obj.contains("PLC_PORT"))
                    ConfigInfo.insert("PLC_PORT",obj.value("PLC_PORT").toString());
                if(obj.contains("REGISTER_PORT"))
                    ConfigInfo.insert("REGISTER_PORT",obj.value("REGISTER_PORT").toString());
                if(obj.contains("VISUAL_LEAD_PORT"))
                    ConfigInfo.insert("VISUAL_LEAD_PORT",obj.value("VISUAL_LEAD_PORT").toString());
                if(obj.contains("MES_PORT"))
                    ConfigInfo.insert("MES_PORT",obj.value("MES_PORT").toString());
                if(obj.contains("CENTRAL_CONTROL_PORT"))
                    ConfigInfo.insert("CENTRAL_CONTROL_PORT",obj.value("CENTRAL_CONTROL_PORT").toString());
                if(obj.contains("COM_PORT"))
                    ConfigInfo.insert("COM_PORT",obj.value("COM_PORT").toString());
                if(obj.contains("COM_BARD_RATE"))
                    ConfigInfo.insert("COM_BARD_RATE",obj.value("COM_BARD_RATE").toString());
            }
        }

        if(m_pRootObj.contains("AutoLogin"))
        {
            QJsonValue autologinValue = m_pRootObj.value(QStringLiteral("AutoLogin"));
            if(autologinValue.isObject())
            {
                QJsonObject obj = autologinValue.toObject();
                if(obj.contains("AUTO_LOGIN_SIGN"))
                    ConfigInfo.insert("AUTO_LOGIN_SIGN",obj.value("AUTO_LOGIN_SIGN").toString());
                if(obj.contains("USER_NAME"))
                    ConfigInfo.insert("USER_NAME",obj.value("USER_NAME").toString());
                if(obj.contains("PASSWORD"))
                    ConfigInfo.insert("PASSWORD",obj.value("PASSWORD").toString());
                if(obj.contains("PLC_WAKE_UP"))
                    ConfigInfo.insert("PLC_WAKE_UP",obj.value("PLC_WAKE_UP").toString());
                if(obj.contains("SCANNER_TIMER"))
                    ConfigInfo.insert("SCANNER_TIMER",obj.value("SCANNER_TIMER").toString());
                if(obj.contains("LOG_LEVEL"))
                    ConfigInfo.insert("LOG_LEVEL",obj.value("LOG_LEVEL").toString());
            }
        }

        if(m_pRootObj.contains("ConfigInstructions"))
        {
            QJsonValue instructionValue = m_pRootObj.value(QStringLiteral("ConfigInstructions"));
            if(instructionValue.isString())
                qDebug()<<instructionValue.toString();
        }
    }
    else
    {
        QLOG_INFO()<<"load json config file failed!";
        return;
    }
    m_pConfigFile.flush();
}

QString GDataFactory::get_config_para(const QString keyname)
{
    return ConfigInfo[keyname];
}

void GDataFactory::set_config_para_1(const QString keyname, const QString keyvalue)
{
    ConfigInfo[keyname] = keyvalue;//update the config file,at the same time you have to update the info that has been loaded

    QJsonObject subObj;
    QJsonArray subArray;

    if(keyname == "AUTO_LOGIN_SIGN" || keyname == "USER_NAME" || keyname == "PASSWORD")
    {
        if(m_pRootObj.contains("AutoLogin") && m_pRootObj["AutoLogin"].isObject())
        {
            QJsonValueRef refValue = m_pRootObj.find("AutoLogin").value();
            QJsonObject obj = refValue.toObject();
            obj[keyname] = keyvalue;
            refValue = obj;
        }
    }

    if((keyname == "BOARD_A_SN")
            || (keyname == "MACHINE_STATION_A1")|| (keyname == "MACHINE_STATION_A2")
            || (keyname == "MACHINE_STATION_A3")|| (keyname == "MACHINE_STATION_A4")
            || (keyname == "MACHINE_STATION_A5")|| (keyname == "BOARD_A_STYLE")
            || (keyname == "MACHINE_STYLE_NAME_A")|| (keyname == "HALF_MATERIAL_NUMBER_A")
            || (keyname == "BOARD_SIZE_A")|| (keyname == "PROJECT_ID_A"))
    {
        QJsonValueRef refValue = m_pRootObj.find("AlterProduction").value();
        QJsonArray refArray = refValue.toArray();

        QJsonArray::Iterator arrayIterator = refArray.begin();
        QJsonValueRef elementOneValueRef = arrayIterator[0];
        QJsonObject elementOneObject = elementOneValueRef.toObject();
        elementOneObject.insert(keyname,keyvalue);
        elementOneValueRef = elementOneObject;
        refValue = refArray;
    }

    if((keyname == "BOARD_B_SN")
            || (keyname == "MACHINE_STATION_B1")|| (keyname == "MACHINE_STATION_B2")
            || (keyname == "MACHINE_STATION_B3")|| (keyname == "MACHINE_STATION_B4")
            || (keyname == "MACHINE_STATION_B5")|| (keyname == "BOARD_B_STYLE")
            || (keyname == "MACHINE_STYLE_NAME_B")|| (keyname == "HALF_MATERIAL_NUMBER_B")
            || (keyname == "BOARD_SIZE_B")|| (keyname == "PROJECT_ID_B"))
    {
        QJsonValueRef refValue = m_pRootObj.find("AlterProduction").value();
        QJsonArray refArray = refValue.toArray();

        QJsonArray::Iterator arrayIterator = refArray.begin();
        QJsonValueRef elementOneValueRef = arrayIterator[1];
        QJsonObject elementOneObject = elementOneValueRef.toObject();
        elementOneObject.insert(keyname,keyvalue);
        elementOneValueRef = elementOneObject;
        refValue = refArray;
    }
    m_pJsonDocument.setObject(m_pRootObj);
}

void GDataFactory::save_config_file()
{
    m_pConfigFile.setFileName(m_pConfigFileName);
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        m_pConfigFile.resize(0);//this sentence is essential,or the file will be abnormal---20211123lilei
        m_pJsonDocument.setObject(m_pRootObj);
        m_pConfigFile.seek(0);
        m_pConfigFile.write(m_pJsonDocument.toJson());
        m_pConfigFile.close();
        m_pConfigFile.flush();
    }
}

void GDataFactory::del_change_production_machine_name(QString machinename)
{
    int i=0;
    bool hvFound = false;
    for (i=0;i<m_pMachineNamesOfChangeProduction.length();i++)
    {
        if(m_pMachineNamesOfChangeProduction[i] == machinename)
        {
            del_one_change_product_info(machinename);
            QLOG_INFO()<<machinename<<" change production SUCCESS!";
            hvFound = true;
        }
        else
            continue;
    }

    if(hvFound)
    {
//        m_pMachineNamesOfChangeProduction.removeAt(i);
        m_pMachineNamesOfChangeProduction.removeOne(machinename);
        if(m_pMachineNamesOfChangeProduction.length() == 0)
        {
//            truncate_change_info_table();
            emit signal_change_production_already_complete();
        }
    }
}

QMap<QString, QString> GDataFactory::get_PLC_or_AOI_change_info(QString keyvalue)
{
//    QMutexLocker lock(&m_mutex_PLC_AOI);
    QMap<QString,QString> tmpMap;
    if(mapOneButtonToVisionOrAOI.find(keyvalue) != mapOneButtonToVisionOrAOI.end())
        return mapOneButtonToVisionOrAOI[keyvalue];
    else {
        return tmpMap;
    }
}

void GDataFactory::add_change_production_info_for_PLC_or_AOI(QString machinename,QString boardtype,QString halfmaterialnumber)
{
//    QMutexLocker lock(&m_mutex_PLC_AOI);
    if(mapOneButtonToVisionOrAOI.find(machinename) != mapOneButtonToVisionOrAOI.end())
        ;
    else {
        QMap<QString,QString> tmpMap;
        QLOG_INFO()<<machinename<<" change production board_type:"<<boardtype<<",half_material_number:"<<halfmaterialnumber;
        tmpMap.insert(boardtype,halfmaterialnumber);
        mapOneButtonToVisionOrAOI.insert(machinename,tmpMap);
    }
}

QString GDataFactory::bytes_to_str(QByteArray data)
{
    QString str_data;
    str_data.clear();
    quint8 n_data = 0;
    for(int i=0; i<data.size(); i++)
    {
        n_data = static_cast<quint8>(data[i]) ;
        if(n_data < 16) str_data.append('0');
        str_data.append(QString::number(n_data, 16));
        str_data.append(' ');
    }
    return str_data;
}

quint8 GDataFactory::get_last_ip(const int id)
{
    if (id == 0)
    {
        QString localHostName = QHostInfo::localHostName();
        QString localIP = "";
//        qDebug()<<"localHostName:"<<localHostName;
        QHostInfo info = QHostInfo::fromName(localHostName);
        foreach(QHostAddress addr,info.addresses())
            if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            {
//                qDebug()<<"\033[31mIPV4 Address:"<<addr.toString();
                localIP = addr.toString();
            }
        return process_ip_string(localIP);
    }
    else
    {
        return process_ip_string(get_config_para("PLC_IP"));
    }
}

void GDataFactory::init_http_service()
{
    QString configFileName = search_http_config_file();
    if(configFileName == nullptr)
        QLOG_WARN()<<"search http config file failed!";
    else
    {
        // Configure and start the TCP listener
        QSettings* listenerSettings=new QSettings(configFileName,QSettings::IniFormat,this);
        listenerSettings->beginGroup("listener");
        new HttpListener(listenerSettings,new HttpRequestHandler(this),this);
        QLOG_INFO()<<"init http service success!";
    }
}

void GDataFactory::clear_alter_production()
{
    board_A_serial_number = "";
    board_B_serial_number = "";
    boardSign = 0;
    m_pMachineNamesOfChangeProduction.clear();
}

QList<QList<QString> > GDataFactory::get_all_machine_change_info()
{
    QMutexLocker lock(&m_mutex);
    QList<QList<QString>> lstAll;
    QString strSql = QString("select * from public.%1").arg(constCurrentChangeInfoTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
//        QLOG_INFO()<<"query success!";
        while(queryResult.next())
        {
            QList<QString> lstChangeInfo;
            lstChangeInfo.push_back(queryResult.value(1).toString());
            lstChangeInfo.push_back(queryResult.value(2).toString());
            lstChangeInfo.push_back(queryResult.value(3).toString());
            lstChangeInfo.push_back(queryResult.value(4).toString());
            lstChangeInfo.push_back(queryResult.value(5).toString());
            lstChangeInfo.push_back(queryResult.value(6).toString());
            lstChangeInfo.push_back(queryResult.value(7).toString());
            lstAll.push_back(lstChangeInfo);
        }
    }
    else {
        QLOG_WARN()<<"query failed!";
    }
    return lstAll;
}

QSqlQuery GDataFactory::get_query_result(QString strSql)
{
    QMutexLocker lock(&m_mutex);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
        QLOG_INFO()<<"GDataFactory get_query_result success!";
    else
        QLOG_WARN()<<"GDataFactory get_query_result failed!";
    return queryResult;
}

bool GDataFactory::truncate_change_info_table()
{
    QMutexLocker lock(&m_mutex);
    QString strTruncate = QString("truncate table public.%1").arg(constCurrentChangeInfoTable);
    QString tmpEr;
    if(GDataFactory::get_pgsql()->ExecSql(strTruncate,tmpEr))
    {
        GDataFactory::get_board_style_wgtA()->save_plan_info();
        GDataFactory::get_board_style_wgtB()->save_plan_info();
        return true;
    }
    else {
        QLOG_WARN()<<"truncate the change info table failed!";
        return false;
    }
}

bool GDataFactory::truncate_change_info_table_pure()
{
    QMutexLocker lock(&m_mutex);
    QString strTruncate = QString("truncate table public.%1").arg(constCurrentChangeInfoTable);
    QString tmpEr;
    if(GDataFactory::get_pgsql()->ExecSql(strTruncate,tmpEr))
    {
        QLOG_WARN()<<"truncate the change info table success!";
        return true;
    }
    else {
        QLOG_WARN()<<"truncate the change info table failed!";
        return false;
    }
}

QString GDataFactory::HandleStr(int i)
{
    QString str,sResult;
        if(i<10)
        {
            str = QString::number(i);
            return "0000"+str;
        }
        else if(i<100)
        {
            str = QString::number(i);
            return "000"+str;
        }
        else if(i<1000)
        {
            str = QString::number(i);
            return "00"+str;
        }
        else if(i<10000)
        {
            str = QString::number(i);
            return "0"+str;
        }
        else
        {
            str = QString::number(i);
            return str;
        }
}

QString GDataFactory::DecodeStr(QString s)
{
    int i,l;
    int m[10];
    QChar n[10];
    QString ss[10],str,sResult;
    int Seed[10]={8,13,19,11,9,21,8,13,19,11};
    l = s.length()/5;
    for (i=0;i<=l-1;i++)
    {
        ss[i]=s.mid(i*5,5);
    }
    for (i=0;i<=l-1;i++)
    {
        m[i]=ss[i].toInt();
        int tmpR = static_cast<int>(qSqrt(static_cast<qreal>(m[i]+Seed[i])))-static_cast<int>(Seed[i]);
        n[i]=tmpR;
    }
    for (i=0;i<=l-1;i++)
        sResult=sResult+n[i];
    return sResult;
}

QString GDataFactory::EncodeStr(QString password)
{
    int Seed[10]={8,13,19,11,9,21,8,13,19,11};
    int i,l;
    int n[10],m[10];
    QString ss[10],sResult;
    l = password.length();
    for (i=0;i<=l-1;i++)
    {
        n[i]=password[i].toLatin1();
        m[i]=(n[i]+Seed[i])*(n[i]+Seed[i])-Seed[i];
        ss[i]=HandleStr(m[i]);
    }
    for(i=0;i<=l-1;i++)
        sResult=sResult+ss[i];
    return sResult;
}

void GDataFactory::save_AB_change_product_info(QList<QString> changeinfo)
{
//    QMutexLocker lock(&m_mutex);
    if(changeinfo.count()==9)
    {
        //check machine name exist in db or not,if not exist insert the current info,if exist update the entry
//        changeinfo[3].prepend("\'");
//        changeinfo[3].append("\'");
        QString strSql = QString("select * from public.%1 where \"Station_Name\"=\'%2\'").arg(constCurrentChangeInfoTable).arg(changeinfo[3]);
        QSqlQuery queryResult;

        int maxID = get_biggest_id(constCurrentChangeInfoTable);
        maxID+=1;
        strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constCurrentChangeInfoTable).
                    arg(maxID).
                    arg(changeinfo[0]).
                    arg(changeinfo[1]).
                    arg(changeinfo[2]).
                    arg(changeinfo[3]).
                    arg(changeinfo[4]).
                    arg(changeinfo[5]).
                    arg(changeinfo[6]).
                    arg(changeinfo[7]).
                    arg(changeinfo[8]);
        QString strErrorMsg;
        if(get_pgsql()->ExecSql(strSql,strErrorMsg))
            QLOG_INFO()<<"insert change product info success!";
        else
            QLOG_WARN()<<"insert change product info failed!";
    }
    else
    {
        QLOG_WARN()<<"save AB change product info to DB failed!";
    }
}

void GDataFactory::set_AB_change_product_info(QList<QString> boardinfo)
{
    //after scan the serial code and request data from mes,save the change product info
    if(boardSign == 0)
    {
        ListBoardInfoA = boardinfo;
//        set_config_para_1("BOARD_A_STYLE",ListBoardInfoA[3]);
//        set_config_para_1("HALF_MATERIAL_NUMBER_A",ListBoardInfoA[1]);
    }
    else {
        ListBoardInfoB = boardinfo;
//        set_config_para_1("BOARD_B_STYLE",ListBoardInfoB[3]);
//        set_config_para_1("HALF_MATERIAL_NUMBER_B",ListBoardInfoB[1]);
    }
}

QList<QString> GDataFactory::get_A_change_product_info()
{
    return ListBoardInfoA;
}

QList<QString> GDataFactory::get_B_change_product_info()
{
    return ListBoardInfoB;
}

int GDataFactory::get_biggest_id(const QString &tablename)
{
    QString strSql = QString("select * from public.%1 order by \"ID\" desc").arg(tablename);
    QSqlQuery queryResult;
    int id = -1;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            if(id > queryResult.value(0).toInt())
                continue;
            else
                id = queryResult.value(0).toInt();
        }
        if(id == -1)
        {
            id = 1;
            return id;
        }
        else
        {
            id += 1;
            return id;
        }
    }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
        return -1;
    }
}

bool GDataFactory::check_mac_exist(const QString &macaddr,QString* machinename)
{
    QString strSql = QString("select * from public.%1").arg(constRegisterMachineTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            if(macaddr == queryResult.value(2).toString())
            {
                *machinename = queryResult.value(4).toString();
                return true;
            }
            else
                continue;
        }
     }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
        return false;
    }
    return false;
}

QString GDataFactory::register_new_mac_machine(QList<QString> lstpara)
{
    int id = get_biggest_id(constRegisterMachineTable);
    QString strResultName = QString(u8"机台%1").arg(id);

    QString strSql = QString("insert into %1 values(%2,'%3','%4','%5','%6')").
            arg(constRegisterMachineTable).
            arg(id).
            arg(lstpara[0]).
            arg(lstpara[1]).
            arg(lstpara[2]).
            arg(strResultName);

    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        QLOG_TRACE()<<"insert into db success!";
        emit signal_register_new_station_success();
        return strResultName;
    }
    else
    {
        QLOG_WARN()<<"insert into db failed!";
        return "";
    }
}

QMap<QString,QList<QString>> GDataFactory::get_all_machine_name_from_db()
{
    QMap<QString,QList<QString>> mapInfo;

    QList<QString> lstname;
    QString strSql = QString("select * from public.%1").arg(constRegisterMachineTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
//        QLOG_TRACE()<<u8"query database success!";
        while(queryResult.next())
        {
            if(mapInfo.contains(queryResult.value(1).toString()))
                mapInfo[queryResult.value(1).toString()].push_back(queryResult.value(4).toString());
            else
            {
                mapInfo.insert(queryResult.value(1).toString(),lstname);
                mapInfo[queryResult.value(1).toString()].push_back(queryResult.value(4).toString());
            }
//            lstname.push_back(queryResult.value(4).toString());
        }
//        while(queryResult.next())
//            lstname.push_back(queryResult.value(4).toString());
     }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
    }
    return mapInfo;
}

QString GDataFactory::get_line_name_by_machine_name(QString machinename)
{
    QMap<QString,QList<QString>> mapAll = get_all_machine_name_from_db();
    QMap<QString,QList<QString>>::Iterator itr = mapAll.begin();
    while(itr!= mapAll.end())
    {
        QList<QString> lstMachineName = itr.value();
        for (int i=0;i<lstMachineName.size();i++) {
            if(machinename == lstMachineName[i])
                return itr.key();
            else
                continue;
        }
        itr++;
    }
    return "";
}

QString GDataFactory::search_http_config_file()
{
    QString binDir=QApplication::applicationDirPath();
    QString fileName("httpconfig.ini");

    QStringList searchList;
    searchList.append(binDir);

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            QLOG_INFO()<<"Using http ini file "<<fileName;
            return fileName;
        }
    }

    // not found
    foreach (QString dir, searchList)
    {
        QLOG_WARN()<<dir<<fileName<<" not found";
    }
    QLOG_FATAL()<<"Cannot find http ini file "<<fileName;
    return nullptr;
}

void GDataFactory::del_one_change_product_info(QString machinename)
{
    QMutexLocker lock(&m_mutex);
    QString strSql = QString("DELETE FROM public.%1 WHERE \"Station_Name\" = \'%2\'").arg(constCurrentChangeInfoTable).arg(machinename);
    QString queryResult;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,queryResult))
    {
        QLOG_WARN()<<u8"delete the entry "<<machinename<<" SUCESS!";
     }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
    }
}

void GDataFactory::slot_rev_serial_number(const QByteArray &data)
{
    if(get_history_display_wgt()->isVisible())
    {
        get_history_display_wgt()->set_serial_number(QString(data));
        //when we get sn from serial port,send the get msg to MES to get the board with the current sn details
        GDataFactory::get_mes_process()->exec_http_get(QString(data));
        QLOG_INFO()<<"rev history display serial number:"<<QString(data);
    }
    else {
        QLOG_INFO()<<"the display log widget is not visible,do not set the edit text";
    }

    if(boardSign == 1)
    {
        if(get_board_style_wgtB()->isVisible())
        {
            get_board_style_wgtB()->set_serial_number(QString(data));
            board_B_serial_number = QString(data);
            //when we get sn from serial port,send the get msg to MES to get the board with the current sn details
            GDataFactory::get_mes_process()->exec_http_get(board_B_serial_number);
            QLOG_INFO()<<"rev board B serial number:"<<board_B_serial_number;
            boardSign++;
        }
        else {
            QLOG_INFO()<<"the boardB widget is not visible,do not set the edit text";
        }
    }

    if(boardSign == 0)
    {
        if(get_board_style_wgtA()->isVisible())
        {
            get_board_style_wgtA()->set_serial_number(QString(data));
            board_A_serial_number = QString(data);
            GDataFactory::get_mes_process()->exec_http_get(board_A_serial_number);
            QLOG_INFO()<<"rev board A serial number:"<<board_A_serial_number;
            boardSign++;
        }
        else {
            QLOG_INFO()<<"the boardA widget is not visible,do not set the edit text";
        }
    }
}

void GDataFactory::slot_change_production_failed(QString machinename, QString linename)
{
    IMessageBox* msgBox = new IMessageBox(3);
//    msgBox->warning(u8"换产失败!");
//    QLOG_WARN()<<u8"换产失败";
    msgBox->warning(u8"当前板子配置文件不存在!");
    return;
}

quint8 GDataFactory::process_ip_string(const QString ipstr)
{
    int nSize = ipstr.size();
    int nCount = 0;
    QString mData;
    mData.clear();
    QChar mChar;
    for(int idx = 0; idx<nSize; idx++)
    {
        mChar = ipstr[idx];
        if(mChar == '.')
        {
            nCount++;
        }
        else if(mChar != '.' && nCount == 3 && (mChar >= '0' || mChar <= '9'))
        {
            mData.append(mChar);
        }
    }
    return static_cast<quint8>(mData.toInt());
}
