#include "testwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include "circlewidget.h"
#include <QLabel>
#include <QDebug>
#include "gdatafactory.h"
#include <QEventLoop>
TestWidget::TestWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();

    QHBoxLayout* hMiddle = new QHBoxLayout();
    CircleWidget* circleGreen = new CircleWidget();
    circleGreen->setStyleSheet("QWidget{min-width:20px;max-width:20px;min-height:20px;max-height:20px;}");
    CircleWidget* circleRed = new CircleWidget();
    circleRed->setStyleSheet("QWidget{min-width:20px;max-width:20px;min-height:20px;max-height:20px;}");
    CircleWidget* circleBlue= new CircleWidget();
    circleBlue->setStyleSheet("QWidget{min-width:20px;max-width:20px;min-height:20px;max-height:20px;}");
    circleGreen->set_color(0);
    circleRed->set_color(1);
    circleBlue->set_color(2);
    QLabel* labelAlreadyDone = new QLabel(u8"已完成");
    QLabel* labelDoing = new QLabel(u8"进行中");
    QLabel* labelWillDone = new QLabel(u8"待完成");
    hMiddle->addStretch(15);
    hMiddle->addWidget(circleGreen);
    hMiddle->addWidget(labelAlreadyDone);
    hMiddle->addWidget(circleRed);
    hMiddle->addWidget(labelDoing);
    hMiddle->addWidget(circleBlue);
    hMiddle->addWidget(labelWillDone);
    hMiddle->addStretch(1);

    vTop = new QVBoxLayout();
    labelCurrentLine = new QLabel(u8"当前未设置产线");
    labelCurrentLine->setStyleSheet("color:rgb(0,0,0);");
    labelCurrentNum = new QLabel(u8"当前产品:");
    vTop->addWidget(labelCurrentLine);
    vTop->addWidget(labelCurrentNum);
//    vTop->addStretch(1);

    QHBoxLayout* hBottom = new QHBoxLayout();
    QVBoxLayout* vBox1= new QVBoxLayout();
    btnTest = new QPushButton();
    connect(btnTest,SIGNAL(clicked()),this,SLOT(slot_start_change_production()));
    btnTest->setStyleSheet("QPushButton{"
                           "background-color:rgba(0,0,0,0);"
                           "border-color:rgba(0,0,0,0);"
                           "min-width:200px;"
                           "max-width:200px;"
                           "min-height:200px;"
                           "max-height:200px;}"
                           "QPushButton:hover{border-color:rgba(0,0,0,0);}");
    const QIcon startIcon = QIcon::fromTheme("",QIcon(":/icon/onebutton.png"));
    btnTest->setIcon(startIcon);
    btnTest->setIconSize(QSize(200,200));
    vBox1->addWidget(btnTest);

    circleWgt1 = new CircleWidget();
    circleWgt1->setStyleSheet("QWidget{min-width:20px;max-width:20px;min-height:20px;max-height:20px;}");
    circleWgt1->set_color(2);
    circleWgt2 = new CircleWidget();
    circleWgt2->setStyleSheet("QWidget{min-width:20px;max-width:20px;min-height:20px;max-height:20px;}");
    circleWgt2->set_color(2);
    circleWgt3 = new CircleWidget();
    circleWgt3->setStyleSheet("QWidget{min-width:20px;max-width:20px;min-height:20px;max-height:20px;}");
    circleWgt3->set_color(2);
    circleWgt4 = new CircleWidget();
    circleWgt4->setStyleSheet("QWidget{min-width:20px;max-width:20px;min-height:20px;max-height:20px;}");
    circleWgt4->set_color(2);
    QVBoxLayout* vBox2 = new QVBoxLayout();
    QHBoxLayout* vBox2_hBox1 = new QHBoxLayout();
    QLabel* labelStart = new QLabel(u8"Start");
    labelStart->adjustSize();
    vBox2_hBox1->addWidget(circleWgt1);
    vBox2_hBox1->addWidget(labelStart);
    QHBoxLayout* vBox2_hBox2 = new QHBoxLayout();
    QLabel* labelMachineModel = new QLabel(u8"扫码获取机型");
    labelMachineModel->adjustSize();
    vBox2_hBox2->addWidget(circleWgt2);
    vBox2_hBox2->addWidget(labelMachineModel);
    QHBoxLayout* vBox2_hBox3 = new QHBoxLayout();
    QLabel* labelPlan = new QLabel(u8"测试方案自动更新");
    labelPlan->adjustSize();
    vBox2_hBox3->addWidget(circleWgt3);
    vBox2_hBox3->addWidget(labelPlan);
    QHBoxLayout* vBox2_hBox4 = new QHBoxLayout();
    QLabel* labelEnd = new QLabel(u8"End");
    labelEnd->adjustSize();
    vBox2_hBox4->addWidget(circleWgt4);
    vBox2_hBox4->addWidget(labelEnd);
    vBox2->addStretch(5);
    vBox2->addLayout(vBox2_hBox1);
    vBox2->addStretch(1);
    vBox2->addLayout(vBox2_hBox2);
    vBox2->addStretch(1);
    vBox2->addLayout(vBox2_hBox3);
    vBox2->addStretch(1);
    vBox2->addLayout(vBox2_hBox4);
    vBox2->addStretch(5);

    hBottom->addStretch(5);
    hBottom->addLayout(vBox1);
    hBottom->addStretch(2);
    hBottom->addLayout(vBox2);
    hBottom->addStretch(5);
    vAll->addLayout(vTop);
    vAll->addLayout(hMiddle);
    vAll->addLayout(hBottom);
    vAll->setStretch(0,1);
    vAll->setStretch(1,1);
    vAll->setStretch(2,16);
    this->setLayout(vAll);
}

void TestWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));

    painter.drawLine(10,vTop->geometry().bottom()+5,this->width()-10,vTop->geometry().bottom()+5);

    CircleWidget* cw[] = {circleWgt1,circleWgt2,circleWgt3,circleWgt4};
    for (int i=0;i<3;i++)
    {
        painter.drawLine(cw[i]->geometry().x()+cw[i]->width()/2,cw[i]->geometry().y()+cw[i]->height()/2,
                         cw[i+1]->geometry().x()+cw[i+1]->width()/2,cw[i+1]->geometry().y()+cw[i+1]->height()/2);
    }
}

void TestWidget::slot_rev_query_result(QString strCurrentLine, QString strCurrentNum)
{
    this->labelCurrentLine->clear();
    this->labelCurrentNum->clear();
//    strCurrentLine.prepend(u8"当前产线:");
    this->labelCurrentLine->setText(strCurrentLine);
    strCurrentNum.prepend(u8"当前产品:");
    this->labelCurrentNum->setText(strCurrentNum);
}

void TestWidget::slot_start_change_production()
{
    btnTest->setEnabled(false);
    const QIcon startIcon = QIcon::fromTheme("",QIcon(":/icon/changing.png"));
    btnTest->setIcon(startIcon);
    btnTest->setIconSize(QSize(200,200));

    GDataFactory::get_factory()->clear_alter_production();
    GDataFactory::get_factory()->read_serial_number();
    this->circleWgt1->set_color(0);
    this->circleWgt2->set_color(1);
    this->circleWgt3->set_color(2);
    this->circleWgt4->set_color(2);
    if(GDataFactory::get_factory()->get_current_line_info() == TCON_LINE)
        GDataFactory::get_test_plan_wgt()->show();
    else if(GDataFactory::get_factory()->get_current_line_info() == ELEC_EQUIP_LINE)
        GDataFactory::get_elec_equip_test_plan_wgt()->show();
    else
        GDataFactory::get_tv_plan_wgt()->show();
}

void TestWidget::slot_change_btn_icon()
{
    const QIcon startIcon = QIcon::fromTheme("",QIcon(":/icon/onebutton.png"));
    btnTest->setIcon(startIcon);
    btnTest->setIconSize(QSize(200,200));
    this->circleWgt1->set_color(2);
    this->circleWgt2->set_color(2);
    this->circleWgt3->set_color(2);
    this->circleWgt4->set_color(2);
    this->btnTest->setEnabled(true);
}

void TestWidget::slot_save_plan()
{
    this->circleWgt2->set_color(0);
    this->circleWgt3->set_color(1);
}

void TestWidget::slot_change_production_already_complete()
{
    emit signal_kill_timeout_timer();
    QEventLoop loop;
    QTimer::singleShot(1000, &loop, SLOT(quit()));
    loop.exec();
    this->circleWgt3->set_color(0);
    this->circleWgt4->set_color(1);
    QTimer::singleShot(1000, &loop, SLOT(quit()));
    loop.exec();
    this->circleWgt4->set_color(0);
    QTimer::singleShot(1000, &loop, SLOT(quit()));
    loop.exec();
    slot_change_btn_icon();
}
