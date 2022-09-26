#include "elecequiptestplanwgt_pd.h"
#include "TitleBar.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
ElecEquipTestPlanWgt::ElecEquipTestPlanWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget/*|Qt::WindowStaysOnTopHint*/);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(1000,500);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"平度电装换产计划");
    pTitleBar->setFixedWidth(1000);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();

    QLabel* labelPicture = new QLabel();
    labelPicture->setStyleSheet("background-color:rgba(0,0,0,255);");
//    QPixmap pixmap(":/icon/pddz.png");
//    pixmap.scaled(labelPicture->size()/2,Qt::IgnoreAspectRatio);
//    labelPicture->setScaledContents(true);
//    labelPicture->setAlignment(Qt::AlignCenter);
//    labelPicture->setPixmap(pixmap);
    QComboBox* combProductStyle = new QComboBox();

    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(labelPicture);
    this->setLayout(vAll);
}

void ElecEquipTestPlanWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}

void ElecEquipTestPlanWgt::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit signal_change_btn_icon();
}
