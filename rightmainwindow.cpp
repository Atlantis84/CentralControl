#include "rightmainwindow.h"
#include <QPainter>
#include <QVBoxLayout>
RightMainWindow::RightMainWindow(QWidget *parent) : QMainWindow(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    subMdiArea = new QMdiArea();
    vAll->addWidget(subMdiArea);
    this->setLayout(vAll);
}

void RightMainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
}
