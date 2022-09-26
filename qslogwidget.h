#ifndef QSLOGWIDGET_H
#define QSLOGWIDGET_H

#include <QWidget>
#include <QTextEdit>
class QsLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QsLogWidget(QWidget *parent = nullptr);

    QTextEdit* m_pLogEdit;
private:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void slot_rev_logs(const QString msg);
};

#endif // QSLOGWIDGET_H
