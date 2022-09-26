#include "qslogwidget.h"
#include "TitleBar.h"
#include <QVBoxLayout>
#include <QPainter>
QsLogWidget::QsLogWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget|Qt::WindowStaysOnTopHint);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(1000,500);
    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"实时日志");
    pTitleBar->setFixedWidth(1000);
    installEventFilter(pTitleBar);

    m_pLogEdit = new QTextEdit();
    m_pLogEdit->setReadOnly(true);
    m_pLogEdit->setStyleSheet("color:rgb(0,0,0);background-color:rgb(0,0,0);font: 10pt 'LiSu'");

    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(m_pLogEdit);
    this->setLayout(vAll);
}

void QsLogWidget::slot_rev_logs(const QString msg)
{
    this->m_pLogEdit->append(msg);
}

void QsLogWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
    painter.drawRect(this->rect());
}
