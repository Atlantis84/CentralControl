#ifndef RIGHTQUERYWIDGET_H
#define RIGHTQUERYWIDGET_H

#include <QWidget>
class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;
class RightQueryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightQueryWidget(QWidget *parent = nullptr);
    void timerEvent(QTimerEvent *event);
private:
    void paintEvent(QPaintEvent *event);
    QLabel* labelPath;
    QComboBox* comboboxProduceLineName;
    QLineEdit* lineEditProduceNum;
    QPushButton* btnQuery;
    QPushButton* btnReset;
public:

signals:
    void signal_add_sub_window(const QString lineName,const QString lineNum);
public slots:
    void slot_rev_path(const QString strPath);
    void slot_query();
    void slot_reset();
    void slot_comb_index_changed(int index);
};

#endif // RIGHTQUERYWIDGET_H
