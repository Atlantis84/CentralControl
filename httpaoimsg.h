#ifndef HTTPAOIMSG_H
#define HTTPAOIMSG_H
#include "httpmsgprocess.h"

class HttpAOIMsg : public HttpMsgProcess
{
public:
    HttpAOIMsg();
    virtual void process_http_msg(QJsonObject httpJsonObj,HttpResponse& response);
};

#endif // HTTPAOIMSG_H
