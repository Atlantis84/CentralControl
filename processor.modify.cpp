#include "processor.h"
#include "../GlobDef.h"
#include "md5.h"

#include <string>
#include "json.h"
#include <qglobal.h>
#define RESET_PASSWD "passwd"

// tv4th：新增配置文件
xmlrpc_value*
proc_add_testplan_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

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

// 	if (!check_auth2(session, session)) {
// 		return build_ret_value(EC_AUTH_FAILED, dataP)
// 	}

    int ec /*= mysqlx_session_->add_testplan(project, board, bom, stat, session)*/;

	return build_ret_value(ec, dataP);
}

// tcon：保存配置文件大小(下载下来创建文件需要用的)
xmlrpc_value*
proc_recipe_size_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

	const char* name;
	int size;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:i,*})", "name", &name, "size", &size);

	CheckIfFaultOccurredServer();

    int32_t ec /*= mysqlx_session_->add_tcon_recipe_size(name, size)*/;

	return build_ret_value(ec, dataP);
}

// 保存配置文件块，文件分块是客户端完成的，服务端并不关心，只需要保存即可
xmlrpc_value*
proc_save_recipe_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* name;
	const char* md5_str;
	const char* ctx;
	int offset;
	int size;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,s:i,s:i,*})", "name", &name, "md5", &md5_str, "data", &ctx, "offset", &offset, "size", &size);

	CheckIfFaultOccurredServer();

    int ec /*= mysqlx_session_->add_tcon_testplan(name, md5_str, ctx, offset, size, "")*/;

	return build_ret_value(ec, dataP);
}

std::string UTF8ToGBK(const char* strUTF8)
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

// 更新配置文件
xmlrpc_value*
proc_update_recipe_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* type;
	int size = 0;
	xmlrpc_value* valueP;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:i,s:A,*})", "testtype", &type, "size", &size, "data", &valueP);
	CheckIfFaultOccurredServer();

	int ec;
	int c = xmlrpc_array_size(envP, valueP);
	for (int i = 0; i < c; i++) {
		xmlrpc_value* v;
		xmlrpc_array_read_item(envP, valueP, i, &v);

		xmlrpc_value* areaName;
		const char* areaName_str;
		xmlrpc_struct_read_value(envP, v, "areaName", &areaName);
		xmlrpc_read_string(envP, areaName, &areaName_str);

		xmlrpc_value* machineName;
		size_t machineName_len;
		const unsigned char* machineName_b64;
		xmlrpc_struct_read_value(envP, v, "machineName", &machineName);
		xmlrpc_read_base64(envP, machineName, &machineName_len, &machineName_b64);
		char machineName_str[200] = { 0 };
		memcpy(machineName_str, machineName_b64, machineName_len);

		xmlrpc_value* boardStyle;
		const char* boardStyle_str;
		xmlrpc_struct_read_value(envP, v, "boardStyle", &boardStyle);
		xmlrpc_read_string(envP, boardStyle, &boardStyle_str);

		xmlrpc_value* machineStyleName;
		const char* machineStyleName_str;
		xmlrpc_struct_read_value(envP, v, "machineStyleName", &machineStyleName);
		xmlrpc_read_string(envP, machineStyleName, &machineStyleName_str);

		xmlrpc_value* halfMaterialNumber;
		const char* halfMaterialNumber_str;
		xmlrpc_struct_read_value(envP, v, "halfMaterialNumber", &halfMaterialNumber);
		xmlrpc_read_string(envP, halfMaterialNumber, &halfMaterialNumber_str);

		xmlrpc_value* stat;
		int stat_val;
		xmlrpc_struct_read_value(envP, v, "stat", &stat);
		xmlrpc_read_int(envP, stat, &stat_val);

        ec /*= mysqlx_session_->updata_tcon_upgrade(type, areaName_str, machineName_str, boardStyle_str, machineStyleName_str, halfMaterialNumber_str, stat_val)*/;
		if (ec < 0) {
			break;
		}
	}

	return build_ret_value(ec, dataP);
}

// 更新机台，线体更新配置文件完成
xmlrpc_value*
proc_update_recipe_machine_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* type;
	const char* area;
	const char* machine;
	int machine_len = 0;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:6,*})", "testtype", &type, "area", &area, "machine", &machine, &machine_len);
	CheckIfFaultOccurredServer();

	char machine_str[200] = { 0 };
	memcpy_s(machine_str, sizeof(machine_str) / sizeof(machine_str[0]) - 1, machine, machine_len);
    int ec /*= mysqlx_session_->updata_tcon_upgrade_machine(type, area, machine_str, 0)*/;
	return build_ret_value(ec, dataP);
}

// tv4th：新建测试方案
xmlrpc_value*
proc_new_testplan_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* project;
	const char* board;
	const char* bom;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,*})", "project", &project, "board", &board, "bom", &bom);
	CheckIfFaultOccurredServer();

    int ec/* = mysqlx_session_->new_testplan(project, board, bom)*/;

	return build_ret_value(ec, dataP);
}

// tv4th：测试方案大小，客户端创建文件用
xmlrpc_value*
proc_update_testplan_size_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* project;
	const char* board;
	const char* bom;
	int size;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,s:i,*})", "project", &project, "board", &board, "bom", &bom, "size", &size);
	CheckIfFaultOccurredServer();

    int ec /*= mysqlx_session_->update_testplan_size(project, board, bom, size)*/;
	return build_ret_value(ec, dataP);
}

// tv4th：更新配置文件
xmlrpc_value*
proc_update_testplan_trunk_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

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

    int ec /*= mysqlx_session_->update_testplan_trunk(project, board, bom, md5_str, ctx, offset, size, "")*/;
	return build_ret_value(ec, dataP);
}

xmlrpc_value*
proc_test_json_insert_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
//	L_INFO("proc_test_json_insert is called");
	const char* area;
	int machine_len = 0;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:6,*})", "testdata", &area, &machine_len);
	CheckIfFaultOccurredServer();

	std::string strtocut = area;
	strtocut = strtocut.substr(strtocut.find("TestData")-1,strtocut.find_last_of("]")-strtocut.find("TestData")+2);
	//L_INFO("the str is: {0}", strtocut);
	Json::Value v;
	Json::Reader reader;
	if (reader.parse(area, v))
	{
//		L_INFO("proc_test_json @ {0}", v["MachineNo"].asString());
        ;
	}
	else {
//		L_ERROR("proc_test_json parse json ERROR!");
        ;
		return build_ret_value(EC_DB_ERROR, dataP);
	}
	
	std::string cBoardType = v["PlateType"].asString();
	std::string cHalfMaterialNum = v["SemiProductNo"].asString();
	std::string cAreaName = v["AreaName"].asString();
	std::string cMachineNo = v["MachineNo"].asString();
	std::string cUserName = v["UserName"].asString();
	std::string cDataArray = strtocut.c_str();
    int ec/* = mysqlx_session_->test_json_insert(cBoardType.c_str(),cHalfMaterialNum.c_str(),cAreaName.c_str(),cMachineNo.c_str(),cUserName.c_str(),strtocut.c_str())*/;
	return build_ret_value(ec, dataP);
}
