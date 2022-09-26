#ifndef BOARDSTYLEWIDGET_H
#define BOARDSTYLEWIDGET_H

#include <QWidget>
#include "rightlabel.h"
#include <QMenu>
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class BoardStyleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardStyleWidget(QWidget *parent = nullptr,QString boardName="");
private:
    void paintEvent(QPaintEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void showEvent(QShowEvent* event);
    void load_pre_controls();//load the controls last time
    RightLabel* createDragLabel(const QString &text, QWidget *parent);
    QGroupBox* grpContainer;
    QHBoxLayout* hBox2;
    QMenu *pMenu;
    QAction *pDeleteTask;
    QString strToDelete;
    void timerEvent(QTimerEvent *event);
    QLineEdit* lineEditSN;
    QString currentBoardName;

public:
    void set_serial_number(const QString serialNum);
    void save_plan_info();

signals:

public slots:
    void slot_on_label_click(QString strText);
    void slot_delete_label();
};

#endif // BOARDSTYLEWIDGET_H
