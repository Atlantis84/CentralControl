
#include "processor.h"
#include "../GlobDef.h"

#include "xmlrpc-c/base.h"
#include "xmlrpc-c/client.h"
#include "xmlrpc-c/client_global.h"
#include "xmlrpc-c/util.h"
#include "json.h"
#include <qglobal.h>
#define RPCSERVER_NAME			"TVRPCSERVER"
#define RPCSERVER_VERSION		"1.0"

static char db_server_url[256] = { 0 };
static xmlrpc_env env;

bool startup_db_client(const char* host, unsigned short port)
{
	/* Initialize our error-handling environment. */
	xmlrpc_env_init(&env);
	if (env.fault_occurred) {
//		L_ERROR("{0} @ {1}, fault_string : {2} fault_code {3})",
//			__FUNCTION__, __LINE__, env.fault_string, env.fault_code);
		return false;
	}

	/* Create the global XML-RPC client object. */
	xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, RPCSERVER_NAME, RPCSERVER_VERSION, NULL, 0);
	if (env.fault_occurred) {
//		L_ERROR("{0} @ {1}, fault_string : {2} fault_code {3})",
//			__FUNCTION__, __LINE__, env.fault_string, env.fault_code);
		return false;
	}

	sprintf_s(db_server_url, sizeof(db_server_url) / sizeof(db_server_url[0]), "http://%s:%d/RPC2", host, port);

//	L_DEBUG("[DB] addr: {0}", db_server_url);

	return true;
}

void clean_db_client()
{
	/* Clean up our error-handling environment. */
	xmlrpc_env_clean(&env);

	/* Shutdown our XML-RPC client library. */
	xmlrpc_client_cleanup();
}

const char* get_db_server_url()
{
	return db_server_url;
}

// 枚举配方信息
xmlrpc_value*
proc_query_recipe(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* name;
	xmlrpc_decompose_value(envP, paramArrayP, "(s)", &name);
//	L_TRACE("name = {0}", name);
	CheckIfFaultOccurredServer();

	call_db_server(query_recipe);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// 枚举配方文件块
xmlrpc_value*
proc_query_recipe_trunk(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

	const char* name;
	xmlrpc_decompose_value(envP, paramArrayP, "(s)", &name);
	CheckIfFaultOccurredServer();

	call_db_server(query_recipe_trunk);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

//////////////////////////////////////////////////////////////////////////////////////

xmlrpc_value*
proc_add_testplan(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

	int project;
	const char* board;
	const char* bom;
	int stat;
	const char* session;
	xmlrpc_decompose_value(envP, paramArrayP
		, "({s:i,s:s,s:s,s:i,s:s,*})"
		, "project", &project
		, "board", &board
		, "bom", &bom
		, "stat", &stat
		, "session", &session);

	CheckIfFaultOccurredServer();

	call_db_server(testplan);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// 检查tcon测试方案是否存在
xmlrpc_value*
proc_tcon_testplan_exist(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* boardstyle;
	const char* machinestylename;
	const char* halfmaterialnum;
	int machine_len = 0;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,*})", "boardStyle", &boardstyle, "machineStyleName", &machinestylename, "halfMaterialNumber", &halfmaterialnum);
	CheckIfFaultOccurredServer();

	call_db_server(tcon_testplan_exist);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// 检查当前文件块是否存在
xmlrpc_value*
proc_check_recipe_trunk(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	xmlrpc_value* arrayP;
	xmlrpc_decompose_value(envP, paramArrayP, "(A)", &arrayP);
	CheckIfFaultOccurredServer();

	if (xmlrpc_value_type(arrayP) != XMLRPC_TYPE_ARRAY || !xmlrpc_array_size(envP, arrayP)) {
		return build_ret_value(EC_INVALID_ARGS, dataP);
	}
	call_db_server(check_recipe_trunk);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// tcon：保存配置文件大小
xmlrpc_value*
proc_recipe_size(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

	const char* name;
	int size;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:i,*})", "name", &name, "size", &size);
	CheckIfFaultOccurredServer();

	call_db_server(recipe_size);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// 保存配置文件块
xmlrpc_value*
proc_save_recipe(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* name;
	const char* md5_str;
	const char* ctx;
	int offset;
	int size;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,s:i,s:i,*})", "name", &name, "md5", &md5_str, "data", &ctx, "offset", &offset, "size", &size);
	CheckIfFaultOccurredServer();

	call_db_server(save_recipe);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}


std::string UTF8ToGBKA(const char* strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	return strTemp;
}

// tcon：检查机台，线体更新状态
xmlrpc_value*
proc_check_recipe_update(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();
	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* type;
	const char* area;
	const char* machine;
	int machine_len = 0;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:6,*})", "testtype", &type, "area", &area, "machine", &machine, &machine_len);
    std::string tmpMA = UTF8ToGBKA(machine);
	std::string tmpArea = area;
//	L_INFO("check_recipe_update @ {0}:{1}", tmpMA,tmpArea);
	CheckIfFaultOccurredServer();

	check_update_machine(type);

	call_db_server(check_recipe_update);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// tcon：检查机台，线体更新状态
