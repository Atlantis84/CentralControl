#ifndef HTTPTVMSG_H
#define HTTPTVMSG_H

#include "httpmsgprocess.h"
class HttpTVMsg : public HttpMsgProcess
{
public:
    HttpTVMsg();
    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response);
};

#endif // HTTPTVMSG_H
