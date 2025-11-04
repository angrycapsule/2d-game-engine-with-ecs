#include "Logger.h"

#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

#define ERROR_COLOR "\x1B[91m"
#define INFO_COLOR "\x1B[32m"
#define DEFAULT_COLOR "\033[0m"

std::vector<LogEntry> Logger::messages;

void Logger::Log(const std::string& message)
{
	LogEntry logEntry;
	logEntry.type = LOG_INFO;
	logEntry.message = "LOG: [" + GetCurrentDateTimeToString() + "]: " + message;
	std::cout << INFO_COLOR << logEntry.message << DEFAULT_COLOR << std::endl;

	messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
	LogEntry logEntry;
	logEntry.type = LOG_ERROR;
	logEntry.message = "LOG: [" + GetCurrentDateTimeToString() + "]: " + message;
	std::cout << ERROR_COLOR << logEntry.message << DEFAULT_COLOR << std::endl;

	messages.push_back(logEntry);
}

std::string Logger::GetCurrentDateTimeToString()
{
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string output(30, '\0');
	std::strftime(&output[0], output.size(), "%d-%b_%Y %H:%M:%S", std::localtime(&now));
	return output;
}
