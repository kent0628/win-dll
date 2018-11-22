#include "stdafx.h"
#include <vector>
#include <string.h>
#include "SqliteUtil.h"
#include "FileUtil.h"


int SqliteUtil::open(std::string name)
{
	this->db_name = name;
	FileUtil::mkdir(DATABASE_ROOT);
	return simple_open(wrap_db_name());
}

int SqliteUtil::simple_open(std::string name)
{
	int ret;

	ret = sqlite3_open(name.c_str(), &db);
	if (ret != SQLITE_OK) {
		db = NULL;
		return -1;
	}
	return 0;
}

void SqliteUtil::close()
{
	if (db) {
		sqlite3_close(db);
	}
}

int SqliteUtil::backup(unsigned int reserve)
{
#if 0
	CFileFind find;
	std::vector<std::string> files;
	std::ostringstream tmp;

	tmp << DATABASE_ROOT << "\\" << db_name << "_*.db" << std::endl;
	BOOL ret = find.FindFile(tmp.str().c_str());
	while (ret) {
		ret = find.FindNextFile();
		if (find.IsDots() || find.IsDirectory()) {
			continue;
		}
		files.push_back(find.GetFileName().GetBuffer());
	}
	if (files.size() >= reserve) {
		tmp << DATABASE_ROOT << "\\" << files.front() << std::endl;
		DeleteFile(tmp.str().c_str());
	}
	// 备份数据库
	if (CopyFile(wrap_db_name().c_str(), wrap_bak_name().c_str(), FALSE)) {
		return 0;
	} else {
		return -1;
	}
#endif
	return 0;
}

int SqliteUtil::create_tab(const char *sql)
{
	char *errmsg = NULL;
	int ret;

	ret = _sqlite3_exec(sql, NULL, NULL, &errmsg);
	if (ret != SQLITE_OK) {
		_sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}

int SqliteUtil::drop_tab(const char *tab_name)
{
	char *errmsg = NULL;
	int ret;
	char sql[64];

	sprintf_s(sql, "drop table if exists %s", tab_name);
	ret = _sqlite3_exec(sql, NULL, NULL, &errmsg);
	if (ret != SQLITE_OK) {
		_sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}

int SqliteUtil::isexists_tab(const char *tab_name)
{
	char sql[1024];

	sprintf_s(sql, "select count(*) from sqlite_master where type = 'table' and name = '%s'", tab_name); 
	return count_tab_data(sql);
}

int SqliteUtil::count_tab_data(const char *sql)
{
	int ret;
	int row, col, count = 0;
	char **result;
	char *errmsg = NULL;

	ret = _sqlite3_get_table(sql, &result, &row, &col, &errmsg);
	if (ret != SQLITE_OK) {
		_sqlite3_free_table(result);
		_sqlite3_free(errmsg);
		return -1;
	}
	if (row > 0 || col > 0) {
		count = atoi(result[1]);
	}
	_sqlite3_free_table(result);
	return count;
}

int SqliteUtil::opt_tab_data(const char *sql)
{
	return sqlite3_cmd(sql);
}

int SqliteUtil::del_tab_all_data(const char *tab_name)
{
	char *errmsg = NULL;
	int ret;
	char sql[64];

	sprintf_s(sql, "delete from %s", tab_name);
	ret = _sqlite3_exec(sql, NULL, NULL, &errmsg);
	if(ret != SQLITE_OK){
		_sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}

int SqliteUtil::sqlite3_cmd(const char *cmd)
{
	char *errmsg = NULL;
	int ret;

	ret = _sqlite3_exec(cmd, NULL, NULL, &errmsg);
	if(ret != SQLITE_OK){
		_sqlite3_free(errmsg);
		return -1;
	}
	return 0;
}

int SqliteUtil::_sqlite3_exec(const char *sql, int (*callback)(void *para, int columnCount, char **columnValue, char **columnName), void *para, char **errMsg)
{
	int ret;

	sqlite3_busy_timeout(db, 30 * 1000); // 最长等待30m
	ret = sqlite3_exec(db, sql, callback, para, errMsg);
	return ret;
}

int SqliteUtil::_sqlite3_get_table(const char *sql,char ***pResult, int *rowCount, int *columnCount, char **errMsg)
{
	int ret;

	sqlite3_busy_timeout(db, 30 * 1000); // 最长等待30m
	ret = sqlite3_get_table(db, sql, pResult, rowCount, columnCount, errMsg);
	return ret;
}

void SqliteUtil::_sqlite3_free_table(char **result)
{
	sqlite3_free_table(result);
}

void SqliteUtil::_sqlite3_free(void *errmsg)
{
	sqlite3_free(errmsg);
}

std::vector<TableField> SqliteUtil::query_tab_field(const char * tab_name)
{
	int ret, i, j;
	int row = 0, col = 0, index;
	char **result;
	char *errmsg = NULL;
	char sql[1024];
	std::vector<TableField> fields;

	sprintf_s(sql, "pragma table_info(%s);", tab_name);
	ret = _sqlite3_get_table(sql, &result, &row, &col, &errmsg);
	if (ret != SQLITE_OK) {
		_sqlite3_free_table(result);
		_sqlite3_free(errmsg);
		return fields;
	}

	index = col;
	for (i = 0; i < row; i++) {
		TableField field;
		for (j = 0; j < col; j++) {
			if (!strcmp("cid", result[j])) {
				field.cid = atoi(result[index++]);
			}
			else if (!strcmp("name", result[j])) {
				strcpy_s(field.name, result[index++]);
			}
			else if (!strcmp("type", result[j])) {
				strcpy_s(field.type, result[index++]);
			}
			else if (!strcmp("notnull", result[j])) {
				field.notnull = atoi(result[index++]);
			}
			else if (!strcmp("pk", result[j])) {
				field.pk = atoi(result[index++]);
			}
			else {
				result[index++];
			}
		}
		fields.push_back(field);
	}
	_sqlite3_free_table(result);
	return fields;
}
