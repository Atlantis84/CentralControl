#-------------------------------------------------
#
# Project created by QtCreator 2021-10-25T18:26:45
#
#-------------------------------------------------

QT += webenginewidgets core gui sql network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CentralControl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO

SOURCES += \
        boardstylewidget.cpp \
        centralwidget.cpp \
        circlewidget.cpp \
        control.cpp \
        datacommserial.cpp \
        dragtreewidget.cpp \
        elecequiptestplanwgt_pd.cpp \
        gdatafactory.cpp \
        historydisplaywgt.cpp \
        httpaoimsg.cpp \
        httpcpps/httpconnectionhandler.cpp \
        httpcpps/httpconnectionhandlerpool.cpp \
        httpcpps/httpcookie.cpp \
        httpcpps/httpglobal.cpp \
        httpcpps/httplistener.cpp \
        httpcpps/httprequest.cpp \
        httpcpps/httprequesthandler.cpp \
        httpcpps/httpresponse.cpp \
        httpcpps/httpsession.cpp \
        httpcpps/httpsessionstore.cpp \
        httpcpps/staticfilecontroller.cpp \
        httpcpps/template.cpp \
        httpcpps/templatecache.cpp \
        httpcpps/templateloader.cpp \
        httpmsgprocess.cpp \
        httpregistermsg.cpp \
        httptconmsg.cpp \
        httptvmsg.cpp \
        httpvisionmsg.cpp \
        imessagebox.cpp \
        leftwidget.cpp \
        main.cpp \
        mainwindow.cpp \
        md5.cpp \
        mesprocess.cpp \
        onebuttonchangewidget.cpp \
        plcwakeupdlg.cpp \
        processor.cpp \
        processor.modify.cpp \
        processor.query.cpp \
        qslogwidget.cpp \
        rightheadwidget.cpp \
        rightlabel.cpp \
        rightmainwidget.cpp \
        rightmainwindow.cpp \
        rightquerywidget.cpp \
        stationmonitorwgt_pd.cpp \
        stationtabwgt_pd.cpp \
        testdbthread.cpp \
        testplanthread.cpp \
        testplanwgt.cpp \
        testwidget.cpp \
        title/TitleBar.cpp \
        title/titlewidget.cpp \
        tvplanwgt.cpp \
        usertabwgt_pd.cpp \
        welcomewidget.cpp

HEADERS += \
        GlobDef.h \
        boardstylewidget.h \
        centralwidget.h \
        circlewidget.h \
        datacommserial.h \
        dragtreewidget.h \
        elecequiptestplanwgt_pd.h \
        gdatafactory.h \
        historydisplaywgt.h \
        httpaoimsg.h \
        httpheaders/httpconnectionhandler.h \
        httpheaders/httpconnectionhandlerpool.h \
        httpheaders/httpcookie.h \
        httpheaders/httpglobal.h \
        httpheaders/httplistener.h \
        httpheaders/httprequest.h \
        httpheaders/httprequesthandler.h \
        httpheaders/httpresponse.h \
        httpheaders/httpsession.h \
        httpheaders/httpsessionstore.h \
        httpheaders/staticfilecontroller.h \
        httpheaders/template.h \
        httpheaders/templatecache.h \
        httpheaders/templateglobal.h \
        httpheaders/templateloader.h \
        httpmsgprocess.h \
        httpregistermsg.h \
        httptconmsg.h \
        httptvmsg.h \
        httpvisionmsg.h \
        imessagebox.h \
        leftwidget.h \
        mainwindow.h \
        md5.h \
        mesprocess.h \
        onebuttonchangewidget.h \
        plcwakeupdlg.h \
        procact.h \
        processor.h \
        qslogwidget.h \
        rightheadwidget.h \
        rightlabel.h \
        rightmainwidget.h \
        rightmainwindow.h \
        rightquerywidget.h \
        stationmonitorwgt_pd.h \
        stationtabwgt_pd.h \
        testdbthread.h \
        testplanthread.h \
        testplanwgt.h \
        testwidget.h \
        title/TitleBar.h \
        title/titlewidget.h \
        tvplanwgt.h \
        usertabwgt_pd.h \
        welcomewidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += .\title
INCLUDEPATH += .\httpheaders
INCLUDEPATH += .\include\curl-7.80.0
INCLUDEPATH += .\include
INCLUDEPATH += .\include\jsoncpp\json

DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
DEFINES += QS_LOG_LINE_NUMBERS
DEFINES += HTTP_ONLY
DEFINES += CURL_STATICLIB
LIBS += -lws2_32 -lwldap32 -lCrypt32 -ladvapi32 -lWininet


LIBS += -L$$PWD\lib\x64\ -lQsLog2
LIBS += -L$$PWD\lib\x64\ -lDMDBAccess
LIBS += -L$$PWD\lib\curl-7.80.0\x64\ -llibcurl
LIBS += -L$$PWD\lib\xml-rpc\x64 -llibxmlrpc -llibxmlrpc_abyss -llibxmlrpc_abyss++ \
                                -llibxmlrpc_client -llibxmlrpc_server -llibxmlrpc_server_abyss \
                                -llibxmlrpc_server_cgi -llibxmlrpc_server_w32httpsys \
                                -llibxmlrpc_util -llibxmlrpc_util++ -llibxmlrpc_xmlparse \
                                -llibxmlrpc_xmltok -llibxmlrpc++
LIBS += -L$$PWD\lib\jsoncpp -ljsoncpp_static


RESOURCES += \
    rc.qrc
