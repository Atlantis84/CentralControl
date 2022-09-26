#include "boardstylewidget.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QTreeWidgetItem>
#include <QDragMoveEvent>
#include <QDebug>
#include <QApplication>
#include <QMenu>
#include <QAction>
#include "gdatafactory.h"
#include "QsLog.h"
#include "imessagebox.h"
BoardStyleWidget::BoardStyleWidget(QWidget *parent,QString boardName) : QWidget(parent)
{
    currentBoardName = boardName;
    setAcceptDrops(true);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);

    QGroupBox* grpBox = new QGroupBox(boardName);
    grpBox->setStyleSheet("border:1px solid rgba(0,0,0,100);color:rgba(0,0,0,150);");

    QHBoxLayout* hBox1 = new QHBoxLayout();
    QLabel* labelSN = new QLabel(u8"SN:");
    labelSN->setStyleSheet("border:none;");
    lineEditSN = new QLineEdit();
    lineEditSN->setReadOnly(true);
    lineEditSN->setStyleSheet("background-color:rgb(240,242,245);font-family:Microsoft YaHei;font-size:20px;");
    hBox1->addWidget(labelSN);
    hBox1->addWidget(lineEditSN);
    hBox2 = new QHBoxLayout();
    grpContainer = new QGroupBox();
    grpContainer->setStyleSheet("background-color:rgba(0,0,0,50);");
    grpContainer->setLayout(hBox2);

    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vControls = new QVBoxLayout();
    vControls->addLayout(hBox1);
    vControls->addWidget(grpContainer);
    grpBox->setLayout(vControls);
    vAll->addWidget(grpBox);
    this->setLayout(vAll);

    pMenu = new QMenu();
    pMenu->setStyleSheet("QMenu::item{color:rgb(255,255,255);"
                         "font-family:Microsoft YaHei;"
                         "font-size:15px;"
                         "background-color:rgba(24,144,255,255);"
                         "}");
    pDeleteTask = new QAction(u8"删除对象",this);
    pMenu->addAction(pDeleteTask);
    connect(pDeleteTask,SIGNAL(triggered()),this,SLOT(slot_delete_label()));

//    load_pre_controls();
}

void BoardStyleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
    painter.setPen(QPen(QColor(0,0,0,100),2));
//    painter.drawRect(this->rect());
}

void BoardStyleWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasText() || event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        QByteArray data = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

RightLabel* BoardStyleWidget::createDragLabel(const QString &text, QWidget *parent)
{
    RightLabel *label=nullptr;
    bool foundSign = false;
    for (int i=0;i<hBox2->count();i++)
    {
        QLayoutItem* it = hBox2->layout()->itemAt(i);
        RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
        if(tmpWgt->text() == text)
            foundSign = true;
        else
            continue;
    }

    if(foundSign)
        return nullptr;
    else
    {
        label = new RightLabel(text, parent);
        label->setAutoFillBackground(true);
        label->setFrameShape(QFrame::Panel);
        label->setFrameShadow(QFrame::Raised);
        connect(label,SIGNAL(signal_right_click(QString)),this,SLOT(slot_on_label_click(QString)));
    }
    return label;
}

void BoardStyleWidget::slot_on_label_click(QString strText)
{
    for (int i=0;i<hBox2->count();i++)
    {
        QLayoutItem* it = hBox2->layout()->itemAt(i);
        RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
        if(tmpWgt->text() == strText)
        {
            strToDelete = strText;
            pMenu->exec(cursor().pos());
        }
    }
}

void BoardStyleWidget::slot_delete_label()
{
    for (int i=0;i<hBox2->count();i++)
    {
        QLayoutItem* it = hBox2->layout()->itemAt(i);
        RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
        if(tmpWgt->text() == strToDelete)
        {
            hBox2->removeWidget(tmpWgt);
            tmpWgt->setParent(nullptr);
        }
        else
            continue;
    }
}

static QString hotSpotMimeDataKey() { return QStringLiteral("application/x-qabstractitemmodeldatalist"); }

void BoardStyleWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasText()) {
        const QMimeData *mime = event->mimeData();
        QStringList pieces = mime->text().split(QRegularExpression(QStringLiteral("\\s+")),
                             QString::SkipEmptyParts);
//        QPoint position = event->pos();
        QPoint hotSpot;

        QByteArrayList hotSpotPos = mime->data(hotSpotMimeDataKey()).split(' ');
        if (hotSpotPos.size() == 2) {
            hotSpot.setX(hotSpotPos.first().toInt());
            hotSpot.setY(hotSpotPos.last().toInt());
        }

        for (const QString &piece : pieces) {
            if(hBox2->count()>2)//if controls > 3 do not add controls
                break;
            else
            {
                RightLabel *newLabel = createDragLabel(piece, grpContainer);
                if(newLabel)
                {
                    //newLabel->move(position - hotSpot);
                    newLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                    newLabel->show();
                    newLabel->setAttribute(Qt::WA_DeleteOnClose);
                    //position += QPoint(newLabel->width(), 0);
                    hBox2->addWidget(newLabel);
                }
//                for (int i=0;i<hBox2->count();i++)
//                {
//                    QLayoutItem* it = hBox2->layout()->itemAt(i);
//                    RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
//                    if(tmpWgt->text() == newLabel->text())
//                        continue;
//                    else
//                        hBox2->addWidget(newLabel);
//                }
            }
        }

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
    for (QWidget *widget : findChildren<QWidget *>()) {
        if (!widget->isVisible())
            widget->deleteLater();
    }
}

void BoardStyleWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    this->load_pre_controls();
}

void BoardStyleWidget::load_pre_controls()
{
    if(currentBoardName == u8"板A")
    {
//        this->lineEditSN->setText(GDataFactory::get_factory()->get_config_para("BOARD_A_SN"));
        this->lineEditSN->setText("");
        QStringList pieces;
        for (int i=1;i<4;i++)
        {
            QString strKey = QString("MACHINE_STATION_A%1").arg(i);
            if(GDataFactory::get_factory()->get_config_para(strKey) != "")
                pieces.append(GDataFactory::get_factory()->get_config_para(strKey));
        }

        for (const QString &piece : pieces)
        {
            RightLabel *newLabel = createDragLabel(piece, grpContainer);
            if(newLabel)
            {
                newLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                newLabel->show();
                newLabel->setAttribute(Qt::WA_DeleteOnClose);
                hBox2->addWidget(newLabel);
            }
        }
    }
    else
    {
//        this->lineEditSN->setText(GDataFactory::get_factory()->get_config_para("BOARD_B_SN"));
        this->lineEditSN->setText("");
        QStringList pieces;
        for (int i=1;i<4;i++)
        {
            QString strKey = QString("MACHINE_STATION_B%1").arg(i);
            if(GDataFactory::get_factory()->get_config_para(strKey) != "")
                pieces.append(GDataFactory::get_factory()->get_config_para(strKey));
        }

        for (const QString &piece : pieces)
        {

            RightLabel *newLabel = createDragLabel(piece, grpContainer);
            if(newLabel)
            {
                newLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                newLabel->show();
                newLabel->setAttribute(Qt::WA_DeleteOnClose);
                hBox2->addWidget(newLabel);
            }
        }
    }
}

void BoardStyleWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasText() || event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void BoardStyleWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
}

void BoardStyleWidget::set_serial_number(const QString serialNum)
{
    this->lineEditSN->setText(serialNum);

    //according to the product SN,get the full info from MES
    // http post operation
}

