#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QMutex>
class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
public:
    void send_message_to_plc(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array);
    QByteArray make_plc_message(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array);
private:
    QUdpSocket *m_pUdpSocket;
    QMutex m_mutex;
    int m_pTimerID;
    int m_pMonitorID;

    bool get_board_type_and_half_number(QString &bdType,QString &halfNum,QString productsn);

signals:
    void signal_proc_plc_msg(const QByteArray data_array);
    void signal_proc_final_data(const QByteArray data_array);
public slots:
    void slot_receive_message();
    void slot_proc_plc_msg(const QByteArray data_array);
    void slot_proc_final_data(const QByteArray data_array);
};

#endif // CENTRALWIDGET_H
