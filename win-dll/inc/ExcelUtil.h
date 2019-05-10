#pragma once
#include <string>
#include <vector>
#include "strUtil.h"
#include "xl\libxl.h"

#include "baseDll.h"

class DLL_API ExcelField {
public:
	char name[128];
	char type[64];
};

class DLL_API MakeSheet {
public:
	char sheetName[128];
	libxl::Sheet *sheet;
	libxl::Font *font;
	libxl::Format *format;
	libxl::Book *book;

	MakeSheet(libxl::Book *book, char *sheetName);
	~MakeSheet();

	void writeCell(int row, int col, const char *c_str);
	void writeCell(int row, int col, const char *c_str, libxl::Format *format);
	void writeCell(int row, int col, int num);
	void writeCell(int row, int col, int num, libxl::Format *format);
	void writeCell(int row, int col, double num);
	void writeCell(int row, int col, double num, libxl::Format *format);
	void writeCell(int row, int col, long long num);
	void writeCell(int row, int col, long long num, libxl::Format *format);

	void makeField(std::vector<ExcelField> fileds);

	void setAutoFitArea(int rowFirst = 0, int colFirst = 0, int rowLast = -1, int colLast = -1);

	void setPwd(const char *password);
};

class DLL_API MakeExcel {
public:
	libxl::Book *book;

	MakeExcel();
	~MakeExcel();

	bool save(char *file_name);
};