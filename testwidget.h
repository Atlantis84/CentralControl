#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
class CircleWidget;
class QLabel;
class QVBoxLayout;
class QPushButton;
class TestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TestWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
    CircleWidget* circleWgt1;
    CircleWidget* circleWgt2;
    CircleWidget* circleWgt3;
    CircleWidget* circleWgt4;
    QVBoxLayout* vTop;
    QLabel* labelCurrentLine;
    QLabel* labelCurrentNum;
    QPushButton* btnTest;
    QString currentState;
public:

signals:
    void signal_kill_timeout_timer();

public slots:
    void slot_rev_query_result(QString strCurrentLine,QString strCurrentNum);
    void slot_start_change_production();
    void slot_change_btn_icon();
    void slot_save_plan();
    void slot_change_production_already_complete();
};

#endif // TESTWIDGET_H
