#ifndef DAAS_LOG_HPP
#define DAAS_LOG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <mutex>
#include <string>

enum class LogLevel {
    INFO,
    WARN,
    ERROR,
    DEBUG
};

class DaasLogger {
public:
    static DaasLogger& instance() {
        static DaasLogger logger;
        return logger;
    }

    void setLogFile(const std::string& path) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (ofs_.is_open()) ofs_.close();
        ofs_.open(path, std::ios::out | std::ios::app);
        file_enabled_ = ofs_.is_open();
    }

    void log(LogLevel level, const std::string& tag, const std::string& msg) {
        std::ostringstream line;
        line << "[" << timestamp() << "]"
             << "[" << tag << "]"
             << "[" << levelToString(level) << "] "
             << msg << std::endl;

        std::lock_guard<std::mutex> lock(mutex_);

        std::cout << line.str();
        if (file_enabled_) ofs_ << line.str();
    }

private:
    DaasLogger() : file_enabled_(false) {}
    ~DaasLogger() { if (ofs_.is_open()) ofs_.close(); }

    std::string timestamp() {
        using namespace std::chrono;
        auto now = system_clock::now();
        auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        auto in_time_t = system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S")
           << "." << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

    std::string levelToString(LogLevel lvl) {
        switch (lvl) {
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::DEBUG: return "DEBUG";
        }
        return "UNK";
    }

    std::mutex mutex_;
    std::ofstream ofs_;
    bool file_enabled_;
};

#endif // DAAS_LOG_HPP
