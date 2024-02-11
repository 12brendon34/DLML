#pragma once
#include <cstdarg>
#include <stdio.h>
#include <string>
#include "Menu.h"
#include "Hooks.h"

enum LogType {
	DEBG = 5,
	WARN = 4,//prob warning?? not actually 100% sure
	INFO = 3,
	ERRR = 2
};

void Log(enum LogType logType, std::string thread, std::string message, std::string printarg, bool filteraction = true);
void Log(enum LogType logType, std::string thread, std::string message, std::string printarg);
void Log(enum LogType logType, std::string thread, std::string message);
void Log(enum LogType logType, std::string message, std::string printarg);
void Log(enum LogType logType, std::string message);
void Log(std::string message, std::string printarg);
void Log(std::string message);