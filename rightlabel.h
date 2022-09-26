#ifndef RIGHTLABEL_H
#define RIGHTLABEL_H

#include <QLabel>
class RightLabel : public QLabel
{
    Q_OBJECT
public:
    RightLabel(const QString &text, QWidget *parent = nullptr);
private:
    void mouseReleaseEvent(QMouseEvent *e);
signals:
    void signal_right_click(QString selfText);
};

#endif // RIGHTLABEL_H
