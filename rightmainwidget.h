#ifndef RIGHTMAINWIDGET_H
#define RIGHTMAINWIDGET_H

#include <QWidget>
#include <QMdiArea>
class RightMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightMainWidget(QWidget *parent = nullptr);
private:
    QMdiArea* subMdiArea;
    void paintEvent(QPaintEvent *event);
public:
signals:

public slots:
    void slot_add_sub_window(const QString lineNum,const QString lineName);
};

#endif // RIGHTMAINWIDGET_H
