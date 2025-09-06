#ifndef LOGGER_HPP
#define LOGGER_HPP


#include <iostream>
#include <vector>
#include <string>


/**
 * @brief 
 */
class Logger {
  private:
    std::vector<std::string> _buffer;
  
  public:
    Logger() = delete;



    /**
     * @brief Print a string without a newline
     * @param str: String to print
     */
    static void print(const std::string& str) {
      std::cout << str;
    }

    /**
     * @brief Print a string along with a newline (clears the buffer as well)
     * @param str: String to print
     */
    static void println(const std::string& str) {
      std::cout << str << std::endl;
    }
};









#endif // LOGGER_HPP