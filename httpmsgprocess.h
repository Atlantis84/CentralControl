#ifndef HTTPMSGPROCESS_H
#define HTTPMSGPROCESS_H

#include <QObject>
#include <QJsonObject>
#include "httpresponse.h"
using namespace stefanfrings;
class HttpMsgProcess : public QObject
{
    Q_OBJECT
public:
    explicit HttpMsgProcess(QObject *parent = nullptr);

    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response)=0;
    static HttpMsgProcess* get_interface_obj(QString objType);
signals:
public slots:
};

#endif // HTTPMSGPROCESS_H
