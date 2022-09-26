#include "processor.h"
#include "../GlobDef.h"
#include <map>
#include <string>
#include "json.h"
#include <qglobal.h>
#define testplan_array_item_init(v_) \
    xmlrpc_set_str_v(v_, uuid); \
    xmlrpc_set_str_v(v_, project); \
    xmlrpc_set_str_v(v_, board); \
    xmlrpc_set_str_v(v_, bom); \
    \
    xmlrpc_set_sec_v(v_, create_ts); \
    xmlrpc_set_sec_v(v_, edit_ts); \
    \
    xmlrpc_set_b64_v(v_, desc);

#define testplan_array_cb() \
    auto cb = [&envP, &valueP](const std::string& uuid, const std::string& project, const std::string& board, const std::string& bom, time_t create_ts, time_t edit_ts, const std::string& desc) \
    { \
        xmlrpc_value* v = xmlrpc_struct_new(envP); \
        \
        testplan_array_item_init(v); \
        \
        xmlrpc_array_append_item(envP, valueP, v); \
    };

xmlrpc_value*
proc_testplan_status_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

	const char* editor = 0;
	xmlrpc_read_string(envP, paramArrayP, &editor);
	CheckIfFaultOccurredServer();

	xmlrpc_value* valueP = xmlrpc_array_new(envP);
	auto cb = [&envP, &valueP](const std::string& uuid, int yahe, int sn, int pcbon, int delay, int img, int comopen, int comclose, int mes, int pcboff, int order, int stat, const std::string& creator, const std::string& editor)
	{
		xmlrpc_value* v = xmlrpc_struct_new(envP);

		xmlrpc_set_str_v(v, uuid);
		xmlrpc_set_int_v(v, yahe);
		xmlrpc_set_int_v(v, sn);
		xmlrpc_set_int_v(v, pcbon);
		xmlrpc_set_int_v(v, delay);
		xmlrpc_set_int_v(v, img);
		xmlrpc_set_int_v(v, comopen);
		xmlrpc_set_int_v(v, comclose);
		xmlrpc_set_int_v(v, mes);
		xmlrpc_set_int_v(v, pcboff);
		xmlrpc_set_int_v(v, order);
		xmlrpc_set_int_v(v, stat);

		xmlrpc_set_b64_v(v, creator);
		xmlrpc_set_b64_v(v, editor);

		xmlrpc_array_append_item(envP, valueP, v);
	};
    int ec /*= mysqlx_session_->query_testplan_status(editor, cb)*/;

	xmlrpc_struct_set_value(envP, dataP, "status", valueP);
	xmlrpc_DECREF(valueP);

	return build_ret_value(ec, dataP);
}

//xmlrpc_value*
//proc_testplan_strcmp_db(
//	xmlrpc_env* const envP,
//	xmlrpc_value* const paramArrayP,
//	void* const serverInfo,
//	void* const channelInfo)
//{
//    Q_UNUSED(serverInfo);
//    Q_UNUSED(channelInfo);
//	CleanServerEnv();

//	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

//	const char* uuid = 0;
//	xmlrpc_read_string(envP, paramArrayP, &uuid);
//	CheckIfFaultOccurredServer();

//	xmlrpc_value* valueP = xmlrpc_array_new(envP);
//	auto cb = [&envP, &valueP](const std::string& uuid, const std::string& board, const std::string& name, int valuemin, int valuemax, int timeout, int order, int stat, const std::string& creator, const std::string& editor)
//	{
//		xmlrpc_value* v = xmlrpc_struct_new(envP);

//		xmlrpc_set_str_v(v, uuid);
//		xmlrpc_set_str_v(v, board);
//		xmlrpc_set_b64_v(v, name);
//		xmlrpc_set_int_v(v, valuemin);
//		xmlrpc_set_int_v(v, valuemax);
//		xmlrpc_set_int_v(v, timeout);
//		xmlrpc_set_int_v(v, order);
//		xmlrpc_set_int_v(v, stat);

//		xmlrpc_set_b64_v(v, creator);
//		xmlrpc_set_b64_v(v, editor);

//		xmlrpc_array_append_item(envP, valueP, v);
//	};
//    int ec /*= mysqlx_session_->query_testplan_strcmp(uuid, cb)*/;
//	xmlrpc_struct_set_value(envP, dataP, "strcmp", valueP);
//	xmlrpc_DECREF(valueP);

