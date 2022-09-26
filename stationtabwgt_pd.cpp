#include "stationtabwgt_pd.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QTimeEdit>
#include <QDateEdit>
#include <QPushButton>
StationTabWgt::StationTabWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vBox1 = new QVBoxLayout();

    checkStationName = new QCheckBox(u8"站点名");
    checkDate = new QCheckBox(u8"日期");
    checkTime = new QCheckBox(u8"时间");
    checkStationName->setStyleSheet("QCheckBox{color:rgb(0,0,0);font-family:\"Microsoft YaHei\";font-size:12pt;}");
    checkDate->setStyleSheet("QCheckBox{color:rgb(0,0,0);font-family:\"Microsoft YaHei\";font-size:12pt;}");
    checkTime->setStyleSheet("QCheckBox{color:rgb(0,0,0);font-family:\"Microsoft YaHei\";font-size:12pt;}");

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QLabel* labelStationName = new QLabel(u8"站点名:");
    labelStationName->setStyleSheet("QLabel{color:rgba(0,0,0,255);}");
    QLineEdit* editStationName = new QLineEdit();
    editStationName->setStyleSheet("QLineEdit{color:rgba(0,0,0,100);border:1px solid rgba(0,0,0,100);\
                                   border-radius:5px;min-width:100px;\
                                   background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
                                   font-size: 20px;font-weight:normal;\
                                   }");
    QPushButton* pushBtnQuery = new QPushButton(u8" 查  询 ");
    pushBtnQuery->setStyleSheet("QPushButton{color:#000000;background-color:rgba(255,255,255,0);border-radius:5px;\
                        min-width:100px;max-width:500px;min-height:20px;font-family:'Microsoft YaHei';}");

    hBox2->addStretch(1);
    hBox2->addWidget(checkStationName);
    hBox2->addSpacing(20);
    hBox2->addWidget(checkDate);
    hBox2->addSpacing(20);
    hBox2->addWidget(checkTime);
    hBox2->addStretch(1);
    hBox2->addWidget(labelStationName);
    hBox2->addWidget(editStationName);
    hBox2->addStretch(1);
    hBox2->addWidget(pushBtnQuery);
    hBox2->addStretch(1);
    vBox1->addLayout(hBox2);

    QLabel* labelDate = new QLabel(u8"日期:");
    labelDate->setStyleSheet("QLabel{color:rgba(0,0,0,255);}");
    QDateEdit* editDate= new QDateEdit();
    editDate->setDateTime(QDateTime::currentDateTime());
    editDate->setDisplayFormat("yyyy/MM/dd");
    editDate->setStyleSheet("QDateEdit{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);\
                                   background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
                                   font-size: 20px;font-weight:normal;\
                                   border-radius:0px;min-width:130px;\
                                   selection-background-color:rgba(0,0,0,100);\
                                   selection-color:#5D5C6C;}\
                                   ");

    QLabel* labelDate1 = new QLabel(u8"-");
    labelDate1->setStyleSheet("QLabel{color:rgba(0,0,0,255);}");
    QDateEdit* editDate1= new QDateEdit();
    editDate1->setDateTime(QDateTime::currentDateTime());
    editDate1->setDisplayFormat("yyyy/MM/dd");
    editDate1->setStyleSheet("QDateEdit{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);\
                                   background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
                                   font-size: 20px;font-weight:normal;\
                                   border-radius:0px;min-width:130px;\
                                   selection-background-color:rgba(0,0,0,100);\
                                   selection-color:#5D5C6C;}");

    QHBoxLayout* hBox4 = new QHBoxLayout();
    QLabel* labelTime = new QLabel(u8"时间:");
    labelTime->setStyleSheet("QLabel{color:rgba(0,0,0,255);}");
    QTimeEdit* editTime= new QTimeEdit();
    editTime->setDateTime(QDateTime::currentDateTime());
    editTime->setDisplayFormat("hh:mm:ss");
    editTime->setStyleSheet("QTimeEdit{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);\
                                   background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
                                   font-size: 20px;font-weight:normal;\
                                   border-radius:0px;\
                                   min-width:100px;\
                                   selection-background-color:rgba(0,0,0,100);\
                                   selection-color:#5D5C6C;}");

    QLabel* labelTime1 = new QLabel(u8"-");
    labelTime1->setStyleSheet("QLabel{color:rgba(0,0,0,255);}");
    QTimeEdit* editTime1= new QTimeEdit();
    editTime1->setDateTime(QDateTime::currentDateTime());
    editTime1->setDisplayFormat("hh:mm:ss");
    editTime1->setStyleSheet("QTimeEdit{color:rgb(0,0,0);border:1px solid rgba(0,0,0,100);\
                                   background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
                                   font-size: 20px;font-weight:normal;\
                                   border-radius:0px;\
                                   min-width:100px;\
                                   selection-background-color:rgba(0,0,0,100);\
                                   selection-color:#5D5C6C;}");
    hBox4->addStretch(1);
    hBox4->addWidget(labelDate);
    hBox4->addWidget(editDate);
    hBox4->addWidget(labelDate1);
    hBox4->addWidget(editDate1);
    hBox4->addStretch(1);
    hBox4->addWidget(labelTime);
    hBox4->addWidget(editTime);
    hBox4->addWidget(labelTime1);
    hBox4->addWidget(editTime1);
    hBox4->addStretch(1);
    vBox1->addLayout(hBox4);

    QGroupBox* grpBox = new QGroupBox(u8"信息查询");
    grpBox->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
    grpBox->setLayout(vBox1);

    vAll->addWidget(grpBox);
    vAll->addWidget(createTableWidget());
    vAll->setStretch(0,1);
    vAll->setStretch(1,4);
    this->setLayout(vAll);
}

QWidget *StationTabWgt::createTableWidget()
{
    QFont* font = new QFont("KaiTi", 15, QFont::StyleNormal);
    tableWidget = new QTableWidget(20,4);

    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"站点名"<<u8"基础数据"<<u8"日期"<<u8"时间";
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,0);}"
                               "QTableWidget::item::selected{color:rgb(0,0,0);"
                               "background-color:rgb(207,207,217);}");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    return tableWidget;
}
