/**************************************************************************************************
 * Copyright (c) 2023. Dennis Oberst                                                              *
 **************************************************************************************************/

#include <TeensyThreads.h>
#include "Arduino.h"

#include <utility>
#include <chrono>
#include <string>
#include <sstream>

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
        static inline void info(const char *file, int line, Args&& ... args)
        {
            print_log("[INFO]   ", file, line, std::forward<Args>(args)...);
        }

        template <typename... Args>
        static inline void warning(const char *file, int line, Args&& ... args)
        {
            print_log("[WARNING]", file, line, std::forward<Args>(args)...);
        }

        template <typename... Args>
        static inline void error(const char *file, int line, Args&& ... args)
        {
            print_log("[ERROR]  ", file, line, std::forward<Args>(args)...);
        }

        template <typename... Args>
        static void debug(const char *file, int line, Args&& ... args)
        {
            print_log("[DEBUG]  ", file, line,
                      current_daytime(),
                      current_runtime(),
                      current_thread(),
                      std::forward<Args>(args)...);
        }

        static std::string current_daytime()
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

        static std::string current_thread()
        {
            std::ostringstream oss;
            oss << "tid: " << threads.id();
            return oss.str();
        }
    protected:
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
} // namespace lbs
