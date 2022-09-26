#ifndef PLCWAKEUPDLG_H
#define PLCWAKEUPDLG_H
#include <QDialog>
#include <QThread>
class WakeUpThread : public QThread
{
    Q_OBJECT
public:
    WakeUpThread();
protected:
    void run();
};

class PlcWakeUpDlg : public QDialog
{
    Q_OBJECT
public:
    PlcWakeUpDlg();

    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    WakeUpThread* m_pWakeUpThread;
};

#endif // PLCWAKEUPDLG_H
