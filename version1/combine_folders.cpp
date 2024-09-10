#include <iostream>
#include <cstdio>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>

bool IsProperFormat(const std::string& str, const int max_length);

std::vector<std::filesystem::path> PrintAndChangeOrder(std::vector<std::filesystem::path>& files);

void CombineAndRename(std::vector<std::filesystem::path>& ordering_list);

int main() {
  std::filesystem::path directory = std::filesystem::current_path();

  std::vector<std::filesystem::path> files;
  // search directory and add to list
  for (const auto& file : std::filesystem::directory_iterator(directory)) {
    // do not include program file in list
    if (file.path() == "combine_folders.exe") continue;
    
    files.push_back(file.path());
  }
  std::vector<std::filesystem::path> ordering_list;
  ordering_list = PrintAndChangeOrder(files);

  for (auto& file : ordering_list) {
    std::cout << file << "\n";
  }

  CombineAndRename(ordering_list);

  return 0;
}

/*
  params: {str- string to check}
  returns: boolean value

  checks if the string meets all conditions
*/
bool IsProperFormat(const std::string& str, const int max_length) {
  const int kLength = str.length();
  int num_of_nums = 0;
  int curr_val = 0;
  std::unordered_map<int, int> seen;

  for (int i = 0; i < kLength; i++) {
    if (num_of_nums > max_length) {
      std::cout << "Too many numbers entered." << std::endl;
      return false;
    }

    if (isdigit(str[i])) {
      num_of_nums++;

      // Check if the number is even in the allowed amount
      curr_val *= 10;
      curr_val += str[i] - '0';

      // check if next idx is a number, if so, dont increase length variable
      if (i != kLength - 1) {
        if (isdigit(str[i + 1])) {
          continue;
        }
        else {
          // if number has already been entered
          if (seen.find(curr_val) != seen.end()) {
            std::cout << "Do not enter a number more than once." << std::endl;
            return false;
          }

          seen[curr_val]++;
          curr_val = 0;
        }
      }
      
      if (curr_val > max_length) {
        std::cout << curr_val << " is not the within range(0 - " << max_length << ")." << std::endl;
        return false;
      }

    } else if (str[i] != ' ') {
      // not a digit or space character, unexpected character entered
      std::cout << "Improper format." << std::endl;
      return false;
    }
  }

  std::cout << "returning true: " << curr_val << std::endl;

  return true;
}

/*
  params: {files- all files in the current directory}
  returns: a ordering list of the files

  Lists all folders in the directory and prompts the user
  the order they wish to combine them in
*/
std::vector<std::filesystem::path> PrintAndChangeOrder(std::vector<std::filesystem::path>& files) {
  // list all files
  int idx = 0;
  for (auto& file : files) {
    std::cout << idx << " - " << file << "\n";
    idx++;
  }

  // Prompt user for order of files
  std::string order;
  const int kLength = files.size();
  do {
    std::cout << "Enter the order of files using spaces(e.g. 0 1 6 2 5 3 4),"
              << " if nothing is entered, order is assumed to be correct: ";
    std::getline(std::cin, order);
  } while(!IsProperFormat(order, kLength));
  

  // Parse files into ordering list
  const int kListSize = order.length();
  std::vector<std::filesystem::path> ordering_list;

  int val = 0;
  for (int i = 0; i < kListSize; i++) {
    if (order[i] == ' ') continue;

    val *= 10;
    val += order[i] - '0';

    // multi-digit numbers
    if (isdigit(order[i + 1])) continue;

    // Add file at corresponding index to end of ordering list
    ordering_list.push_back(files[val]);
  }

  return ordering_list;
}

/*
  params: {ordering_list: filenames in order of renaming}
  returns: void

  takes a list containing the order of files to combine and
  creates a folder that contains the combined files
*/
void CombineAndRename(std::vector<std::filesystem::path>& ordering_list) {
  // rename
  // use filesystem to get new list for each element ordering_list
  // then use names to rename...
  const int kListSize = ordering_list.size();
  std::string destination_folder = ordering_list[0].string();


  // get length and find smallest prefix of 0's to use
  std::string last_idx = "1";
  int length = 0;
  
  for (const auto& curr_folder : ordering_list) {
    for (const auto& file : std::filesystem::directory_iterator(curr_folder)) {
      length++;
    }
  }

  int num_digits = 0;
  while(length > 9) {
    length /= 10;
    num_digits++;
  }

  for (const auto& curr_folder : ordering_list) {
    for (const auto& file : std::filesystem::directory_iterator(curr_folder)) {
      const char* old_filename = "";
      old_filename = file.path().string().c_str();

      const char* new_filename = "TODO: add renaming method";

      // rename(old_filename, new_filename);
    }
  }
}
