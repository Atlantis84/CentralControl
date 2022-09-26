#pragma once

#include "xmlrpc-c/base.h"
#include "xmlrpc-c/server.h"
#include "xmlrpc-c/server_abyss.h"

#include "procact.h"

extern class mysqlx_session* mysqlx_session_;

#define PROC_ACT(act__) \
    xmlrpc_value* \
    proc_##act__(\
        xmlrpc_env* const envP, \
        xmlrpc_value* const paramArrayP, \
        void* const serverInfo, \
        void* const channelInfo);
PROC_ACTS()

#define PROC_ACT_DB(act__) \
    xmlrpc_value* \
    proc_##act__(\
        xmlrpc_env* const envP, \
        xmlrpc_value* const paramArrayP, \
        void* const serverInfo, \
        void* const channelInfo);
PROC_ACTS_DB()

bool check_auth(const char* uuid, const char* dst_auth);
bool check_auth2(const char* src_uuid, const char* dst_uuid);

bool startup_db_client(const char* host, unsigned short port);
void clean_db_client();

xmlrpc_value* proc_update_recipe_machine(
        xmlrpc_env* const envP,
        xmlrpc_value* const paramArrayP,
        void* const serverInfo,
        void* const channelInfo);

xmlrpc_value*
proc_add_testplan(
    xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP,
    void* const serverInfo,
    void* const channelInfo);

xmlrpc_value*
proc_tcon_testplan_exist(
    xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP,
    void* const serverInfo,
    void* const channelInfo);

xmlrpc_value*
proc_check_recipe_trunk(
    xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP,
    void* const serverInfo,
    void* const channelInfo);

xmlrpc_value*
proc_save_recipe(
    xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP,
    void* const serverInfo,
    void* const channelInfo);

xmlrpc_value*
proc_query_recipe(
    xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP,
    void* const serverInfo,
    void* const channelInfo);

xmlrpc_value*
proc_query_recipe_trunk(
    xmlrpc_env* const envP,
    xmlrpc_value* const paramArrayP,
    void* const serverInfo,
    void* const channelInfo);

const char* get_db_server_url();

// ???????????
void start_check_upgrade(const char* url, unsigned short port);
// ??????????(?????????????????)
void stop_check_upgrade();
// ??????????????????????????????????????????????????????(TCON_TEST->TCON_RESULT)
void check_update_machine(const char* type);
// ????????????
void notify_result(const char* type, const char* line, const char* machine, int result);

#undef PROC_ACT
