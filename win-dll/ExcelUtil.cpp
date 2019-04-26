#include "stdafx.h"
#include "ExcelUtil.h"
#include "strUtil.h"

#pragma comment(lib, "libxl.lib")

MakeExcel::MakeExcel()
{
	book = xlCreateBook();
	book->setKey(StrUtil::char2wchar("TommoT"), StrUtil::char2wchar("windows-2421220b07c2e10a6eb96768a2p7r6gc"));
}

MakeExcel::~MakeExcel()
{
	book->release();
}

bool MakeExcel::save(char *file_name)
{
	bool ret;
	wchar_t *p = StrUtil::char2wchar(file_name);
	ret = book->save(p);
	StrUtil::freeWchar(p);
	return ret;
}

MakeSheet::MakeSheet(libxl::Book * book, char *sheetName)
{
	wchar_t *p = StrUtil::char2wchar(sheetName);
	sheet = book->addSheet(p);
	StrUtil::freeWchar(p);
	font = book->addFont();
	font->setName(TEXT(""));
	font->setSize(11);
	format = book->addFormat();
	format->setFont(font);
}

MakeSheet::~MakeSheet()
{
}

void MakeSheet::writeCell(int row, int col, const char *c_str)
{
	writeCell(row, col, c_str, format);
}
void MakeSheet::writeCell(int row, int col, const char *c_str, libxl::Format *format)
{
	wchar_t *p = StrUtil::char2wchar(c_str);
	sheet->writeStr(row, col, p, format);
	StrUtil::freeWchar(p);
}
void MakeSheet::writeCell(int row, int col, int num)
{
	writeCell(row, col, num, format);
}
void MakeSheet::writeCell(int row, int col, int num, libxl::Format *format)
{
	sheet->writeNum(row, col, num, format);
}
void MakeSheet::writeCell(int row, int col, double num)
{
	writeCell(row, col, num, format);
}
void MakeSheet::writeCell(int row, int col, double num, libxl::Format *format)
{
	sheet->writeNum(row, col, num, format);
}
void MakeSheet::writeCell(int row, int col, long long num)
{
	writeCell(row, col, (double)num, format);
}
void MakeSheet::writeCell(int row, int col, long long num, libxl::Format *format)
{
	sheet->writeNum(row, col, (double)num, format);
}

void MakeSheet::makeField(std::vector<ExcelField> fileds)
{
	int i = 0;

	for (std::vector<ExcelField>::iterator iter = fileds.begin(); iter != fileds.end(); iter++, i++) {
		writeCell(0, i, iter->name);
	}
}

void MakeSheet::setPwd(const char * password)
{
	sheet->setProtect(true, StrUtil::char2wchar(password), libxl::PROT_ALL);
}
