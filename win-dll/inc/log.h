#pragma once

#include <iostream>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#include "baseDll.h"

#ifdef _DEBUG
#pragma comment(lib, "log4cppD.lib")
#else
#pragma comment (lib, "log4cppLIB.lib")
#endif // _DEBUG
#pragma comment(lib, "ws2_32.lib")

class DLL_API Log
{
public:
	int open();
	void close();
	log4cpp::Category &getLog(std::string name = "NOTSET");
};