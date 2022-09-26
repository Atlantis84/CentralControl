#ifndef HTTPREGISTERMSG_H
#define HTTPREGISTERMSG_H
#include "httpmsgprocess.h"

class HttpRegisterMsg : public HttpMsgProcess
{
public:
    HttpRegisterMsg();
    virtual void process_http_msg(QJsonObject jsonObject,HttpResponse& response);
};

#endif // HTTPREGISTERMSG_H
