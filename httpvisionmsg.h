#ifndef HTTPVISIONMSG_H
#define HTTPVISIONMSG_H

#include "httpmsgprocess.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>
class HttpVisionMsg : public HttpMsgProcess
{
    Q_OBJECT
public:
    HttpVisionMsg();
    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response);
private:
signals:
public slots:
};

#endif // HTTPVISIONMSG_H
