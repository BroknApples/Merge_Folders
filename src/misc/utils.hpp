#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <type_traits>


/***********************************************************
******************* beg Printing Methods *******************
***********************************************************/


/**
 * @brief Traits to check if T can be sent to std::cout
 * NOTE: Def got this from ChatGPT, I don't know about these <type_traits> techniques yet
 */
template <typename T, typename = void>
struct is_streamable : std::false_type {};

template <typename T>
struct is_streamable<T, std::void_t<decltype(std::cout << std::declval<T>())>> : std::true_type {};


/** 
 * Single argument print.
 * 
 * @brief Print a value without a newline
 * @param val: Value to print
 */
template <typename T>
typename std::enable_if<is_streamable<T>::value>::type
print(const T& value) {
  std::cout << value;
}


/**
 * Fallback for non-streamable types
 */ 
template <typename T>
typename std::enable_if<!is_streamable<T>::value>::type
print(const T&) {
  std::cout << "[Unsupported Type]";
}


/** 
 * Variadic print
 * 
 * @brief Print a value without a newline
 * @param val: Value to print
 */
template <typename T, typename... Args>
void print(const T& first, const Args&... args) {
  print(first);
  if constexpr (sizeof...(args) > 0) {
    print(args...);
  }
}


/** 
 * Single argument print.
 * 
 * @brief Print a value along with a newline (clears the buffer as well)
 * @param val: Value to print
 */
template <typename T>
typename std::enable_if<is_streamable<T>::value>::type
println(const T& value) {
  std::cout << value << std::endl;
}


/**
 * Fallback for non-streamable types
 */ 
template <typename T>
typename std::enable_if<!is_streamable<T>::value>::type
println(const T&) {
  std::cout << "[Unsupported Type]" << std::endl;
}


/** 
 * Variadic print.
 * 
 * @brief Print a value along with a newline (clears the buffer as well)
 * @param val: Value to print
 */
template <typename T, typename... Args>
void println(const T& first, const Args&... args) {
  println(first);
  if constexpr (sizeof...(args) > 0) {
    println(args...);
  }
}


/***********************************************************
******************* end Printing Methods *******************
***********************************************************/


#endif // UTILS_HPP
