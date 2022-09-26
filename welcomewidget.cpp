#include "welcomewidget.h"
#include <QIcon>
#include "TitleBar.h"
#include <QPainter>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QCheckBox>
#include <QSqlQuery>
#include "gdatafactory.h"
#include "QsLog.h"
#include "imessagebox.h"
#include <QProcess>
#include <QTextCodec>
WelcomeWidget::WelcomeWidget()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    resize(1600,1000);
//    TitleBar *pTitleBar = new TitleBar(this);
//    pTitleBar->setTitle(u8"     登录");
//    pTitleBar->setFixedWidth(1000);
//    installEventFilter(pTitleBar);
//    TitleHeight = pTitleBar->height();

    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QHBoxLayout* hBox2 = new QHBoxLayout();
    QHBoxLayout* hBox3 = new QHBoxLayout();
    QHBoxLayout* hBox4 = new QHBoxLayout();

    QLabel* userNameLabel = new QLabel(u8"用户名:");
    userNameLabel->setStyleSheet("QLabel{color:rgba(255,255,255,150);border:1px;\
                            border-radius:8px;\
                            background-color:rgba(24,33,38,0);font-family:\"LiSu\";\
                            font-size: 30px;font-weight:bold;}");
    userNameLineEdit = new QLineEdit();
    userNameLineEdit->setStyleSheet("QLineEdit{color:rgba(0,0,0,100);border:1px solid rgba(0,0,0,100);\
                                    border-radius:5px;\
                                    background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
                                    font-size: 20px;font-weight:normal;\
                                    min-width:400px;min-height:40px;}");
    QAction *nameAction = new QAction(userNameLineEdit);
    nameAction->setIcon(QIcon(":/icon/username.png"));
    userNameLineEdit->addAction(nameAction,QLineEdit::LeadingPosition);
    userNameLineEdit->setAlignment(Qt::AlignLeft);
    userNameLineEdit->setText(u8"用户名");
    hBox1->addStretch(1);
//    hBox1->addWidget(userNameLabel);
    hBox1->addWidget(userNameLineEdit);
    hBox1->addStretch(1);

    QLabel* passwordLabel = new QLabel(u8" 密码: ");
    passwordLabel->setStyleSheet("QLabel{color:rgba(255,255,255,255);border:1px;\
                            border-radius:8px;\
                            background-color:rgba(24,33,38,0);font-family:\"LiSu\";\
                            font-size: 30px;font-weight:bold;}");
    passwordLineEdit = new QLineEdit();
    connect(passwordLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(slot_password_editEdited(const QString&)));
    passwordLineEdit->setStyleSheet("QLineEdit{color:rgba(0,0,0,100);border:1px solid rgba(0,0,0,100);\
                            border-radius:5px;\
                            background-color:rgba(255,255,255,255);font-family:\"Microsoft YaHei\";\
                            font-size: 20px;font-weight:normal;\
                            min-width:400px;min-height:40px;}");
    QAction *passwordAction = new QAction(passwordLineEdit);
    passwordAction->setIcon(QIcon(":/icon/password.png"));
    passwordLineEdit->addAction(passwordAction,QLineEdit::LeadingPosition);
    passwordLineEdit->setAlignment(Qt::AlignLeft);
    passwordLineEdit->setText(u8"密码");
    hBox2->addStretch(1);
//    hBox2->addWidget(passwordLabel);
    hBox2->addWidget(passwordLineEdit);
    hBox2->addStretch(1);

    ngCheckBox = new QCheckBox(u8"自动登录");
    QString autoLogin = GDataFactory::get_factory()->get_config_para("AUTO_LOGIN_SIGN");
    QString userName = GDataFactory::get_factory()->get_config_para("USER_NAME");
    QString userPassword = GDataFactory::get_factory()->get_config_para("PASSWORD");
    if(autoLogin == "TRUE")
    {
        ngCheckBox->setCheckState(Qt::CheckState::Checked);
        userNameLineEdit->setText(userName);
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        passwordLineEdit->setText(GDataFactory::get_factory()->DecodeStr(userPassword));
    }
    else
    {
        userNameLineEdit->setText(u8"请输入用户名");
        passwordLineEdit->setText(u8"请输入密码");
        ngCheckBox->setCheckState(Qt::CheckState::Unchecked);
    }
    ngCheckBox->setStyleSheet("QCheckBox{color:rgba(0,0,0,100);font-family:'Microsoft YaHei';font-size: 20px;font-weight:normal;}");
    hBox3->addStretch(47);
    hBox3->addWidget(ngCheckBox);
    hBox3->addStretch(72);

    QPushButton* btnOK = new QPushButton(u8"     登录     ");
    connect(btnOK,SIGNAL(clicked()),this,SLOT(slot_user_login()));
    btnOK->setStyleSheet("QPushButton{background-color:rgba(24,144,255,255);border-radius:5px;\
                            min-width:400px;max-width:500px;min-height:35px;font-family:'Microsoft YaHei';}");
