#pragma once
#include <sstream>
#include <vector>
#include <timeapi.h>
#include "baseDll.h"
#include "sqlite3.h"

#pragma comment( lib,"winmm.lib" )

#define DATABASE_ROOT					"database"

class DLL_API TableField
{
public:
	int cid;
	char name[128];
	char type[64];
	int notnull;
	int pk;
};

class DLL_API SqliteUtil
{
private:
	sqlite3 *db;

	inline std::string wrap_db_name()
	{
		std::ostringstream tmp_name;

		tmp_name << DATABASE_ROOT << "\\" << db_name << ".db";
		return tmp_name.str();
	}
	inline std::string wrap_bak_name()
	{
		std::ostringstream tmp_name;

		tmp_name << DATABASE_ROOT << "\\" << db_name << "_" << timeGetTime() << ".db";
		return tmp_name.str();
	}

public:
	std::string db_name;

	int open(std::string name);
	int simple_open(std::string name);
	void close();
	int backup(unsigned int reserve);
	int create_tab(const char *sql);
	int drop_tab(const char *tab_name);
	int isexists_tab(const char *tab_name);
	int count_tab_data(const char *sql);
	int opt_tab_data(const char *sql);
	int del_tab_all_data(const char *tab_name);
	int sqlite3_cmd(const char *cmd);
	inline int _sqlite3_exec(const char *sql, int (*callback)(void *para, int columnCount, char **columnValue, char **columnName), void *para, char **errMsg);
	inline int _sqlite3_get_table(const char *sql, char ***pResult, int *rowCount, int *columnCount, char **errMsg);
	void _sqlite3_free_table(char **result);
	void _sqlite3_free(void *errmsg);
	std::vector<TableField> query_tab_field(const char *tab_name);
};