void BoardStyleWidget::save_plan_info()
{
    if(currentBoardName == u8"板A")
    {
        QList<QString> tmpInfo = GDataFactory::get_factory()->get_A_change_product_info();
        if(tmpInfo.length()<5)
        {
            QLOG_WARN()<<"board info is ABNORMAL!";
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"A板信息异常!");
            return;
        }

        if(hBox2->count() == 0)
            ;
        else
        {
            GDataFactory::get_factory()->set_config_para_1("BOARD_A_SN",lineEditSN->text());
            GDataFactory::get_factory()->set_config_para_1("PROJECT_ID_A",tmpInfo[0]);
            GDataFactory::get_factory()->set_config_para_1("HALF_MATERIAL_NUMBER_A",tmpInfo[1]);
            GDataFactory::get_factory()->set_config_para_1("MACHINE_STYLE_NAME_A",tmpInfo[2]);
            GDataFactory::get_factory()->set_config_para_1("BOARD_A_STYLE",tmpInfo[3]);
            GDataFactory::get_factory()->set_config_para_1("BOARD_SIZE_A",tmpInfo[4]);
        }

        //pre entry has to be cleared---lilei20211122
        for(int i=1;i<6;i++)
        {
            QString tmpStr = QString("MACHINE_STATION_A%1").arg(i);
            GDataFactory::get_factory()->set_config_para_1(tmpStr,"");
        }

        for (int i=0;i<hBox2->count();i++)
        {
            QLayoutItem* it = hBox2->layout()->itemAt(i);
            RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
            QString keyStr = QString("MACHINE_STATION_A%1").arg(i+1);

            GDataFactory::get_factory()->set_config_para_1(keyStr,tmpWgt->text());
            //for tcon change production
            GDataFactory::get_factory()->add_change_production_machine_name(tmpWgt->text());
            //for tcon change production

            //for vision or AOI change production
            QString tmpBoardStyle="";
            for (int i=0;i<5;i++)
            {
                tmpBoardStyle.append(tmpInfo[3][i]);
            }
            GDataFactory::get_factory()->add_change_production_info_for_PLC_or_AOI(tmpWgt->text(),tmpBoardStyle,tmpInfo[1]);
            //for vision or AOI change production

            //save changing product info to db
            QList<QString> lstChangeInfo;
            lstChangeInfo.push_back(tmpInfo[3]);
            lstChangeInfo.push_back(tmpInfo[2]);
            lstChangeInfo.push_back(tmpInfo[1]);
            lstChangeInfo.push_back(tmpWgt->text());
            lstChangeInfo.push_back("1");
            lstChangeInfo.push_back(lineEditSN->text());
            lstChangeInfo.push_back(GDataFactory::get_factory()->get_line_name_by_machine_name(tmpWgt->text()));
            lstChangeInfo.push_back(tmpInfo[0]);
            lstChangeInfo.push_back(tmpInfo[4]);
            GDataFactory::get_factory()->save_AB_change_product_info(lstChangeInfo);
        }
    }
    else
    {
        QList<QString> tmpInfo = GDataFactory::get_factory()->get_B_change_product_info();
        if(tmpInfo.length()<5)
        {
            QLOG_WARN()<<"board info is ABNORMAL!";
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"B板信息异常!");
            return;
        }

        if(hBox2->count() == 0)
            ;
        else
        {
            GDataFactory::get_factory()->set_config_para_1("BOARD_B_SN",lineEditSN->text());
            GDataFactory::get_factory()->set_config_para_1("PROJECT_ID_B",tmpInfo[0]);
            GDataFactory::get_factory()->set_config_para_1("HALF_MATERIAL_NUMBER_B",tmpInfo[1]);
            GDataFactory::get_factory()->set_config_para_1("MACHINE_STYLE_NAME_B",tmpInfo[2]);
            GDataFactory::get_factory()->set_config_para_1("BOARD_B_STYLE",tmpInfo[3]);
            GDataFactory::get_factory()->set_config_para_1("BOARD_SIZE_B",tmpInfo[4]);
        }
        //repeated entry has to be cleared---lilei20211122
        for(int i=1;i<6;i++)
        {
            QString tmpStr = QString("MACHINE_STATION_B%1").arg(i);
            GDataFactory::get_factory()->set_config_para_1(tmpStr,"");
        }

        for (int i=0;i<hBox2->count();i++)
        {
            QLayoutItem* it = hBox2->layout()->itemAt(i);
            RightLabel* tmpWgt = qobject_cast<RightLabel*>(it->widget());
            QString keyStr = QString("MACHINE_STATION_B%1").arg(i+1);
            GDataFactory::get_factory()->set_config_para_1(keyStr,tmpWgt->text());
            //for tcon change produciton
            GDataFactory::get_factory()->add_change_production_machine_name(tmpWgt->text());
            //for tcon change produciton
            //for vision or AOI change production
            QString tmpBoardStyle="";
            for (int i=0;i<5;i++)
            {
                tmpBoardStyle.append(tmpInfo[3][i]);
            }
            GDataFactory::get_factory()->add_change_production_info_for_PLC_or_AOI(tmpWgt->text(),tmpBoardStyle,tmpInfo[1]);
            //for vision or AOI change production

            //save changing product info to db
            QList<QString> lstChangeInfo;
            lstChangeInfo.push_back(tmpInfo[3]);
            lstChangeInfo.push_back(tmpInfo[2]);
            lstChangeInfo.push_back(tmpInfo[1]);
            lstChangeInfo.push_back(tmpWgt->text());
            lstChangeInfo.push_back("1");
            lstChangeInfo.push_back(lineEditSN->text());
            lstChangeInfo.push_back(GDataFactory::get_factory()->get_line_name_by_machine_name(tmpWgt->text()));
            lstChangeInfo.push_back(tmpInfo[0]);
            lstChangeInfo.push_back(tmpInfo[4]);
            GDataFactory::get_factory()->save_AB_change_product_info(lstChangeInfo);
        }
    }
    GDataFactory::get_factory()->save_config_file();
}
