#include "rightmainwidget.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QDebug>
#include "onebuttonchangewidget.h"
#include <QMdiSubWindow>
RightMainWidget::RightMainWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    subMdiArea = new QMdiArea(this);
    subMdiArea->setBackground(Qt::NoBrush);
    subMdiArea->setStyleSheet("QMdiArea{background-color:rgba(0,0,0,100);}");
    vAll->addWidget(subMdiArea);
    vAll->setSpacing(0);
    vAll->setMargin(0);
    this->setLayout(vAll);
}

void RightMainWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
}

void RightMainWidget::slot_add_sub_window(const QString lineNum,const QString lineName)
{
    OneButtonChangeWidget* newWgt = new OneButtonChangeWidget(this,lineNum,lineName);
    subMdiArea->addSubWindow(newWgt);
    foreach(QMdiSubWindow* s, subMdiArea->subWindowList())
        s->resize(800,400);
    newWgt->showMaximized();
}
