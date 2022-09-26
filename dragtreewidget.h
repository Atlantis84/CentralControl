#ifndef DRAGTREEWIDGET_H
#define DRAGTREEWIDGET_H

#include <QTreeWidget>
#include <QLabel>
class QtGuiDrag:public QWidget{
    Q_OBJECT
public:
    QtGuiDrag(QWidget* parent = nullptr);
    ~QtGuiDrag();
    void setShowText(QString str);
private:
    QString strToDisplay;
    QLabel* labelToDisplay;
    void paintEvent(QPaintEvent *event);
};
class DragTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    DragTreeWidget();
private:
    void mousePressEvent(QMouseEvent *event);
//    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void startDrag(Qt::DropActions supportedActions);
//    void mouseMoveEvent(QMouseEvent *event);
    QPoint startPos;
//    void performDrag();
public:
};

#endif // DRAGTREEWIDGET_H
