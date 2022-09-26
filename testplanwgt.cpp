#include "testplanwgt.h"
#include "TitleBar.h"
#include <QIcon>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeWidget>
#include <QStyleFactory>
#include <QAbstractItemView>
#include <QTimerEvent>
#include "QsLog.h"
#include "gdatafactory.h"
#include "imessagebox.h"
#pragma execution_character_set("utf-8")
TestPlanWgt::TestPlanWgt(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget/*|Qt::WindowStaysOnTopHint*/);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(1000,500);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"TCON换产计划");
    pTitleBar->setFixedWidth(1000);
    installEventFilter(pTitleBar);

    m_pUdpSocket = new QUdpSocket(this);
//    m_pUdpSocket->bind(QHostAddress("127.0.0.1"),60000);
    connect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_rev_plc_msg()));

    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hHead = new QHBoxLayout();
    QVBoxLayout* hHead_V1 = new QVBoxLayout();
    treeWidget = new DragTreeWidget();
    treeWidget->setFont(QFont(u8"Microsoft YaHei",12,QFont::Normal,false));

    hHead_V1->addWidget(treeWidget);

    QVBoxLayout* hHead_V2 = new QVBoxLayout();
    QLabel* labelScanCode = new QLabel(u8"请扫码");
    labelScanCode->setStyleSheet("font-size:30pt");
    labelScanCode->setAlignment(Qt::AlignCenter);
    QLabel* labelPicture = new QLabel();
    labelPicture->setPixmap(QPixmap(":/icon/code.jpg"));
    labelPicture->setAlignment(Qt::AlignCenter);
    hHead_V2->addStretch(1);
    hHead_V2->addWidget(labelScanCode);
    hHead_V2->addWidget(labelPicture);
    hHead_V2->addStretch(1);

    QVBoxLayout* hHead_V3 = new QVBoxLayout();
//    hHead_V3->addStretch(1);
    hHead_V3->addWidget(GDataFactory::get_board_style_wgtA());
    hHead_V3->addWidget(GDataFactory::get_board_style_wgtB());
//    hHead_V3->addStretch(1);

    hHead->addLayout(hHead_V2);
    hHead->addLayout(hHead_V3);
    hHead->addLayout(hHead_V1);
    hHead->setStretch(0,4);
    hHead->setStretch(1,6);
    hHead->setStretch(2,2);

    QHBoxLayout* hBottom = new QHBoxLayout();
    QPushButton* btnSave = new QPushButton(u8"   保  存   ");
    btnSave->setStyleSheet("QPushButton{min-width:200;color:rgba(0,0,0,150);background-color:rgba(255,255,255,255);}\
                            QPushButton:hover{color:rgba(255,255,255,255);background-color:rgba(24,144,255,255);}");
    connect(btnSave,SIGNAL(clicked()),this,SLOT(slot_save_test_plan()));
    hBottom->addStretch(8);
    hBottom->addWidget(btnSave);
    hBottom->addStretch(5);
    vAll->addSpacing(pTitleBar->height());
    vAll->addLayout(hHead);
    vAll->addLayout(hBottom);
    this->setLayout(vAll);
}

void TestPlanWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void TestPlanWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit signal_change_btn_icon();
    GDataFactory::get_factory()->stop_serial_port();
}

void TestPlanWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    treeWidget->clear();
    QList<QTreeWidgetItem*> items;
    QMap<QString,QList<QString>> mapAll = GDataFactory::get_factory()->get_all_machine_name_from_db();
    QMap<QString,QList<QString>>::Iterator itr = mapAll.begin();
    while(itr!= mapAll.end())
    {
        QTreeWidgetItem* tmpItem = new QTreeWidgetItem(treeWidget,QStringList(itr.key()));
        QList<QString> lstChild = itr.value();
        for (int i=0;i<lstChild.size();i++) {
            QTreeWidgetItem* tmpChildItem = new QTreeWidgetItem(tmpItem,QStringList(lstChild[i]));
            tmpItem->addChild(tmpChildItem);
        }
        treeWidget->addTopLevelItem(tmpItem);
        itr++;
    }

    treeWidget->setHeaderHidden(true);
    treeWidget->setStyle(QStyleFactory::create("windows"));
    treeWidget->setStyleSheet("background-color:rgb(240,242,245);");
    treeWidget->expandAll();
}

