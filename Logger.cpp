#include "Logger.h"
//super cool not at all bad logger

void Log(enum LogType logType, std::string thread, std::string message, std::string printarg, bool filteraction)
{
	if (filteraction) {
		CLogV_Hook(logType, (char*)thread.c_str(), NULL, NULL, 2, NULL, message.c_str(), printarg.c_str());
	}
	else {
		CLogV_Hook(logType, (char*)thread.c_str(), NULL, 0, NULL, 1, message.c_str(), printarg.c_str());
	}
}

void Log(enum LogType logType, std::string thread, std::string message, std::string printarg)
{
	CLogV_Hook(logType, (char*)thread.c_str(), NULL, NULL, 2, NULL, message.c_str(), printarg.c_str());
}

void Log(enum LogType logType, std::string thread, std::string message)
{
	CLogV_Hook(logType, (char*)thread.c_str(), NULL, NULL, 2, NULL, message.c_str(), NULL);
}

void Log(enum LogType logType, std::string message)
{
	CLogV_Hook(logType, (char*)"DLML", NULL, NULL, 2, NULL, message.c_str(), NULL);
}

void Log(std::string message, std::string printarg)
{
	CLogV_Hook(LogType::INFO, (char*)"DLML", NULL, NULL, 2, NULL, message.c_str(), printarg.c_str());
}

void Log(std::string message)
{
	CLogV_Hook(LogType::INFO, (char*)"DLML", NULL, NULL, 2, NULL, message.c_str(), NULL);
}
