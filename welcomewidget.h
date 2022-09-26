#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QDialog>
class QLineEdit;
class QCheckBox;
class WelcomeWidget : public QDialog
{
    Q_OBJECT
public:
    explicit WelcomeWidget();

private:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    void timerEvent(QTimerEvent *event);
    int TitleHeight;
    int m_TimerID;
    bool bPingSuccess;
    QLineEdit* passwordLineEdit;
    QLineEdit* userNameLineEdit;
    QCheckBox* ngCheckBox;
public:

signals:

public slots:
    void slot_password_editEdited(const QString& strText);
    void slot_user_login();
    void slot_logout();
};

#endif // WELCOMEWIDGET_H