void TestPlanWgt::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timer_ID)
    {
        m_time_out++;
        if(m_time_out == 120)
        {
            killTimer(m_timer_ID);
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"换产超时!");
            emit signal_change_btn_icon();
        }
    }
}

void TestPlanWgt::slot_save_test_plan()
{
    GDataFactory::get_factory()->clear_change_production_info_for_PLC_or_AOI();
    m_time_out=0;
    m_timer_ID = startTimer(1000);
    //truncate the change info table
    if(GDataFactory::get_factory()->truncate_change_info_table())
        emit signal_save_plan();
    else {
        QLOG_WARN()<<"truncate change info table failed!";
        return;
    }
    GDataFactory::get_factory()->stop_serial_port();

    QByteArray dataArray;
    QByteArray dataArray1;
    QByteArray dataArray2;
    QByteArray dataArray3;
    QByteArray dataArray4;
    QByteArray dataArray5;
    QByteArray dataArray6;
    //notice PLC or AOI,changing production now
    /******************************notice vision change production info*****************************/
    QString strBdType = "";
    QString strHfMaterialNum="";
    QString tmpMachineName = u8"机台1";
    QMap<QString,QString> tmpMapInfo = GDataFactory::get_factory()->get_PLC_or_AOI_change_info(tmpMachineName);
    if(tmpMapInfo.size() == 1)
    {
        QMap<QString,QString>::Iterator itr = tmpMapInfo.begin();
        while(itr != tmpMapInfo.end())
        {
            strHfMaterialNum = itr.value();
            strBdType = itr.key();
            itr++;
        }
        dataArray1.append(strBdType.toLatin1());
        dataArray1.append(strHfMaterialNum.toLatin1());
    }
    else
    {
        for(int i=0;i<5;i++)
            strBdType.append(GDataFactory::get_factory()->get_config_para("BOARD_A_STYLE")[i]);
        strHfMaterialNum.append(GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_A"));
        dataArray1.append(strBdType.toLatin1());
        dataArray1.append(strHfMaterialNum.toLatin1());
    }
    QLOG_WARN()<<"Station-1 boardType and HFS is:"<<strBdType<<","<<strHfMaterialNum;

    strBdType = "";
    strHfMaterialNum = "";
    tmpMachineName = u8"机台2";
    tmpMapInfo = GDataFactory::get_factory()->get_PLC_or_AOI_change_info(tmpMachineName);
    if(tmpMapInfo.size() == 1)
    {
        QMap<QString,QString>::Iterator itr = tmpMapInfo.begin();
        while(itr != tmpMapInfo.end())
        {
            strHfMaterialNum = itr.value();
            strBdType = itr.key();
            itr++;
        }
        dataArray2.append(strBdType.toLatin1());
        dataArray2.append(strHfMaterialNum.toLatin1());
    }
    else
    {
        for(int i=0;i<5;i++)
            strBdType.append(GDataFactory::get_factory()->get_config_para("BOARD_A_STYLE")[i]);
        strHfMaterialNum.append(GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_A"));
        dataArray2.append(strBdType.toLatin1());
        dataArray2.append(strHfMaterialNum.toLatin1());
    }
    QLOG_WARN()<<"Station-2 boardType and HFS is:"<<strBdType<<","<<strHfMaterialNum;

    strBdType = "00000";
    strHfMaterialNum = "000000";
    dataArray3.append(strBdType.toLatin1());
    dataArray3.append(strHfMaterialNum.toLatin1());

    strBdType = "";
    strHfMaterialNum = "";
    tmpMachineName = u8"机台3";
    tmpMapInfo = GDataFactory::get_factory()->get_PLC_or_AOI_change_info(tmpMachineName);
    if(tmpMapInfo.size() == 1)
    {
        QMap<QString,QString>::Iterator itr = tmpMapInfo.begin();
        while(itr != tmpMapInfo.end())
        {
            strHfMaterialNum = itr.value();
            strBdType = itr.key();
            itr++;
        }
        dataArray4.append(strBdType.toLatin1());
        dataArray4.append(strHfMaterialNum.toLatin1());
    }
    else
    {
        for(int i=0;i<5;i++)
            strBdType.append(GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE")[i]);
        strHfMaterialNum.append(GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_B"));
        dataArray4.append(strBdType.toLatin1());
        dataArray4.append(strHfMaterialNum.toLatin1());
    }
    QLOG_WARN()<<"Station-3 boardType and HFS is:"<<strBdType<<","<<strHfMaterialNum;

    strBdType = "";
    strHfMaterialNum = "";
    tmpMachineName = u8"机台4";
    tmpMapInfo = GDataFactory::get_factory()->get_PLC_or_AOI_change_info(tmpMachineName);
    if(tmpMapInfo.size() == 1)
    {
        QMap<QString,QString>::Iterator itr = tmpMapInfo.begin();
        while(itr != tmpMapInfo.end())
        {
            strHfMaterialNum = itr.value();
            strBdType = itr.key();
            itr++;
        }
        dataArray5.append(strBdType.toLatin1());
        dataArray5.append(strHfMaterialNum.toLatin1());
    }
    else
    {
        for(int i=0;i<5;i++)
            strBdType.append(GDataFactory::get_factory()->get_config_para("BOARD_B_STYLE")[i]);
        strHfMaterialNum.append(GDataFactory::get_factory()->get_config_para("HALF_MATERIAL_NUMBER_B"));
        dataArray5.append(strBdType.toLatin1());
        dataArray5.append(strHfMaterialNum.toLatin1());
    }
    QLOG_WARN()<<"Station-4 boardType and HFS is:"<<strBdType<<","<<strHfMaterialNum;

    strBdType = "00000";
    strHfMaterialNum = "000000";
    dataArray6.append(strBdType.toLatin1());
    dataArray6.append(strHfMaterialNum.toLatin1());

    dataArray.append(dataArray1);
    dataArray.append(dataArray2);
    dataArray.append(dataArray3);
    dataArray.append(dataArray4);
    dataArray.append(dataArray5);
    dataArray.append(dataArray6);

//    for(int i=1;i<7;i++)
//    {
//        QString machineName=QString(u8"机台%1").arg(i);
//        QMap<QString,QString> tmpMapInfo = GDataFactory::get_factory()->get_PLC_or_AOI_change_info(machineName);
//        if(tmpMapInfo.size() == 1)
//        {
//            if(machineName == u8"机台3")
//            {
//                QString strBdType = "00000";
//                QString strHfMaterialNum="000000";
//                dataArray.append(strBdType.toLatin1());
//                dataArray.append(strHfMaterialNum.toLatin1());
//            }

//            QMap<QString,QString>::Iterator itr = tmpMapInfo.begin();
//            QString halfMaterialNum;
//            QString boardType;
//            while(itr != tmpMapInfo.end())
//            {
//                halfMaterialNum = itr.value();
//                boardType = itr.key();
//                itr++;
//            }
//            dataArray.append(boardType.toLatin1());
//            dataArray.append(halfMaterialNum.toLatin1());
//        }
//        else
//        {
//            if((i==6)&&(dataArray.length() == 66))
//                continue;
//            if((i==6)&&(dataArray.length() != 66)){
//                QString strBdType = "00000";
//                QString strHfMaterialNum="000000";
//                dataArray.append(strBdType.toLatin1());
//                dataArray.append(strHfMaterialNum.toLatin1());
//            }

//            if(i != 6)
//            {
//                QString strBdType = "";
//                QString strHfMaterialNum="";
//                if((machineName == u8"机台1") || (machineName == u8"机台2"))
//                {
//                    for(int i=0;i<5;i++)
//                        strBdType.append(GDataFactory::get_config_para("BOARD_A_STYLE")[i]);
//                    strHfMaterialNum.append(GDataFactory::get_config_para("HALF_MATERIAL_NUMBER_A"));
//                }

//                if((machineName == u8"机台3") || (machineName == u8"机台4"))
//                {
//                    for(int i=0;i<5;i++)
//                        strBdType.append(GDataFactory::get_config_para("BOARD_B_STYLE")[i]);
//                    strHfMaterialNum.append(GDataFactory::get_config_para("HALF_MATERIAL_NUMBER_B"));
//                }
//            }
//        }
//    }
//    QLOG_ERROR()<<GDataFactory::get_factory()->bytes_to_str(dataArray);
    /*the sentence is for auto test,send the change production info to VISION directly,the 0x79 is a sign for VISION distinguish the infos*/
//    dataArray.append(0x79);//vision sign
    send_message_to_plc(WRITE_PLC,ADDRESS_D600,dataArray.length(),dataArray);
    /*the sentence is for auto test,send the change production info to VISION directly,the 0x79 is a sign for VISION distinguish the infos*/
    /******************************notice vision change production info*****************************/
    this->hide();
}

void TestPlanWgt::send_message_to_plc(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array)
{
//    if(GDataFactory::get_factory()->get_system_state() == NULL_SYSTEM_STATE)
//        return;
    QHostAddress hAddr(GDataFactory::get_factory()->get_config_para("PLC_IP"));
    int hPort = GDataFactory::get_factory()->get_config_para("PLC_PORT").toInt();
    QByteArray btArray = make_plc_message(function_code,data_address,data_length,data_array);

    if(function_code == WRITE_PLC)
    {
        QLOG_INFO()<<"Send Msg To PLC:"<<GDataFactory::get_factory()->bytes_to_str(btArray);
        m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
    }
    else {
        QLOG_INFO()<<"Send Msg To VISION:"<<GDataFactory::get_factory()->bytes_to_str(data_array);
        m_pUdpSocket->writeDatagram(data_array,data_length,hAddr,static_cast<quint16>(hPort));
    }
}

void TestPlanWgt::send_message_to_vision_lead(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array)
{
    QHostAddress hAddr(GDataFactory::get_factory()->get_config_para("VISUAL_LEAD_IP"));
    int hPort = GDataFactory::get_factory()->get_config_para("VISUAL_LEAD_PORT").toInt();
    QByteArray btArray = make_plc_message(function_code,data_address,data_length,data_array);
    QLOG_INFO()<<"Send Msg To VISION:"<<GDataFactory::get_factory()->bytes_to_str(btArray);
    if(function_code == WRITE_PLC)
        m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
    else {
        m_pUdpSocket->writeDatagram(data_array,data_length,hAddr,static_cast<quint16>(hPort));
    }
}

QByteArray TestPlanWgt::make_plc_message(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array)
{
    QByteArray m_data;
    m_data.append(static_cast<char>(0x80));  //起始码
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x02));
    m_data.append(static_cast<char>(0x00));  //PLC IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(1)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));  //self IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(0)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0xFF));  //SID
    m_data.append(static_cast<char>(0x01));  //FINS

    switch (function_code)
    {
    case 1://read plc
        m_data.append(static_cast<char>(0x01));
        break;
    case 2://write plc
        m_data.append(static_cast<char>(0x02));
        break;
    }

    m_data.append(static_cast<char>(0x82));
    m_data.append(static_cast<char>(0xFF&(data_address>>8)));
    m_data.append(static_cast<char>(0xFF&data_address));
    m_data.append(static_cast<char>(0x00));
    int length = data_length/2;//plc recognize by word
    m_data.append(static_cast<char>(0xFF&(length>>8)));
    m_data.append(static_cast<char>(0xFF&length));

    if(function_code == 2)
        m_data.append(data_array);

    return m_data;
}

void TestPlanWgt::slot_rev_plc_msg()
{
    QByteArray datagram;
    datagram.resize(static_cast<int>(m_pUdpSocket->pendingDatagramSize()));
    while(m_pUdpSocket->hasPendingDatagrams())
    {
        m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
        QLOG_INFO()<<"REV PLC MSG:"<<GDataFactory::get_factory()->bytes_to_str(datagram);
    }
}

void TestPlanWgt::slot_kill_timeout_timer()
{
    killTimer(m_timer_ID);
}

void TestPlanWgt::slot_refresh_treewgt()
{
    treeWidget->clear();
    QList<QTreeWidgetItem*> items;
    QMap<QString,QList<QString>> mapAll = GDataFactory::get_factory()->get_all_machine_name_from_db();
    QMap<QString,QList<QString>>::Iterator itr = mapAll.begin();
    while(itr!= mapAll.end())
    {
        QTreeWidgetItem* tmpItem = new QTreeWidgetItem(treeWidget,QStringList(itr.key()));
        QList<QString> lstChild = itr.value();
        for (int i=0;i<lstChild.size();i++) {
            QTreeWidgetItem* tmpChildItem = new QTreeWidgetItem(tmpItem,QStringList(lstChild[i]));
            tmpItem->addChild(tmpChildItem);
        }
        treeWidget->addTopLevelItem(tmpItem);
        itr++;
    }

    treeWidget->setHeaderHidden(true);
    treeWidget->setStyle(QStyleFactory::create("windows"));
    treeWidget->setStyleSheet("background-color:rgb(240,242,245);");
    treeWidget->expandAll();
}
