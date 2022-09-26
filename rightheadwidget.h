#ifndef RIGHTHEADWIDGET_H
#define RIGHTHEADWIDGET_H

#include <QWidget>

class RightHeadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightHeadWidget(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
public:

signals:

public slots:
};

#endif // RIGHTHEADWIDGET_H