xmlrpc_value*
proc_update_recipe(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* type;
	int size = 0;
	xmlrpc_value* valueP;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:i,s:A,*})", "testtype", &type, "size", &size, "data", &valueP);
	CheckIfFaultOccurredServer();

	int c = xmlrpc_array_size(envP, valueP);
	for (int i = 0; i < c; i++) {
		xmlrpc_value* v;
		xmlrpc_array_read_item(envP, valueP, i, &v);

		xmlrpc_value* areaName;
		const char* areaName_str;
		xmlrpc_struct_read_value(envP, v, "areaName", &areaName);
		xmlrpc_read_string(envP, areaName, &areaName_str);
//		L_WARN("manual single change production-areaName:{0}", areaName_str);

		xmlrpc_value* machineName;
		size_t machineName_len;
		const unsigned char* machineName_b64;
		xmlrpc_struct_read_value(envP, v, "machineName", &machineName);
		xmlrpc_read_base64(envP, machineName, &machineName_len, &machineName_b64);
		char machineName_str[200] = { 0 };
		memcpy(machineName_str, machineName_b64, machineName_len);
//		L_WARN("manual single change production-machineName:{0}", UTF8ToGBK(machineName_str));

		xmlrpc_value* boardStyle;
		const char* boardStyle_str;
		xmlrpc_struct_read_value(envP, v, "boardStyle", &boardStyle);
		xmlrpc_read_string(envP, boardStyle, &boardStyle_str);
//		L_WARN("manual single change production-boardStyle:{0}", boardStyle_str);

		xmlrpc_value* machineStyleName;
		const char* machineStyleName_str;
		xmlrpc_struct_read_value(envP, v, "machineStyleName", &machineStyleName);
		xmlrpc_read_string(envP, machineStyleName, &machineStyleName_str);

		xmlrpc_value* halfMaterialNumber;
		const char* halfMaterialNumber_str;
		xmlrpc_struct_read_value(envP, v, "halfMaterialNumber", &halfMaterialNumber);
		xmlrpc_read_string(envP, halfMaterialNumber, &halfMaterialNumber_str);
//		L_WARN("manual single change production-halfMaterialNumber:{0}", halfMaterialNumber_str);
	}

	call_db_server(update_recipe);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

xmlrpc_value*
proc_test_json(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();
	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	
	CheckIfFaultOccurredServer();

	call_db_server(test_json_insert);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

//查询配置文件参数
xmlrpc_value*
proc_query_test_config_data(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();
	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

	CheckIfFaultOccurredServer();

	call_db_server(query_test_config_data);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

//获取tcon配置文件详细参数
xmlrpc_value*
proc_get_test_details(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();
	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

	CheckIfFaultOccurredServer();

    call_db_server(get_test_details_db);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// 更新配置文件
xmlrpc_value*
proc_update_recipe_machine(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	char* type;
	char* areaname;
	char* machine;
	int len;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:6,*})", "testtype", &type, "area", &areaname, "machine", &machine, &len);
	CheckIfFaultOccurredServer();

	char machine_str[256] = { 0 };
	strncpy_s(machine_str, machine, len);

	call_db_server(update_recipe_machine);
	decompose_ret_value();

	if (ec >= EC_SUCCESS) {
		notify_result(type, areaname, machine_str, EC_SUCCESS);
//		L_TRACE("线体: {0}, 机台: {1}, 类型{2} 换产【成功】", areaname, machine_str, type);
	}
	else {
		notify_result(type, areaname, machine_str, EC_DB_ERROR);
//		L_TRACE("线体: {0}, 机台: {1}, 类型{2} 换产 #失败#", areaname, machine_str, type);
	}

	return build_ret_value(ec, dataP);
}

// tv4th：新建测试方案
xmlrpc_value*
proc_new_testplan(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* project;
	const char* board;
	const char* bom;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,*})", "project", &project, "board", &board, "bom", &bom);
	CheckIfFaultOccurredServer();

	call_db_server(new_testplan);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// tv4th：查询机台，线体更新信息
xmlrpc_value*
proc_check_tv_recipe_update(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* type;
	const char* area;
	const char* machine;
	int machine_len = 0;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:6,*})", "testtype", &type, "area", &area, "machine", &machine, &machine_len);

	CheckIfFaultOccurredServer();

	check_update_machine(type);

	call_db_server(check_tv_recipe_update);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// 查询TV测试方案信息
xmlrpc_value*
proc_query_testplan2(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* project;
	const char* board;
	const char* bom;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,*})", "project", &project, "board", &board, "bom", &bom);
	CheckIfFaultOccurredServer();

	call_db_server(query_testplan2);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// 查询(下载)配置文件块
xmlrpc_value*
proc_query_testplan_trunk(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* project;
	const char* board;
	const char* bom;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,*})", "project", &project, "board", &board, "bom", &bom);
	CheckIfFaultOccurredServer();

	call_db_server(query_testplan_trunk);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// tv4th：测试方案大小，客户端创建文件用
xmlrpc_value*
proc_update_testplan_size(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* project;
	const char* board;
	const char* bom;
	int size;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,s:i,*})", "project", &project, "board", &board, "bom", &bom, "size", &size);
	CheckIfFaultOccurredServer();

	call_db_server(update_testplan_size);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// tv4th：更新配置文件
xmlrpc_value*
proc_update_testplan_trunk(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* project;
	const char* board;
	const char* bom;
	const char* md5_str;
	const char* ctx;
	int offset;
	int size;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,s:s,s:s,s:i,s:i,*})", "project", &project, "board", &board, "bom", &bom, "md5", &md5_str, "data", &ctx, "offset", &offset, "size", &size);
	CheckIfFaultOccurredServer();

	call_db_server(update_testplan_trunk);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}

// tv4th：检查配置文件块是否存在
xmlrpc_value*
proc_query_tv_recipe(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	CleanClientEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* project;
	const char* board;
	const char* bom;

	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,*})", "project", &project, "board", &board, "bom", &bom);
	CheckIfFaultOccurredServer();

	call_db_server(query_tv_recipe);
	decompose_ret_value();
	return build_ret_value(ec, dataP);
}
