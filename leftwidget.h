#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QWidget>
class QTreeWidget;
class QTreeWidgetItem;
class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
public:

signals:
    void signal_send_path(const QString strPath);

public slots:
    void slot_getPath(QTreeWidgetItem* wItem,int count);
};

#endif // LEFTWIDGET_H
