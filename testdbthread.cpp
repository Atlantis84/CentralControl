#include "testdbthread.h"

#include "QsLog.h"
#include "xmlrpc-c/base.h"
#include "xmlrpc-c/server.h"
#include "xmlrpc-c/server_abyss.h"
#include "curl/curl.h"
#include "processor.h"
using namespace QsLogging;
static xmlrpc_env env;
static xmlrpc_server_abyss_t* serverP = 0;
TestDBThread::TestDBThread()
{

}

void TestDBThread::run()
{
    xmlrpc_server_abyss_parms parms = { 0 };
    xmlrpc_registry* registryP = 0;

    do {
        xmlrpc_env_init(&env);

        registryP = xmlrpc_registry_new(&env);
        if (env.fault_occurred) {
            QLOG_ERROR()<<"xmlrpc_registry_new() failed."<<env.fault_string;
            break;
        }

#define PROC_ACT_DB(act__) {"proc."###act__, &proc_##act__ },
        static xmlrpc_method_info3 methodInfo[] = {
            PROC_ACTS_DB()
        };
#undef PROC_ACT_DB

        for (int i = 0; i < sizeof(methodInfo) / sizeof(methodInfo[0]); i++) {
            xmlrpc_registry_add_method3(&env, registryP, &methodInfo[i]);
            if (env.fault_occurred) {
                QLOG_ERROR()<<"xmlrpc_registry_add_method3() failed."<<methodInfo[i].methodName;
                break;
            }
        }

        if (env.fault_occurred) {
            QLOG_ERROR()<<"xmlrpc_registry_add_method3() failed."<<env.fault_string;
            break;
        }

        char szLogName[MAX_PATH] = { 0 };

        memset(&parms, 0, sizeof(parms));
        parms.config_file_name = NULL;   /* Select the modern normal API */
        parms.registryP = registryP;
        parms.port_number = 8997;
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
            QLOG_WARN()<<"Running server @"<<QString::number(8997);
            xmlrpc_server_abyss_run_server(&env, serverP);

            QLOG_WARN()<<"Stopping server @"<<QString::number(8997);
            xmlrpc_server_abyss_restore_sig(oldHandlersP);

            free(oldHandlersP);
        }
        xmlrpc_server_abyss_destroy(serverP);

        if (env.fault_occurred) {
            QLOG_ERROR()<<"xmlrpc_server_abyss() failed."<<env.fault_string;
            break;
        }
    } while (0);

    if (registryP) {
        xmlrpc_registry_free(registryP);
    }

    xmlrpc_server_abyss_global_term();
}
