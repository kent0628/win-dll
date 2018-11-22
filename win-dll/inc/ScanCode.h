#pragma once
#include <string>
#include "baseDll.h"

class DLL_API ScanCode{
public:
	static int readCode(MSG *pMsg, std::string &data);
};