//	return build_ret_value(ec, dataP);
//}

//xmlrpc_value*
//proc_testplan_voltage_db(
//	xmlrpc_env* const envP,
//	xmlrpc_value* const paramArrayP,
//	void* const serverInfo,
//	void* const channelInfo)
//{
//    Q_UNUSED(serverInfo);
//    Q_UNUSED(channelInfo);
//	CleanServerEnv();

//	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

//	const char* uuid = 0;
//	xmlrpc_read_string(envP, paramArrayP, &uuid);
//	CheckIfFaultOccurredServer();

//	xmlrpc_value* valueP = xmlrpc_array_new(envP);
//	auto cb = [&envP, &valueP](const std::string& uuid, const std::string& board, const std::string& name, int valuemin, int valuemax, int timeout, int order, int stat, const std::string& creator, const std::string& editor)
//	{
//		xmlrpc_value* v = xmlrpc_struct_new(envP);

//		xmlrpc_set_str_v(v, uuid);
//		xmlrpc_set_str_v(v, board);
//		xmlrpc_set_b64_v(v, name);
//		xmlrpc_set_int_v(v, valuemin);
//		xmlrpc_set_int_v(v, valuemax);
//		xmlrpc_set_int_v(v, timeout);
//		xmlrpc_set_int_v(v, order);
//		xmlrpc_set_int_v(v, stat);

//		xmlrpc_set_b64_v(v, creator);
//		xmlrpc_set_b64_v(v, editor);

//		xmlrpc_array_append_item(envP, valueP, v);
//	};
//    int ec /*= mysqlx_session_->query_testplan_voltage(uuid, cb)*/;

//	xmlrpc_struct_set_value(envP, dataP, "voltage", valueP);
//	xmlrpc_DECREF(valueP);

//	return build_ret_value(ec, dataP);
//}

// �����Է����Ƿ����
xmlrpc_value*
proc_tcon_testplan_exist_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	const char* boardStyle;
	const char* machineStyleName;
	const char* halfMaterialNumber;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:s,*})", "boardStyle", &boardStyle, "machineStyleName", &machineStyleName, "halfMaterialNumber", &halfMaterialNumber);
	CheckIfFaultOccurredServer();

    xmlrpc_int32 ec /*= mysqlx_session_->check_tcon_testplan_exist(boardStyle, machineStyleName, halfMaterialNumber)*/;

	return build_ret_value(ec, dataP);
}

// ��������ļ����Ƿ����
xmlrpc_value*
proc_check_recipe_trunk_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
	xmlrpc_value* arrayP;
	xmlrpc_decompose_value(envP, paramArrayP, "(A)", &arrayP);
	CheckIfFaultOccurredServer();

	std::vector<std::string> md5;
	for (int i = 0; i < xmlrpc_array_size(envP, arrayP); i++) {
		xmlrpc_value* m = xmlrpc_array_get_item(envP, arrayP, i);
		const char* md5_str;
		xmlrpc_read_string(envP, m, &md5_str);
		md5.push_back(md5_str);
	}
	xmlrpc_DECREF(arrayP);

	xmlrpc_value* valueP = xmlrpc_array_new(envP);
	auto cb = [&envP, &valueP](const std::string& md5)
	{
		xmlrpc_array_append_item(envP, valueP, xmlrpc_string_new(envP, md5.c_str()));
	};
    xmlrpc_int32 ec /*= mysqlx_session_->check_tcon_hstvt_trunk(md5, cb)*/;

	xmlrpc_struct_set_value(envP, dataP, "md5", valueP);
	xmlrpc_DECREF(valueP);

	return build_ret_value(ec, dataP);
}

// ��ѯ�����ļ�����Ϣ
xmlrpc_value*
proc_query_recipe_db(
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
	xmlrpc_decompose_value(envP, paramArrayP, "(s)", &name);
	CheckIfFaultOccurredServer();

	xmlrpc_value* valueP = xmlrpc_array_new(envP);
	unsigned size = 0;
	auto cb = [&envP, &valueP, &size](const std::string& md5, unsigned offset, unsigned trunk_size, unsigned file_size)
	{
		xmlrpc_value* v = xmlrpc_struct_new(envP);

		xmlrpc_set_str_v(v, md5);
		xmlrpc_set_int_v(v, offset);
		xmlrpc_set_int_v(v, trunk_size);

		size = file_size;

		xmlrpc_array_append_item(envP, valueP, v);
	};
    int ec /*= mysqlx_session_->query_tcon_testplan(name, cb)*/;

	xmlrpc_struct_set_value(envP, dataP, "trunk", valueP);
	xmlrpc_DECREF(valueP);

	xmlrpc_struct_set_value(envP, dataP, "file_size", xmlrpc_int_new(envP, size));

	return build_ret_value(ec, dataP);
}

