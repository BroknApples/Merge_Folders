/**
 * 
 * TODO: Add info here...
 * 
 * 
 * 
 * 
 */

#include <iostream>
#include "misc/clock.hpp"
#include "misc/interval_timer.hpp"
#include "misc/utils.hpp"

/**
 * @brief Sets general parameters and starts the program
 */
int main() {
  IntervalTimer t;
  
  // TESTING
  while (true) {
    print("Enter seconds to wait: ");
    double time;
    std::cin >> time;

    t.start();
    Clock::wait_s(time);
    t.end();

    println(std::to_string(t.diff_ms()) + "ms");
    
    t.reset();
  }


  return 0;
}