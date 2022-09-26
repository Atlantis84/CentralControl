#include "onebuttonchangewidget.h"
#include "TitleBar.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "testwidget.h"
#include "gdatafactory.h"
#pragma execution_character_set("utf-8")
OneButtonChangeWidget::OneButtonChangeWidget(QWidget *parent,const QString lineNum,const QString lineName) : QWidget(parent)
{
    setWindowFlags(Qt::SubWindow);
    setWindowIcon(QIcon(":/icon/24x24.png"));

    QVBoxLayout* vAll = new QVBoxLayout();

    QVBoxLayout* vTop = new QVBoxLayout();
    labelLineName = new QLabel(lineName);
    QString tmpStr = u8"当前产品:";
    tmpStr.append(lineNum);
    labelLineNum = new QLabel(tmpStr);
    vTop->addWidget(labelLineName);
    vTop->addWidget(labelLineNum);

    QHBoxLayout* hBox1 = new QHBoxLayout();
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
    hBox1->addStretch(15);
    hBox1->addWidget(circleGreen);
    hBox1->addWidget(labelAlreadyDone);
//    hBox1->addStretch(1);
    hBox1->addWidget(circleRed);
    hBox1->addWidget(labelDoing);
//    hBox1->addStretch(1);
    hBox1->addWidget(circleBlue);
    hBox1->addWidget(labelWillDone);
    hBox1->addStretch(1);

    QHBoxLayout* hBox2 = new QHBoxLayout();
//    TestWidget* testWgt = new TestWidget();
    hBox2->addWidget(GDataFactory::get_test_wgt());
//    QVBoxLayout* hBox2_vBox1 = new QVBoxLayout();
//    QVBoxLayout* hBox2_vBox2 = new QVBoxLayout();
//    hBox2->addLayout(hBox2_vBox1);
//    hBox2->addLayout(hBox2_vBox2);
    vAll->addLayout(vTop);
    vAll->addLayout(hBox1);
    vAll->addLayout(hBox2);
    vAll->setStretch(0,2);
    vAll->setStretch(1,1);
    vAll->setStretch(2,12);
    this->setLayout(vAll);
}

void OneButtonChangeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,255),1));
    painter.drawRect(0,0,this->width()-1,this->height()-1);
    int lineBottom = labelLineNum->geometry().bottom() + 5;
    painter.drawLine(10,lineBottom,this->width()-20,lineBottom);
}
