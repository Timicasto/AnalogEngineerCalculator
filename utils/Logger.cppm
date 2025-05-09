/**
******************************************************************************
  * @file    Logger.cppm
  * @author  Timicasto
  * @brief   A logger util class
  ******************************************************************************
  */

// SPDX-License-Identifier: AGPL-3.0-or-later

export module Logger;

import std;

export enum class LogLevel {
	FATAL, ERROR, WARNING, INFO, DEBUG
};

auto level = LogLevel::INFO;

std::string parseLogLevel(const LogLevel level) {
	switch (level) {
		case LogLevel::FATAL:
			return "[FATAL]";
		case LogLevel::ERROR:
			return "[ERROR]";
		case LogLevel::WARNING:
			return "[WARN] ";
		case LogLevel::INFO:
			return "[INFO] ";
		case LogLevel::DEBUG:
			return "[DEBUG]";
	}
	return "";
}
export namespace Logger {
	void log(const LogLevel level, const std::string& message) {
		auto time = std::chrono::system_clock::now();
		const std::string formattedTime = std::format("[{:%Y-%m-%d %H:%M:%S}]", time);
		const std::string formattedLevel = parseLogLevel(level);
		std::cout << formattedTime << " " << formattedLevel << " " << message << std::endl;
	}
}