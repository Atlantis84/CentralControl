#include "testplanthread.h"
#include "QsLog.h"
#include "xmlrpc-c/base.h"
#include "xmlrpc-c/server.h"
#include "xmlrpc-c/server_abyss.h"
#include "curl/curl.h"
#include "processor.h"
using namespace QsLogging;
static xmlrpc_env env;
static xmlrpc_server_abyss_t* serverP = nullptr;
//just for test
#include "xmlrpc-c/client_global.h"
//static char s_rpc_server_url_[256] = { 0 };
//static xmlrpc_env s_env_;
#define RPCCLIENT_NAME			"TCON"
#define RPCCLIENT_VERSION		"1.0"
#define RPCSERVER_DEFAULT_URL	"http://127.0.0.1:8888/RPC2"

#define REPICE_TRUNK_SIZE		(64 * 1024)
TestPlanThread::TestPlanThread()
{

}

void TestPlanThread::run()
{
    //test xml-rpc
    if(curl_global_init(CURL_GLOBAL_ALL) == CURLE_OK)
        QLOG_INFO()<<"curl init SUCCESS";
    else
        QLOG_WARN()<<"curl init FAILED";
    xmlrpc_server_abyss_parms parms = { 0 };
    xmlrpc_registry* registryP = nullptr;
    do
    {
        xmlrpc_env_init(&env);
        registryP = xmlrpc_registry_new(&env);
        if (env.fault_occurred) {
            QLOG_WARN()<<"xmlrpc_registry_new() failed. "<<env.fault_string;
            break;
        }

#define PROC_ACT(act__) {"proc."#act__, &proc_##act__ },
        static xmlrpc_method_info3 methodInfo[] = {
            PROC_ACTS()
        };
#undef PROC_ACT
        for (unsigned int i = 0; i < sizeof(methodInfo) / sizeof(methodInfo[0]); i++) {
                    xmlrpc_registry_add_method3(&env, registryP, &methodInfo[i]);
                    if (env.fault_occurred) {
                        QLOG_WARN()<<"xmlrpc_registry_add_method3() failed. "<<methodInfo[i].methodName;
                        break;
                    }
                }

                if (env.fault_occurred) {
                    QLOG_ERROR()<<"xmlrpc_registry_add_method3() failed. "<<env.fault_string;
                    break;
                }

                char szLogName[MAX_PATH] = { 0 };
                parms.config_file_name = nullptr;   /* Select the modern normal API */
                parms.registryP = registryP;
                parms.port_number = 8880;
                parms.log_file_name = szLogName;

                xmlrpc_server_abyss_global_init(&env);
                if (env.fault_occurred) {
                    QLOG_ERROR()<<"xmlrpc_server_abyss_global_init() failed."<<env.fault_string;
                    break;
                }

                xmlrpc_server_abyss_create(&env, &parms, XMLRPC_APSIZE(log_file_name), &serverP);
                if (env.fault_occurred) {
                    QLOG_ERROR()<<"xmlrpc_server_abyss_create() failed."<<env.fault_string;
                    break;
                }
                xmlrpc_server_abyss_sig* oldHandlersP;

                xmlrpc_server_abyss_setup_sig(&env, serverP, &oldHandlersP);

                if (!env.fault_occurred) {
                    if (!startup_db_client("127.0.0.1", 8997)) {
                        break;
                    }

                    start_check_upgrade("http://127.0.0.1", 8080);
                    QLOG_WARN()<<"Running server @8080";
                    xmlrpc_server_abyss_run_server(&env, serverP);
                    QLOG_WARN()<<"Stopping server @8080";
                    xmlrpc_server_abyss_restore_sig(oldHandlersP);

                    free(oldHandlersP);

                    stop_check_upgrade();
                }
                xmlrpc_server_abyss_destroy(serverP);

                if (env.fault_occurred) {
                    QLOG_WARN()<<"xmlrpc_server_abyss() failed. "<<env.fault_string;
                    break;
                }
    }
    while(0);

    if (registryP)
    {
        xmlrpc_registry_free(registryP);
    }

    xmlrpc_server_abyss_global_term();

    clean_db_client();
    curl_global_cleanup();
    //test xml-rpc
}
