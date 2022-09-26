#ifndef TESTPLANWGT_H
#define TESTPLANWGT_H

#include <QWidget>
#include "dragtreewidget.h"
#include <QMutex>
#include <QUdpSocket>
class QTreeWidget;
class TestPlanWgt : public QWidget
{
    Q_OBJECT
public:
    explicit TestPlanWgt(QWidget *parent = nullptr);
private:
    QUdpSocket *m_pUdpSocket;
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
    void timerEvent(QTimerEvent *event);
    int m_time_out;
    int m_timer_ID;
    DragTreeWidget *treeWidget;
    void send_message_to_plc(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array);
    void send_message_to_vision_lead(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array);
    QByteArray make_plc_message(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array);
public:

signals:
    void signal_change_btn_icon();
    void signal_save_plan();
public slots:
    void slot_rev_plc_msg();
    void slot_save_test_plan();
    void slot_kill_timeout_timer();
    void slot_refresh_treewgt();//when receive the register success info,refresh the tree widget to add the new station
};

#endif // TESTPLANWGT_H
