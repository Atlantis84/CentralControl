#ifndef ELECEQUIPTESTPLANWGT_H
#define ELECEQUIPTESTPLANWGT_H

#include <QWidget>

class ElecEquipTestPlanWgt : public QWidget
{
    Q_OBJECT
public:
    explicit ElecEquipTestPlanWgt(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

signals:
    void signal_change_btn_icon();
public slots:
};

#endif // ELECEQUIPTESTPLANWGT_H
