#ifndef HISTORYDISPLAYWGT_H
#define HISTORYDISPLAYWGT_H

#include <QWidget>
class QTableWidget;
class HistoryDisplayWgt : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryDisplayWgt(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
    QWidget* createTableWidgetA();
    QWidget* createTableWidgetB();
    QTableWidget* tableWidgetA;
    QTableWidget* tableWidgetB;
public:
    void set_serial_number(QString boardsn);
    void set_boardtype_halfmaterialnumber(QString boardtype,QString halfmaterialnumber);
signals:

public slots:
    void slot_save();
};

#endif // HISTORYDISPLAYWGT_H
