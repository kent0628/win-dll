#include "stdafx.h"
#include "strUtil.h"

wchar_t *StrUtil::char2wchar(const char *str)
{
	int iSize;
	wchar_t *p;

	iSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	p = (wchar_t *)malloc(iSize * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, -1, p, iSize);
	return p;
}

wchar_t *StrUtil::str2wchar(const std::string str)
{
	return char2wchar(str.c_str());
}

void StrUtil::freeWchar(wchar_t *p)
{
	if (p != NULL) {
		free(p);
		p = NULL;
	}
}

char *StrUtil::wchar2char(const wchar_t *str)
{
	int iSize;
	char *p;

	iSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	p = (char *)malloc(iSize * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, str, -1, p, iSize, NULL, NULL);
	return p;
}

void StrUtil::freeChar(char *p)
{
	if (p != NULL) {
		free(p);
		p = NULL;
	}
}
