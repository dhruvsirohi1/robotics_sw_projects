//
// Created by Dhruv Sirohi on 8/24/25.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include <thread>

namespace logger {

enum class Level : int {TRACE, DEBUG, INFO, WARN, ERROR, FATAL};

inline const char* to_cstr(Level L) {
    switch (L) {
        case Level::TRACE: return "TRACE";
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return "INFO";
        case Level::WARN: return "WARN";
        case Level::ERROR: return "ERROR";
        case Level::FATAL: return "FATAL";
    }
    return "? Unrecognized Logger level ?";
}

struct Config {
    std::string file_path = "robot.log";
    std::size_t max_bytes = 8 * 1024 * 1024;
    int max_files = 3;
    std::size_t queue_capacity = 8 * 1024;
    bool block_when_full = false;
    Level min_level = Level::INFO;
    bool also_stderr = true;
};

struct LogMsg {
    Level level;
    std::string text;
    std::chrono::system_clock::time_point ts;
    std::uint64_t tid;
};

class AsyncLogger {

    Config config_;
    std::atomic<bool> running_;
    std::thread worker_;
    bool stopping_ = false;
    std::mutex logger_mutex_;
    bool stopping_ = false;
    std::condition_variable cv_;
    std::FILE* file_ = nullptr;

public:

    explicit AsyncLogger(const Config &cfg = {}) : config_(std::move(cfg)) {};

    void start() {
        if (running_.exchange(true)) return;
        open_log_file();
        worker_ = std::thread(&AsyncLogger::run, this);
    }

    void stop() {
        if (!running_.exchange(false)) return;
        {
            std::lock_guard<std::mutex> lock(logger_mutex_);
            stopping_ = true;
        }
        cv_.notify_all();
        if (worker_.joinable()) worker_.join();
        if (file_) std::fclose(file_);
    }

    ~AsyncLogger() { stop(); }


};
}
#endif //LOGGER_HPP
