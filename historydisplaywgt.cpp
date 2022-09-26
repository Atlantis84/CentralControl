#include "historydisplaywgt.h"
#include "TitleBar.h"
#include <QPainter>
#include "gdatafactory.h"
#include "QsLog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QTabWidget>
HistoryDisplayWgt::HistoryDisplayWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget|Qt::WindowStaysOnTopHint);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(800,800);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"数据管理");
    pTitleBar->setFixedWidth(800);
    installEventFilter(pTitleBar);

    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->addTab(GDataFactory::get_station_tab_wgt(),u8"基础数据查询");
    tabWidget->addTab(GDataFactory::get_user_tab_wgt(),u8"用户数据管理");

    QVBoxLayout* vAll = new QVBoxLayout();
//    vAll->setMargin(0);
//    vAll->setSpacing(0);
    vAll->addSpacing(40);
    vAll->addWidget(tabWidget);
    this->setLayout(vAll);
}

void HistoryDisplayWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(235,236,240)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void HistoryDisplayWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    GDataFactory::get_factory()->stop_serial_port();
}

void HistoryDisplayWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    GDataFactory::get_factory()->read_serial_number();
}

void HistoryDisplayWgt::set_serial_number(QString boardsn)
{
    QLOG_INFO()<<boardsn;
}

void HistoryDisplayWgt::set_boardtype_halfmaterialnumber(QString boardtype, QString halfmaterialnumber)
{
    ;
}

void HistoryDisplayWgt::slot_save()
{
    GDataFactory::get_factory()->clear_A_sn();
    GDataFactory::get_factory()->clear_B_sn();

    for (int i=0;i<100;i++)
    {
        if(tableWidgetA->item(i,0)->text().trimmed() != "")
        {
            GDataFactory::get_factory()->add_A_sn(i,tableWidgetA->item(i,0)->text().trimmed());
        }

        if(tableWidgetB->item(i,0)->text().trimmed() != "")
        {
            GDataFactory::get_factory()->add_B_sn(i,tableWidgetB->item(i,0)->text().trimmed());
        }
    }
    this->hide();
}

QWidget *HistoryDisplayWgt::createTableWidgetA()
{
    QFont* font = new QFont("KaiTi", 15, QFont::StyleNormal);
    tableWidgetA = new QTableWidget(100,1);

    tableWidgetA->setFont(*font);
    tableWidgetA->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"板A_SN";
    tableWidgetA->setHorizontalHeaderLabels(header);
    tableWidgetA->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidgetA->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,0);}"
                               "QTableWidget::item::selected{color:rgb(0,0,0);"
                               "background-color:rgb(207,207,217);}");
    tableWidgetA->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidgetA->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidgetA->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i=0;i<100;i++)
    {
        if(i<12)
        {
            tableWidgetA->setItem(i,0,new QTableWidgetItem(GDataFactory::get_factory()->get_config_para(QString("A_SN%1").arg(i+1))));
        }
        else
            tableWidgetA->setItem(i,0,new QTableWidgetItem(""));
        tableWidgetA->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
     }
    return tableWidgetA;
}

QWidget *HistoryDisplayWgt::createTableWidgetB()
{
    QFont* font = new QFont("KaiTi", 15, QFont::StyleNormal);
    tableWidgetB = new QTableWidget(100,1);

    tableWidgetB->setFont(*font);
    tableWidgetB->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"板B_SN";
    tableWidgetB->setHorizontalHeaderLabels(header);
    tableWidgetB->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidgetB->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,0);}"
                               "QTableWidget::item::selected{color:rgb(0,0,0);"
                               "background-color:rgb(207,207,217);}");
    tableWidgetB->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    tableWidgetB->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgb(0,0,0);color:rgb(0,0,0);background-color:rgba(216,217,222,255);}");
    tableWidgetB->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i=0;i<100;i++)
    {
        if(i<10)
            tableWidgetB->setItem(i,0,new QTableWidgetItem(GDataFactory::get_factory()->get_config_para(QString("B_SN%1").arg(i+1))));
        else
            tableWidgetB->setItem(i,0,new QTableWidgetItem(""));
        tableWidgetB->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
     }
    return tableWidgetB;
}
