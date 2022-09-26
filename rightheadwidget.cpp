#include "rightheadwidget.h"
#include <QPainter>
#include <QDebug>
#include "gdatafactory.h"
RightHeadWidget::RightHeadWidget(QWidget *parent) : QWidget(parent)
{

}

void RightHeadWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));

    painter.save();
    QPixmap *imgHead = new QPixmap(":/icon/girl1.png");
    QRect eRect(static_cast<int>(this->width()*8.8/10),this->height()/6,this->height()*4/6,this->height()*4/6);
    qDebug()<<QString::number(this->height()*4/6);
    QBrush br(*imgHead);
    painter.setPen(QColor(0,0,0,0));
    painter.setBrush(br);
    painter.setBrushOrigin(static_cast<int>(this->width()*8.8/10),this->height()/6);//设置画刷规则，不设置，画刷默认为平铺绘制
    painter.drawEllipse(eRect);
    painter.restore();

    painter.setFont(QFont(u8"Microsoft YaHei",15,QFont::Normal,false));
    painter.setPen(QColor(0,0,0));
    QRect nameRect(static_cast<int>(this->width()*8.5/10)+this->height()/2,0,static_cast<int>(this->width()*1.5/10),this->height());
    QString loginUserName = GDataFactory::get_factory()->get_config_para("USER_NAME");
    painter.drawText(nameRect,Qt::AlignHCenter | Qt::AlignVCenter,loginUserName);
}
