#ifndef CLOCK_HPP
#define CLOCK_HPP


#include <chrono>
#include <ctime>
#include <thread>


class Clock {
  private:
    /** The time in which the program starts. */
    // TODO: Make this hold only the actual time like 3:10PM 6/7/2025


  public:
    /** NOTE: Static Class */
    Clock() = delete;


    /**
     * @brief Setup the clock for usage
     */
    static void init();


    /**
     * @brief Equivalent to a sleep() function
     * 
     * Usage  ->   Clock::wait(100, std::chrono::milliseconds{});
     * 
     * @tparam Duration: A 'std::chrono::' duration value such as 'milliseconds' or 'microseconds'
     * @param seconds: Time to sleep the current thread for in seconds
     */
    template <typename Duration>
    static void wait(double time, Duration) {
      std::this_thread::sleep_for(std::chrono::duration<double>(time));
    }

    /** Helper methods for seconds, milliseconds, and microseconds */

    static void wait_s(double time) { wait(time, std::chrono::seconds{}); }
    static void wait_ms(double time) { wait(time, std::chrono::milliseconds{}); }
    static void wait_us(double time) { wait(time, std::chrono::microseconds{}); }
};


#endif // CLOCK_HPP
