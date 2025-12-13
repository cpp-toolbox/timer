#include "timer.hpp"
#include <stdexcept>

Timer::Timer(double duration_seconds, bool start_immediately)
    : duration(std::chrono::duration<double>(duration_seconds)), start_time(), running(false) {

    if (duration_seconds <= 0.0) {
        throw std::invalid_argument("Timer duration must be positive");
    }

    if (start_immediately)
        this->start();
}

void Timer::start() {
    start_time = std::chrono::steady_clock::now();
    running = true;
}

bool Timer::time_up() const {
    if (!running)
        return false;
    return std::chrono::steady_clock::now() - start_time >= duration;
}

bool Timer::time_up_and_try_to_restart() {
    bool time_is_up = time_up();
    if (time_is_up) {
        start();
    }
    return time_is_up;
}

double Timer::get_remaining_time() const {
    if (!running)
        return duration.count(); // return the full duration, because the timer hasn't started yet.
    auto elapsed = std::chrono::steady_clock::now() - start_time;
    double remaining = duration.count() - std::chrono::duration<double>(elapsed).count();
    return remaining > 0.0 ? remaining : 0.0; // return zero if we've gone over budget
}

void Timer::change_duration(double duration_seconds) {
    if (duration_seconds <= 0.0)
        return; // duration must be strictly positive
    duration = std::chrono::duration<double>(duration_seconds);
}

double Timer::get_percent_complete() const {
    if (!running)
        return 0.0;
    auto elapsed = std::chrono::steady_clock::now() - start_time;
    double progress = std::chrono::duration<double>(elapsed).count() / duration.count();
    return progress < 1.0 ? progress : 1.0;
}
