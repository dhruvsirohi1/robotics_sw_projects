//
// Created by Dhruv Sirohi on 8/24/25.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <queue>
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
    std::condition_variable cv_;
    std::FILE* file_ = nullptr;
    std::queue<LogMsg> queue_;
    std::atomic<std::size_t> dropped_{0};

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

    bool log(Level level, const std::string &text) {
        if (level < config_.min_level) return true;  // msg filtered out
        LogMsg msg;
        msg.level = level;
        msg.text = std::move(text);
        msg.ts = std::chrono::system_clock::now();
        msg.tid = get_tid();

        if (!running_.load(std::memory_order_acquire)) start();

        std::unique_lock<std::mutex> lock(logger_mutex_);
        if (config_.block_when_full) {
            cv_.wait(lk, [&] { return stopping_ || queue_.size() < config_.queue_capacity; });
            if (stopping_) return false;
            queue_.push(std::move(msg));
            lock.unlock();
            cv_.notify_one();
            return true;
        } else {
            if (queue_.size() >= config_.queue_capacity) {
                dropped_.fetch_add(1, std::memory_order_relaxed);
                return false;
            }
        }
        queue_.push(std::move(msg));
        lock.unlock();
        cv_.notify_one();
        return true;
    }

    // Convenience shortcuts
    bool trace(const std::string& s){ return log(Level::TRACE,s); }
    bool debug(const std::string& s){ return log(Level::DEBUG,s); }
    bool info (const std::string& s){ return log(Level::INFO ,s); }
    bool warn (const std::string& s){ return log(Level::WARN ,s); }
    bool error(const std::string& s){ return log(Level::ERROR,s); }
    bool fatal(const std::string& s){ return log(Level::FATAL,s); }

private:
    void run() {
        std::vector<LogMsg> batch;
        batch.reserve(1024);

        while (true) {
            std::unique_lock<std::mutex> lock(logger_mutex_);
            cv_.wait(lock, [&] { return stopping_ || !queue_.empty();});
            if (stopping_ && queue_.empty()) break;
            while (!queue_.empty() && batch.size() < 1024) {
                batch.push_back(std::move(queue_.front()));
                queue_.pop();
            }

            for (auto& msg : batch) {
                // TODO: write_one(msg)

            }
            batch.clear();
            std::this_thread::yield();
        }

        TODO: Drain any leftover messages
    }

};
}
#endif //LOGGER_HPP