// ��ѯ(����)�����ļ���
xmlrpc_value*
proc_query_recipe_trunk_db(
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
	xmlrpc_decompose_value(envP, paramArrayP, "(s)", &name);
	CheckIfFaultOccurredServer();

	auto cb = [&envP, &dataP](const std::string& md5, const std::string content)
	{
		xmlrpc_set_str_v(dataP, md5);
		xmlrpc_set_str_v(dataP, content);
	};
    int ec /*= mysqlx_session_->query_tcon_hstvt(name, cb)*/;

	return build_ret_value(ec, dataP);
}

xmlrpc_value*
proc_query_databaseuuid_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_struct_new(envP);

	const char* sn;
	xmlrpc_decompose_value(envP, paramArrayP, "(s)", &sn);
	CheckIfFaultOccurredServer();

	auto cb = [&envP, &dataP](const std::string& uuid)
	{
		xmlrpc_struct_set_value(envP, dataP, "uuid", xmlrpc_string_new(envP, uuid.c_str()));
	};
    int ec /*= mysqlx_session_->query_databaseuuid(sn, cb)*/;

	xmlrpc_struct_set_value(envP, dataP, "ec", xmlrpc_int_new(envP, ec));
	return dataP;
}

// ��ѯ��̨�����������Ϣ
xmlrpc_value*
proc_check_recipe_update_db(
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

	// ��ֹ���ݿ��ﲻ���ڸû�̨����������ݣ���ô�ᵼ����stat�ֶΣ����¿ͻ��˽�������
	xmlrpc_struct_set_value(envP, dataP, "stat", xmlrpc_int_new(envP, 0));
	auto cb = [&envP, &dataP](const std::string& areaName, const std::string& machineName, const std::string& boardStyle, const std::string& machineStyleName, const std::string& halfMaterialNumber, int stat)
	{
		xmlrpc_set_int_v(dataP, stat);

		char name[200] = { 0 };
		if (stat) {
			if (boardStyle.size()) {
				sprintf_s(name + strlen(name), sizeof(name) / sizeof(name[0]) - strlen(name), "%s-", boardStyle.c_str());
			}
			if (machineStyleName.size()) {
				sprintf_s(name + strlen(name), sizeof(name) / sizeof(name[0]) - strlen(name), "%s-", machineStyleName.c_str());
			}
			if (halfMaterialNumber.size()) {
				sprintf_s(name + strlen(name), sizeof(name) / sizeof(name[0]) - strlen(name), "%s", halfMaterialNumber.c_str());
			}
		}

		xmlrpc_struct_set_value(envP, dataP, "name", xmlrpc_string_new(envP, name));
	};
	char machine_str[200] = { 0 };
	memcpy_s(machine_str, sizeof(machine_str) / sizeof(machine_str[0]) - 1, machine, machine_len);
    int ec /*= mysqlx_session_->query_tcon_upgrade(type, area, machine_str, cb)*/;

	return build_ret_value(ec, dataP);
}

