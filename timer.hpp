#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
  public:
    explicit Timer(double duration_seconds, bool start_immediately = false)
        : duration(std::chrono::duration<double>(duration_seconds)), start_time(), running(false) {
        if (start_immediately)
            this->start();
    }

    // NOTE: you have to run start or else nothing will occur
    void start() {
        start_time = std::chrono::steady_clock::now();
        running = true;
    }

    bool time_up() const {
        if (!running)
            return false;
        return std::chrono::steady_clock::now() - start_time >= duration;
    }

    double get_remaining_time() const {
        if (!running)
            return duration.count();
        auto elapsed = std::chrono::steady_clock::now() - start_time;
        double remaining = duration.count() - std::chrono::duration<double>(elapsed).count();
        return remaining > 0.0 ? remaining : 0.0;
    }

    double get_percent_complete() const {
        if (!running)
            return 0.0;
        auto elapsed = std::chrono::steady_clock::now() - start_time;
        double progress = std::chrono::duration<double>(elapsed).count() / duration.count();
        return progress < 1.0 ? progress : 1.0;
    }

  private:
    std::chrono::duration<double> duration;
    std::chrono::steady_clock::time_point start_time;
    bool running;
};

#endif // TIMER_HPP
