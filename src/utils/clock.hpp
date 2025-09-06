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
     * @param seconds: Time to sleep the current thread for in seconds
     */
    static inline void wait(float seconds) {
      std::this_thread::sleep_for(std::chrono::duration<float>(seconds));
    }
    
};


#endif // CLOCK_HPP