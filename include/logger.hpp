/**************************************************************************************************
 * Copyright (c) 2023. Dennis Oberst                                                              *
 **************************************************************************************************/
#include <usb_serial.h>
#include <TeensyThreads.h>

#include <ctime>
#include <string>
#include <chrono>
#include <sstream>
#include <utility>

namespace lbs
{

#ifdef LOG
#define LOG_INFO(...) Logger::info(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) Logger::warning(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) Logger::error(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) Logger::debug(__FILE__, __LINE__, __VA_ARGS__)
#else // Expand to nothing for performance boost.
#define LOG_INFO(...)
#define LOG_WARNING(...)
#define LOG_ERROR(...)
#define LOG_DEBUG(...)
#endif

class Logger
{
public:
	template <typename... Args>
	static void info(const char *file, int line, Args&& ... args)
	{
		print_log("[INFO]   ", file, line, std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void warning(const char *file, int line, Args&& ... args)
	{
		print_log("[WARNING]", file, line, std::forward<Args>(args)...);
	}
	template <typename... Args>
	static void error(const char *file, int line, Args&& ... args)
	{
		print_log("[ERROR]  ", file, line, std::forward<Args>(args)...);
	}

	template <typename... Args>
	static void debug(const char *file, int line, Args&& ... args)
	{
		print_log("[DEBUG]  ", file, line, date_time(), current_runtime(), std::forward<Args>(args)...);
	}

	static std::string date_time()
	{
		auto now = std::chrono::system_clock::now();
		auto now_time = std::chrono::system_clock::to_time_t(now);
		auto now_tm = std::localtime(&now_time);
		char timestamp[20];
		strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", now_tm);
		return {timestamp};
	}

	static std::string current_runtime()
	{
		auto now = std::chrono::system_clock::now();
		auto active_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
		std::ostringstream oss;
		oss << "runtime " << active_time.count() << "ms";
		return oss.str();
	}

private:
	template <typename... Args>
	static void print_log(const char *prefix, Args&& ... args)
	{
		m_mutex.lock();
		std::stringstream ss;
		ss << prefix << " ";
		((ss << std::forward<Args>(args) << ", "), ...);
		Serial.println(ss.str().c_str());
		m_mutex.unlock();
	}

private:
	static std::mutex m_mutex;
	static const std::chrono::system_clock::time_point start_time;
};

std::mutex Logger::m_mutex;

const std::chrono::system_clock::time_point Logger::start_time = std::chrono::system_clock::now();

} // namespace lbs
