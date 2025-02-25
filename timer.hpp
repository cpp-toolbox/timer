#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
  public:
    explicit Timer(double duration_seconds)
        : duration(std::chrono::duration<double>(duration_seconds)), start_time(), running(false) {}

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

  private:
    std::chrono::duration<double> duration;
    std::chrono::steady_clock::time_point start_time;
    bool running;
};

#endif // TIMER_HPP
