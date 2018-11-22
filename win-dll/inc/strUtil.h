#pragma once
#include <string>

class StrUtil {
public:
	static wchar_t *char2wchar(const char *str);
	static void freeWchar(wchar_t *p);
	static wchar_t *str2wchar(const std::string str);
	static char *wchar2char(const wchar_t *str);
	static void freeChar(char *p);
};
