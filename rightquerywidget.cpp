#include "rightquerywidget.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "gdatafactory.h"
#pragma execution_character_set("utf-8")
RightQueryWidget::RightQueryWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    labelPath = new QLabel(u8"当前未选中路径");
    hBox1->addWidget(labelPath);
//    hBox1->addStretch(1);
    QHBoxLayout* hBox2 = new QHBoxLayout();

    QLabel* labelProductStyle = new QLabel(u8"产品类型:");
    labelProductStyle->setStyleSheet("color:rgb(0,0,0)");
    QComboBox* combProductStyle = new QComboBox();
    combProductStyle->insertItem(0,"TCON");
    combProductStyle->insertItem(1,u8"平度电装");
    combProductStyle->insertItem(2,u8"TV");
    combProductStyle->setCurrentIndex(0);
    combProductStyle->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                           "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);min-width:150px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    connect(combProductStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_comb_index_changed(int)));

    QLabel* labelProduceLineNum = new QLabel(u8"产线号:");
    labelProduceLineNum->setStyleSheet("color:rgb(0,0,0);");
    lineEditProduceNum = new QLineEdit();
    lineEditProduceNum->setReadOnly(true);
    lineEditProduceNum->setStyleSheet("QLineEdit{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                      "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);}"
                                      "QLineEdit:hover{border:2px solid rgba(0,0,0,100);}");
    lineEditProduceNum->setAlignment(Qt::AlignCenter);
    QRegExp regx("[1-9][0-9]+$");
    QValidator *validator = new QRegExpValidator(regx,lineEditProduceNum);
    lineEditProduceNum->setValidator(validator);

    QLabel* labelProduceLineName = new QLabel(u8"产线名:");
    labelProduceLineName->setStyleSheet("color:rgb(0,0,0);");
    comboboxProduceLineName = new QComboBox();
    comboboxProduceLineName->setEnabled(false);
    comboboxProduceLineName->setStyleSheet("QComboBox{border:1px solid rgba(0,0,0,100);font-family:Microsoft YaHei;font-size:20px;"
                                           "color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);min-width:150px;}"
                                           "QComboBox:hover{border:2px solid rgba(0,0,0,100);}");
    QMap<QString,QList<QString>> mapAll = GDataFactory::get_factory()->get_all_machine_name_from_db();
    QMap<QString,QList<QString>>::Iterator itr = mapAll.begin();
    int itemCount = 0;
    while (itr != mapAll.end())
    {
        comboboxProduceLineName->insertItem(itemCount,itr.key());
        itr++;
        itemCount++;
    }
    comboboxProduceLineName->setCurrentIndex(-1);

    btnQuery = new QPushButton(u8"  查   询  ");
    btnQuery->setEnabled(false);
    connect(btnQuery,SIGNAL(clicked()),this,SLOT(slot_query()));
    btnQuery->setStyleSheet("QPushButton{color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);min-height:30px;min-width:100px;}\
                            QPushButton:!enabled{border:1px solid rgba(0,0,0,30);}\
                            QPushButton:hover{color:rgb(255,255,255);background-color:rgba(24,144,255,255);}");
    btnReset = new QPushButton(u8"  重   置  ");
    btnReset->setEnabled(false);
    connect(btnReset,SIGNAL(clicked()),this,SLOT(slot_reset()));
    btnReset->setStyleSheet("QPushButton{color:rgba(0,0,0,150);background-color:rgba(0,0,0,0);min-height:30px;min-width:100px;}"
                            "QPushButton:!enabled{border:1px solid rgba(0,0,0,30);}\
                            QPushButton:hover{color:rgb(255,255,255);background-color:rgba(24,144,255,255);}");


    hBox2->addWidget(labelProductStyle);
    hBox2->addWidget(combProductStyle);
    hBox2->addWidget(labelProduceLineNum);
    hBox2->addWidget(lineEditProduceNum);
    hBox2->addWidget(labelProduceLineName);
    hBox2->addWidget(comboboxProduceLineName);
    hBox2->addWidget(btnQuery);
    hBox2->addWidget(btnReset);
    hBox2->addStretch(1);

    vAll->addLayout(hBox1);
    vAll->addLayout(hBox2);
    this->setLayout(vAll);
}

void RightQueryWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    GDataFactory::get_factory()->get_http_process_obj("TCON_TEST");
}

void RightQueryWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
}

void RightQueryWidget::slot_rev_path(const QString strPath)
{
    labelPath->clear();
    labelPath->setText(strPath);
    if(strPath == u8"换产管理/一键换产")
    {
        this->lineEditProduceNum->setReadOnly(false);
        this->comboboxProduceLineName->setEnabled(true);
        this->btnQuery->setEnabled(true);
        this->btnReset->setEnabled(true);
    }
    else
    {
        this->lineEditProduceNum->setReadOnly(true);
        this->comboboxProduceLineName->setEnabled(false);
        this->btnQuery->setEnabled(false);
        this->btnReset->setEnabled(false);
    }
}

void RightQueryWidget::slot_query()
{
//    GDataFactory::get_factory()->read_serial_number();
    if((this->comboboxProduceLineName->currentIndex() != -1) && (this->lineEditProduceNum->text() !=""))
        emit signal_add_sub_window(this->comboboxProduceLineName->currentText(),this->lineEditProduceNum->text());
    else
        ;
}

void RightQueryWidget::slot_reset()
{
    GDataFactory::get_mes_process()->exec_http_get("TG21CR230F");
    this->lineEditProduceNum->setText("");
    this->comboboxProduceLineName->setCurrentIndex(-1);
}

void RightQueryWidget::slot_comb_index_changed(int index)
{
    if(index == 0)
        GDataFactory::get_factory()->set_current_line_info(TCON_LINE);
    else if(index == 1)
        GDataFactory::get_factory()->set_current_line_info(ELEC_EQUIP_LINE);
    else
        GDataFactory::get_factory()->set_current_line_info(TV_LINE);
}
