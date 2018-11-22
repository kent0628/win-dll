#include "stdafx.h"
#include <Shlwapi.h>
#include "FileUtil.h"
#include "strUtil.h"

#pragma comment(lib, "shlwapi.lib")

bool FileUtil::mkdir(std::string dir)
{
	wchar_t *p = StrUtil::str2wchar(dir);
	if (!PathIsDirectory(p)) {
		if (!CreateDirectory(p, NULL)) {
			StrUtil::freeWchar(p);
			return false;
		}
	}
	StrUtil::freeWchar(p);
	return true;
}