#include "stdafx.h"
#include "log.h"
#include "FileUtil.h"

int Log::open()
{
	try {
		FileUtil::mkdir("logs");
		log4cpp::PropertyConfigurator::configure("log4cpp.properties");
	}
	catch (log4cpp::ConfigureFailure &f) {
		f.what();
		return -1;
	}
	return 0;
}

void Log::close()
{
}

log4cpp::Category &Log::getLog(std::string name)
{
	return log4cpp::Category::getRoot().getInstance(name);
}