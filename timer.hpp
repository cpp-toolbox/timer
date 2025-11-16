#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

/**
 * @class Timer
 * @brief A simple timer utility for measuring elapsed time.
 *
 * The `Timer` class provides a straightforward way to measure a fixed duration.
 * Once started, it tracks elapsed time and allows checking whether the timer has
 * expired, retrieving remaining time, or determining the percent completion.
 *
 * Example usage:
 * @code
 * // Create a 3-second timer, but don't start it yet
 * Timer timer(3.0);
 *
 * // Do some setup work before starting
 * std::cout << "Preparing..." << std::endl;
 * std::this_thread::sleep_for(std::chrono::seconds(1));
 *
 * // Start the timer manually
 * timer.start();
 *
 * // Run until the timer expires
 * while (!timer.time_up()) {
 *     std::cout << "Remaining time: " << timer.get_remaining_time() << "s\r" << std::flush;
 *     std::this_thread::sleep_for(std::chrono::milliseconds(100));
 * }
 * std::cout << "\nTimer finished!" << std::endl;
 *
 * // Restart the timer for another interval
 * timer.start();
 * std::cout << "Timer restarted." << std::endl;
 * @endcode
 */
class Timer {
  public:
    /**
     * @brief Construct a new Timer with a specified duration.
     *
     * @param duration_seconds The duration of the timer in seconds.
     * @param start_immediately If true, the timer starts immediately upon construction.
     *
     * If `start_immediately` is false (default), you must call start() manually.
     */
    explicit Timer(double duration_seconds, bool start_immediately = false)
        : duration(std::chrono::duration<double>(duration_seconds)), start_time(), running(false) {
        if (start_immediately)
            this->start();
    }

    /**
     * @brief Start or restart the timer.
     *
     * Resets the timer's start time to the current moment and marks it as running.
     * You must call this function before using time_up(), get_remaining_time(), or get_percent_complete().
     */
    void start() {
        start_time = std::chrono::steady_clock::now();
        running = true;
    }

    /**
     * @brief Check whether the timer duration has elapsed.
     *
     * @return true if the timer has been started and the elapsed time >= duration.
     * @return false if the timer has not been started or is still running.
     */
    bool time_up() const {
        if (!running)
            return false;
        return std::chrono::steady_clock::now() - start_time >= duration;
    }

    /**
     * @brief Get the remaining time before the timer expires.
     *
     * @return Remaining time in seconds.
     * - Returns the full duration if the timer hasn’t started.
     * - Returns 0 if the timer has expired.
     */
    double get_remaining_time() const {
        if (!running)
            return duration.count();
        auto elapsed = std::chrono::steady_clock::now() - start_time;
        double remaining = duration.count() - std::chrono::duration<double>(elapsed).count();
        return remaining > 0.0 ? remaining : 0.0;
    }

    /**
     * @brief Get the completion percentage of the timer.
     *
     * @return A value between 0.0 and 1.0 representing how much of the duration has elapsed.
     * - Returns 0.0 if the timer hasn’t started.
     * - Returns 1.0 once the duration has been reached or exceeded.
     */
    double get_percent_complete() const {
        if (!running)
            return 0.0;
        auto elapsed = std::chrono::steady_clock::now() - start_time;
        double progress = std::chrono::duration<double>(elapsed).count() / duration.count();
        return progress < 1.0 ? progress : 1.0;
    }

  private:
    /** @brief The total duration of the timer. */
    std::chrono::duration<double> duration;

    /** @brief The time point when the timer was started. */
    std::chrono::steady_clock::time_point start_time;

    /** @brief Whether the timer is currently running. */
    bool running;
};

#endif // TIMER_HPP
