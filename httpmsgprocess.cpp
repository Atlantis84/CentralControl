#include "httpmsgprocess.h"
#include <QJsonDocument>
#include <QTextCodec>
#include "gdatafactory.h"
#include "QsLog.h"
#include "httpaoimsg.h"
#include "httpregistermsg.h"
#include "httptconmsg.h"
#include "httpvisionmsg.h"
#include "httptvmsg.h"
HttpMsgProcess::HttpMsgProcess(QObject *parent) : QObject(parent)
{

}

HttpMsgProcess *HttpMsgProcess::get_interface_obj(QString objType)
{
    HttpMsgProcess* tmpProcess;
    if((objType == "TCON_TEST")||(objType == "TCON_RESULT")||(objType == "TCON_NULL"))
        tmpProcess = new HttpTconMsg();
    else if(objType == "MACHINE_REGISTER")
        tmpProcess = new HttpRegisterMsg();
    else if((objType == "AOI_INFO") || (objType == "AOI_INIT"))
        tmpProcess = new HttpAOIMsg();
    else if(objType == "VISION_LEAD")
        tmpProcess = new HttpVisionMsg();
    else if((objType == "TV_TEST")||(objType == "TV_RESULT"))
        tmpProcess = new HttpTVMsg();
    else
        return nullptr;
    return tmpProcess;
}