xmlrpc_value*
proc_query_databaselist_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();

	xmlrpc_value* dataP = xmlrpc_array_new(envP);

	const char*	qstarttime = 0; const char* qendtime = 0; const char* qsn = 0; const char* qtestplan = 0;
	const char* qfailprjname = 0; const char* qfailsubprjname = 0; const char* qtestip = 0; const char* qpcname = 0; const char* qtestresult = 0;
	const char* qareaname = 0; const char* qdeviceno = 0; const char* qtoolno = 0;
	xmlrpc_decompose_value(envP, paramArrayP, "(ssssssssssss)", &qstarttime, &qendtime,&qsn, &qtestplan, &qfailprjname, &qfailsubprjname, 
		&qtestip, &qpcname, &qtestresult, &qareaname, &qdeviceno, &qtoolno);
	CheckIfFaultOccurredServer();

	xmlrpc_value* valueP = xmlrpc_array_new(envP);
	auto cb = [&envP, &valueP](const std::string& uuid, const std::string& sn, const std::string& mac, const std::string& softwareversion, const std::string& keyname,
		const std::string& testresult, const std::string& testplan, const std::string& modulebom, const std::string& areaname, const std::string& deviceno, const std::string& toolno,
		const std::string& failprjname, const std::string& failsubprjname, const std::string& testip, const std::string& pcname, const std::string& testtime, const time_t testdate,
		const std::string& testperson)
	{
		xmlrpc_value* v = xmlrpc_struct_new(envP);

		xmlrpc_set_str_v(v, uuid);
		xmlrpc_set_str_v(v, sn);
		xmlrpc_set_str_v(v, mac);
		xmlrpc_set_str_v(v, softwareversion);
		xmlrpc_set_str_v(v, keyname);
		xmlrpc_set_str_v(v, testresult);
		xmlrpc_set_str_v(v, testplan);
		xmlrpc_set_str_v(v, modulebom);
		xmlrpc_set_str_v(v, areaname);
		xmlrpc_set_str_v(v, deviceno);
		xmlrpc_set_str_v(v, toolno);
		xmlrpc_set_str_v(v, failprjname);
		xmlrpc_set_str_v(v, failsubprjname);
		xmlrpc_set_str_v(v, testip);
		xmlrpc_set_str_v(v, pcname);
		xmlrpc_set_str_v(v, testtime);
		//xmlrpc_struct_set_value(envP, v, "testdate", xmlrpc_string_new(envP, testdate.c_str()));
		xmlrpc_set_str_v(v, testperson);

		xmlrpc_array_append_item(envP, valueP, v);
	};
    int ec /*= mysqlx_session_->query_databaselist(qstarttime, qendtime, qsn, qtestplan, qfailprjname, qfailsubprjname,
        qtestip, qpcname, qtestresult, qareaname, qdeviceno, qtoolno, cb)*/;

	return xmlrpc_build_value(envP, "{s:i,s:A}", "ec", ec, "data", valueP);
}

// ��ѯTV���Է�����Ϣ
xmlrpc_value*
proc_query_testplan2_db(
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

    auto cb = [&envP, &dataP](const std::string& uuid, const std::string& project, const std::string& board, const std::string& bom, time_t create_ts, time_t edit_ts, const std::string& desc)
    {
        testplan_array_item_init(dataP);
    };
    int ec /*= mysqlx_session_->query_testplan2(project, board, bom, cb)*/;
    return build_ret_value(ec, dataP);
}

// tv4th����ѯ��̨�����������Ϣ
xmlrpc_value*
proc_check_tv_recipe_update_db(
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

    auto cb = [&envP, &dataP](const std::string& areaName, const std::string& machineName, const std::string& boardStyle, const std::string& machineStyleName, const std::string& halfMaterialNumber, int stat)
    {
        xmlrpc_set_int_v(dataP, stat);
        xmlrpc_set_str_v(dataP, boardStyle);
        xmlrpc_set_str_v(dataP, machineStyleName);
        xmlrpc_set_str_v(dataP, halfMaterialNumber);
    };
    char machine_str[200] = { 0 };
    memcpy_s(machine_str, sizeof(machine_str) / sizeof(machine_str[0]) - 1, machine, machine_len);
    int ec /*= mysqlx_session_->query_tcon_upgrade(type, area, machine_str, cb)*/;

    return build_ret_value(ec, dataP);
}

// ��ѯ(����)�����ļ���
xmlrpc_value*
proc_query_testplan_trunk_db(
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

	xmlrpc_value* valueP = xmlrpc_array_new(envP);
	unsigned size = 0;
	auto cb = [&envP, &valueP, &size](const std::string& md5, unsigned offset, unsigned trunk_size, unsigned file_size)
	{
		xmlrpc_value* v = xmlrpc_struct_new(envP);

		xmlrpc_set_str_v(v, md5);
		xmlrpc_set_int_v(v, offset);
		xmlrpc_set_int_v(v, trunk_size);

		size = file_size;

		xmlrpc_array_append_item(envP, valueP, v);
	};
    int ec /*= mysqlx_session_->query_testplan_trunk(project, board, bom, cb)*/;

	xmlrpc_struct_set_value(envP, dataP, "trunk", valueP);
	xmlrpc_DECREF(valueP);

	xmlrpc_struct_set_value(envP, dataP, "file_size", xmlrpc_int_new(envP, size));

	return build_ret_value(ec, dataP);
}

