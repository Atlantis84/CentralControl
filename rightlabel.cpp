#include "rightlabel.h"
#include <QMouseEvent>
RightLabel::RightLabel(const QString &text, QWidget *parent)
{
    this->setText(text);
    this->setParent(parent);
}

void RightLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if(Qt::RightButton == e->button())
        emit signal_right_click(this->text());
}
