#include "mainwindow.h"
#include <QApplication>
#include "gdatafactory.h"
#include <QDir>
#include "QsLog.h"
#include "QsLogDest.h"
#include <QFile>
#include <QTextCodec>
using namespace QsLogging;
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString qssFile = ":/icon/darkgray.qss";
    QFile file(qssFile);

    Logger& logger = Logger::instance();

    QString ttl = QDateTime::currentDateTime().toString("yyyy-MM-dd HH-mm-ss");
    QString tmpStr = "\Log\\log.txt";
    const QString sLogPath(QDir(QApplication::applicationDirPath()).filePath(tmpStr));
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(sLogPath, EnableLogRotation, MaxSizeBytes(256*1024), MaxOldLogCount(5000)));
    logger.addDestination(fileDestination);

    DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
    logger.addDestination(debugDestination);

    DestinationPtr controlDestination(DestinationFactory::MakeFunctorDestination(GDataFactory::get_logs_widget(),SLOT(slot_rev_logs(const QString))));
    logger.addDestination(controlDestination);

    QLOG_INFO()<<u8"QsLog init SUCCESS";
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }

//    GDataFactory::get_test_plan_thread()->start();
//    GDataFactory::get_test_db_thread()->start();

    if(GDataFactory::get_pgsql()->ConnectDataBase())
        QLOG_INFO()<<"connect to db success!";
    else {
        QLOG_FATAL()<<"connect to db failed!";
        QApplication::exit(1);
    }
    GDataFactory::get_factory()->connect_common_signal_slot();

    QString binDir=QApplication::applicationDirPath();
    QString fileName("config.json");

    QStringList searchList;
    searchList.append(binDir);

    foreach (QString dir, searchList)
    {
        QFile file(dir+"/"+fileName);
        if (file.exists())
        {
            fileName=QDir(file.fileName()).canonicalPath();
            QLOG_INFO()<<"Using json config file"<<fileName;
            GDataFactory::get_factory()->load_json_config_1(QByteArray(fileName.toLatin1()).data());
        }
        else
        {
            QLOG_WARN()<<"Json config file not exist"<<fileName;
        }
    }

    QString logLevel = GDataFactory::get_factory()->get_config_para("LOG_LEVEL");
    if(logLevel == "0")
        logger.setLoggingLevel(QsLogging::TraceLevel);
    else if(logLevel == "1")
        logger.setLoggingLevel(QsLogging::DebugLevel);
    else if(logLevel == "2")
        logger.setLoggingLevel(QsLogging::InfoLevel);
    else if(logLevel == "3")
        logger.setLoggingLevel(QsLogging::WarnLevel);
    else if(logLevel == "4")
        logger.setLoggingLevel(QsLogging::ErrorLevel);
    else if(logLevel == "5")
        logger.setLoggingLevel(QsLogging::FatalLevel);
    else
        logger.setLoggingLevel(QsLogging::OffLevel);

    if(GDataFactory::get_factory()->get_config_para("PLC_WAKE_UP") == "1")
    {
        if(GDataFactory::get_plc_wake_up_dlg()->exec() == QDialog::Accepted)
            ;
        else
        {
            QApplication::exit(1);
            return 0;
        }
    }

    if(GDataFactory::get_welcome_wgt()->exec() == QDialog::Accepted )
    {
        GDataFactory::get_factory()->init_http_service();
        GDataFactory::get_main_window()->show();
    }
    else
        QApplication::exit(1);
    return a.exec();
}