// tv4th����������ļ����Ƿ����
xmlrpc_value*
proc_query_tv_recipe_db(
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

	unsigned size = 0;
	xmlrpc_value* valueP = xmlrpc_array_new(envP);
	auto cb = [&envP, &valueP, &size](const std::string& md5, unsigned offset, unsigned trunk_size, unsigned file_size)
	{
		xmlrpc_value* v = xmlrpc_struct_new(envP);

		xmlrpc_set_str_v(v, md5);
		xmlrpc_set_int_v(v, offset);
		xmlrpc_set_int_v(v, trunk_size);

		size = file_size;

		xmlrpc_array_append_item(envP, valueP, v);
	};
    int ec/* = mysqlx_session_->query_testplan_trunk(project, board, bom, cb)*/;

 	xmlrpc_struct_set_value(envP, dataP, "trunk", valueP);
 	xmlrpc_DECREF(valueP);

	xmlrpc_struct_set_value(envP, dataP, "file_size", xmlrpc_int_new(envP, size));

	return build_ret_value(ec, dataP);
}

xmlrpc_value*
proc_query_test_config_data_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
//	L_INFO("proc_query_test_config_data is called");
	const char* boardtype;
	const char* halfmaterialnum;
	bool displayhistoryeditflag;
	int machine_len = 0;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,s:s,s:b,*})", 
		"boardtype", &boardtype,
		"halfmaterialnum", &halfmaterialnum,
		"displayhistoryeditflag",&displayhistoryeditflag);
	if (displayhistoryeditflag)
	{
//		L_INFO("boardtype is: {0},halfmaterialnum is: {1}", boardtype, halfmaterialnum);
    ;
    }
	CheckIfFaultOccurredServer();

	xmlrpc_value* valueP = xmlrpc_array_new(envP);
	auto cb = [&envP, &valueP](int uuid, 
		const std::string& boardtype, 
		const std::string& halfmaterialnum, 
		const std::string& areaname,
		const std::string& machinename, 
		const std::string& usrname, 
		const std::string& edittime)
	{
		xmlrpc_value* v = xmlrpc_struct_new(envP);
		xmlrpc_set_int_v(v, uuid);
		xmlrpc_set_str_v(v, boardtype);
		xmlrpc_set_str_v(v, halfmaterialnum);
		xmlrpc_set_str_v(v, areaname);
		xmlrpc_set_str_v(v, machinename);
		xmlrpc_set_str_v(v, usrname);
		xmlrpc_set_str_v(v, edittime);

		xmlrpc_array_append_item(envP, valueP, v);
	};
    int ec /*= mysqlx_session_->query_test_config_data(boardtype, halfmaterialnum, displayhistoryeditflag,cb)*/;
	xmlrpc_struct_set_value(envP, dataP, "TestData", valueP);
	xmlrpc_DECREF(valueP);
	//return xmlrpc_build_value(envP, "{s:V}", "data", valueP);
	return build_ret_value(ec, dataP);
}

xmlrpc_value*
proc_get_test_details_db(
	xmlrpc_env* const envP,
	xmlrpc_value* const paramArrayP,
	void* const serverInfo,
	void* const channelInfo)
{
    Q_UNUSED(serverInfo);
    Q_UNUSED(channelInfo);
	CleanServerEnv();
	xmlrpc_value* dataP = xmlrpc_struct_new(envP);
//	L_INFO("proc_get_test_details is called");
	const char* uuid;
	xmlrpc_decompose_value(envP, paramArrayP, "({s:s,*})",
		"uuid", &uuid);
	CheckIfFaultOccurredServer();
	//xmlrpc_struct_set_value(envP, dataP, "name", xmlrpc_string_new(envP, name));
	auto cb = [&envP, &dataP](const std::string& testdetails)
	{
		xmlrpc_set_str_v(dataP, testdetails);
	};
    int ec/* = mysqlx_session_->get_test_details(uuid,cb)*/;
	/*xmlrpc_struct_set_value(envP, dataP, "TestDetails",dataP);
	xmlrpc_DECREF(dataP);*/
	return build_ret_value(ec, dataP);
}