//    btnOK->setStyleSheet("QPushButton{background-color:rgba(24,144,255,255);border-top-left-radius:30px;border-bottom-right-radius:30px;\
//                            min-width:400px;max-width:500px;min-height:40px;font-family:'Microsoft YaHei';}");
    QPushButton* btnCancel = new QPushButton(u8"   取 消   ");
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(slot_logout()));
    btnCancel->setStyleSheet("QPushButton{background-color:rgba(24,33,38,0);min-height:40px;}");
    hBox4->addStretch(1);
    hBox4->addWidget(btnOK);
//    hBox4->addStretch(1);
//    hBox4->addWidget(btnCancel);
    hBox4->addStretch(1);

    vAll->addStretch(18);
    vAll->addLayout(hBox1);
    vAll->addStretch(1);
    vAll->addLayout(hBox2);
    vAll->addStretch(1);
    vAll->addLayout(hBox3);
    vAll->addStretch(1);
    vAll->addLayout(hBox4);
    vAll->addStretch(8);
    this->setLayout(vAll);
}

void WelcomeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPixmap *img = new QPixmap(":/icon/2.png");
    QRect iconRt(this->width()/2 - 150,100,300,84);
    painter.fillRect(this->rect(),QBrush(QColor(241,242,246)));
    painter.drawPixmap(iconRt,*img);

    painter.setPen(QPen(QColor(1,158,161),1));
    QRect textRt(0,iconRt.bottom()+50,this->width(),100);
    QRect textRt1(0,textRt.bottom()+5,this->width(),40);
    QRect textRt2(0,this->rect().bottom()-150,this->width(),30);
    QString strToDisplay = u8"智动精工中控系统";
    QString strToDisplay1 = "Central Control System";
    QString strToDisplay2 = u8"copyright@2021 智动精工智能制造部出品";
    painter.setFont(QFont(u8"Microsoft YaHei",40,QFont::Normal,false));
    painter.drawText(textRt,Qt::AlignHCenter|Qt::AlignVCenter,strToDisplay);
    painter.setFont(QFont(u8"Microsoft YaHei",15,QFont::Normal,false));
    painter.drawText(textRt1,Qt::AlignHCenter|Qt::AlignVCenter,strToDisplay1);
    painter.setFont(QFont(u8"Microsoft YaHei",10,QFont::Normal,false));
    painter.setPen(QColor(0,0,0,100));
    painter.drawText(textRt2,Qt::AlignHCenter|Qt::AlignVCenter,strToDisplay2);
}

void WelcomeWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    exit(1);
}

void WelcomeWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_TimerID)
    {
        QProcess pingProcess;
        QString strArg = "ping " + GDataFactory::get_factory()->get_config_para("PLC_IP") + " -n 1 -i 2";
        pingProcess.start(strArg,QIODevice::ReadOnly);
        pingProcess.waitForFinished(-1);


        QTextCodec* tc = QTextCodec::codecForName("GBK");
        QString p_stdout = tc->toUnicode(pingProcess.readAllStandardOutput());
//        QLOG_WARN()<<p_stdout;

        if(p_stdout.contains("TTL="))
        {
            bPingSuccess = true;
            killTimer(m_TimerID);
        }else
        {
            bPingSuccess = false;
        }
    }
}

void WelcomeWidget::slot_password_editEdited(const QString &strText)
{
    Q_UNUSED(strText);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
}

void WelcomeWidget::slot_user_login()
{
//    if(!bPingSuccess)
//    {
//        IMessageBox* msgBox = new IMessageBox(3);
//        msgBox->error(u8"PLC连接异常!");
//        return;
//    }

    QString strSql = QString("select * from public.%1").arg(constUserInfoTable);
    QSqlQuery queryResult;
    bool loginSign = false;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        QString uName = this->userNameLineEdit->text();
        QString uPassword = GDataFactory::get_factory()->EncodeStr(this->passwordLineEdit->text());
        while(queryResult.next())
        {
            if((uName == queryResult.value(1).toString())&&( uPassword== queryResult.value(2).toString()))
            {
                loginSign = true;
                if(ngCheckBox->isChecked())
                {
                    GDataFactory::get_factory()->set_config_para_1("AUTO_LOGIN_SIGN","TRUE");
                    GDataFactory::get_factory()->set_config_para_1("USER_NAME",uName);
                    GDataFactory::get_factory()->set_config_para_1("PASSWORD",uPassword);
                    GDataFactory::get_factory()->save_config_file();
                }
                else
                {
                    GDataFactory::get_factory()->set_config_para_1("AUTO_LOGIN_SIGN","FALSE");
                    GDataFactory::get_factory()->set_config_para_1("USER_NAME",uName);
                    GDataFactory::get_factory()->set_config_para_1("PASSWORD",uPassword);
                    GDataFactory::get_factory()->save_config_file();
                }
                break;
            }
            else
                continue;
        }
    }

    if(loginSign == true)
    {
//        QString tmpLog = this->userNameLineEdit->text() + " login success!";
//        QLOG_TRACE()<<tmpLog;
        this->accept();
    }
    else
    {
        IMessageBox* msgBox = new IMessageBox(3);
        msgBox->warning(u8"用户名或密码错误,请重新输入!");
    }
}

void WelcomeWidget::slot_logout()
{
    this->close();
}
