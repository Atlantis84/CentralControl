#pragma once
#define PROC_ACTS() \
    PROC_ACT(add_testplan) \
    PROC_ACT(tcon_testplan_exist) \
    PROC_ACT(check_recipe_trunk) \
    PROC_ACT(save_recipe)\
    PROC_ACT(query_recipe) \
    PROC_ACT(query_recipe_trunk)\
    PROC_ACT(recipe_size)\
    PROC_ACT(check_recipe_update) \
    PROC_ACT(update_recipe) \
    PROC_ACT(update_recipe_machine) \
    PROC_ACT(query_testplan2) \
    PROC_ACT(new_testplan) \
    PROC_ACT(check_tv_recipe_update) \
    PROC_ACT(query_testplan_trunk) \
    PROC_ACT(update_testplan_size) \
    PROC_ACT(update_testplan_trunk) \
    PROC_ACT(test_json) \
    PROC_ACT(query_test_config_data) \
    PROC_ACT(get_test_details)

#define PROC_ACTS_DB() \
    PROC_ACT_DB(add_testplan_db) \
    PROC_ACT_DB(tcon_testplan_exist_db) \
    PROC_ACT_DB(check_recipe_trunk_db) \
    PROC_ACT_DB(save_recipe_db)\
    PROC_ACT_DB(query_recipe_db) \
    PROC_ACT_DB(query_recipe_trunk_db)\
    PROC_ACT_DB(recipe_size_db)\
    PROC_ACT_DB(check_recipe_update_db) \
    PROC_ACT_DB(update_recipe_db) \
    PROC_ACT_DB(update_recipe_machine_db) \
    PROC_ACT_DB(query_testplan2_db) \
    PROC_ACT_DB(new_testplan_db) \
    PROC_ACT_DB(check_tv_recipe_update_db) \
    PROC_ACT_DB(query_testplan_trunk_db) \
    PROC_ACT_DB(update_testplan_size_db) \
    PROC_ACT_DB(update_testplan_trunk_db) \
    PROC_ACT_DB(test_json_insert_db) \
    PROC_ACT_DB(query_test_config_data_db) \
    PROC_ACT_DB(get_test_details_db)
