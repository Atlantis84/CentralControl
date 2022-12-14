#include "plcwakeupdlg.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QtMath>
#include <QProcess>
#include "gdatafactory.h"
#include <QTextCodec>

static bool wake_up_sign = false;
PlcWakeUpDlg::PlcWakeUpDlg()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground,true);

    m_pWakeUpThread = new WakeUpThread();
    m_pWakeUpThread->start();

    this->resize(800,600);
    this->startTimer(50);
}

static int rotateValue = 0;
void PlcWakeUpDlg::timerEvent(QTimerEvent *event)
{
    if(wake_up_sign == true)
    {
        m_pWakeUpThread->terminate();
        m_pWakeUpThread->quit();
        this->accept();
    }
    else {
        this->repaint();
        rotateValue += 10;
    }
}

void PlcWakeUpDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.save();
    QPoint ptCenter(400,300);
    painter.translate(ptCenter.x(),ptCenter.y());
    painter.rotate(rotateValue);
    int R = 160;
    for(int i=0;i<18;i++)
    {
        double angle = (double)(i*20)*M_PI/180.0;
        QPainterPath pathE;
        pathE.addEllipse(0-R*qCos(angle)-25,R*qSin(angle)-25,50,50);
        painter.fillPath(pathE,QBrush(QColor(0,0,255,255-i*10)));
    }
    painter.restore();

    painter.setPen(QPen(QColor(0,0,255)));
    painter.setFont(QFont(u8"Lucida Calligraphy",40,QFont::Bold,false));
    painter.drawText(this->rect(),Qt::AlignHCenter|Qt::AlignVCenter,"PLC-WAKE-UP");
}

WakeUpThread::WakeUpThread()
{

}

void WakeUpThread::run()
{
    while (1) {
        QProcess pingProcess;
        QString strArg = "ping " + GDataFactory::get_factory()->get_config_para("PLC_IP") + " -n 1 -i 2";
        pingProcess.start(strArg,QIODevice::ReadOnly);
        pingProcess.waitForFinished(-1);

        QTextCodec* tc = QTextCodec::codecForName("GBK");
        QString p_stdout = tc->toUnicode(pingProcess.readAllStandardOutput());

        if(p_stdout.contains("TTL="))
        {
           wake_up_sign = true;
        }
        QThread::msleep(500);
    }
}
