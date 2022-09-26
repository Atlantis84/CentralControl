#ifndef ONEBUTTONCHANGEWIDGET_H
#define ONEBUTTONCHANGEWIDGET_H

#include <QWidget>
#include "circlewidget.h"
class QLabel;
class OneButtonChangeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OneButtonChangeWidget(QWidget *parent = nullptr,const QString lineNum="",const QString lineName="");
private:
    void paintEvent(QPaintEvent *event);
    QLabel* labelLineName;
    QLabel* labelLineNum;
public:

signals:

public slots:
};

#endif // ONEBUTTONCHANGEWIDGET_H
