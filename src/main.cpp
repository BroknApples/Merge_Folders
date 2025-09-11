/**
 * 
 * TODO: Add info here...
 * 
 * 
 * 
 * 
 */

#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "core/config.hpp"
#include "core/filesystem.hpp"
#include "misc/clock.hpp"
#include "misc/interval_timer.hpp"
#include "misc/utils.hpp"

/**
 * @brief Sets general parameters and starts the program
 */
int main() {
  // TESTING

  //char path[MAX_PATH];
  //GetModuleFileNameA(NULL, path, MAX_PATH);
  //printf("Executable path: %s\n", path);

  // Initialize 'globals'
  Config::init();
  Filesystem::init();


  println("[Test1]");
  Filesystem::setCurrentDirectory(Config::getString(Config::Keys::DEFAULT_DIRECTORY));
  std::vector<std::filesystem::path> dirs = Filesystem::getFilesInCurrentDirectory();
  println("\nGathered Paths: ");
  for (const auto path : dirs) {
    println("Path: ", path);
  }
  println("");


  if (dirs.size() > 0) {
    println("[Test2]");
    println("Using path: '", dirs[0], "'");
    Filesystem::setCurrentDirectory(dirs[0]);
    std::vector<std::filesystem::path> dirs2 = Filesystem::getFilesInCurrentDirectory();
    println("\nGathered Paths: ");
    for (const auto path : dirs2) {
      println("Path: ", path);
    }
    println("");
  }
  

  // // TESTING
  // IntervalTimer t;
  // while (true) {
  //   print("Enter seconds to wait: ");
  //   double time;
  //   std::cin >> time;

  //   t.start();
  //   Clock::wait_s(time);
  //   t.end();

  //   println(std::to_string(t.diff_ms()) + "ms");
    
  //   t.reset();
  // }


  return 0;
}