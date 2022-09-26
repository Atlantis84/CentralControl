#include "dragtreewidget.h"
#include <QMimeData>
#include <QDrag>
#include <QDragMoveEvent>
#include <QApplication>
#include <QDebug>
#include <QVariant>
#include <QWindow>
#include <QPainter>
DragTreeWidget::DragTreeWidget()
{
    setMouseTracking(true);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setDragEnabled(true);
    this->setAcceptDrops(false);
    this->setDefaultDropAction(Qt::MoveAction);
    this->setDragDropMode(QAbstractItemView::DragOnly);
}

void DragTreeWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() & Qt::LeftButton)
    {
        startPos = event->pos();
    }
    QTreeWidget::mousePressEvent(event);
}

//void DragTreeWidget::dragEnterEvent(QDragEnterEvent *event)
//{
//    QWidget* source = qobject_cast<DragTreeWidget*>(event->source());
//    if(source)
//    {
//        event->setDropAction(Qt::MoveAction);
//        event->accept();
//    }
//}

void DragTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void DragTreeWidget::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);

    QTreeWidgetItem* item = currentItem();
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream<<item->text(0);//理论上讲该句有了就可以不加下面setText(item->text(0))，但是测试未通过，不清楚原因

    QMimeData* mimeData = new QMimeData;
    mimeData->setText(item->text(0));
    mimeData->setData("application/x-qabstractitemmodeldatalist",itemData);

    QSize pSize(this->visualItemRect(item).width(),this->visualItemRect(item).height());
    QPixmap pixmap;
    QtGuiDrag* dragP = new QtGuiDrag();
    dragP->resize(pSize);
    dragP->setShowText(item->text(0));
    pixmap = dragP->grab();

    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(pixmap.width()/2,pixmap.height()/2));
    drag->exec(Qt::MoveAction);
}

QtGuiDrag::QtGuiDrag(QWidget *parent)
{
    Q_UNUSED(parent);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setAttribute(Qt::WA_TranslucentBackground,true);
    strToDisplay = u8"未设置";
}

QtGuiDrag::~QtGuiDrag()
{

}

void QtGuiDrag::setShowText(QString str)
{
    strToDisplay = str;
}

void QtGuiDrag::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(0,135,135)));
    painter.setPen(QPen(QColor(255,255,255)));
    painter.setFont(QFont(u8"KaiTi",10,QFont::Bold,false));
    painter.drawText(this->rect(),Qt::AlignHCenter|Qt::AlignVCenter,strToDisplay);
}
