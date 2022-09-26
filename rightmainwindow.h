#ifndef RIGHTMAINWINDOW_H
#define RIGHTMAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QWebEngineView>
#include <QtWebEngine/qtwebengineglobal.h>
class RightMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit RightMainWindow(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
    QMdiArea* subMdiArea;
public:

signals:

public slots:
};

#endif // RIGHTMAINWINDOW_H
