#ifndef HTTPTCONMSG_H
#define HTTPTCONMSG_H

#include "httpmsgprocess.h"
class HttpTconMsg : public HttpMsgProcess
{
    Q_OBJECT
public:
    HttpTconMsg();
    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response);

signals:
    void signal_change_production_failed(QString machinename,QString linename);
public slots:
};

#endif // HTTPTCONMSG_H
