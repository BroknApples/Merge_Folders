#include <iostream>
#include <cstdio>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>

bool IsProperFormat(const std::string& str, const int max_length);

std::vector<std::filesystem::path> PrintAndChangeOrder(std::vector<std::filesystem::path>& files);

bool CombineAndRename(std::vector<std::filesystem::path>& ordering_list);

int main() {
  std::filesystem::path directory = std::filesystem::current_path();
  const std::string kProgramFilename = "combine_folders.exe";
  const int kProgramFilenameLength = kProgramFilename.length();

  std::vector<std::filesystem::path> files;
  // search directory and add to list
  for (const auto& file : std::filesystem::directory_iterator(directory)) {
    // do not include program file in list
    std::string str = file.path().string();
    int temp_length = str.length();

    bool isProgramFile = true;
    for (int i = 0; i < kProgramFilenameLength; i++) {
      if (str[(temp_length - kProgramFilenameLength) + i] != kProgramFilename[i]) isProgramFile = false;
    }

    // don't add program file to list
    if (isProgramFile) continue;
    
    files.push_back(file.path());
  }

  std::vector<std::filesystem::path> ordering_list;
  bool is_valid = true;
  do {
    // Get files to include
    ordering_list = PrintAndChangeOrder(files);
    std::cout << std::endl;

    // List files and ask to confirm order
    std::cout << "File order: \n";
    int idx = 0;
    for (auto& file : ordering_list) {
      std::cout << idx << " - " << file << "\n";
      idx++;
    }
    std::cout << "Press '/' and press ENTER if this is correct, type 'any other character' and press ENTER to go back: ";
    std::string input = "";
    std::cin >> input;
    std::cout << std::endl;

    if (input != "0" && input.length() > 0) is_valid = false;
  } while (is_valid);

  bool ret = CombineAndRename(ordering_list);

  if (ret) {
    std::cout << "\nFiles Successfully renamed." << std::endl;

    std::cout << "Press 'any key' and press ENTER to close: ";
    std::string input;
    std::cin >> input;
  }

  return 0;
}

/*
  params: {str- string to check}
  returns: boolean value

  checks if the string meets all conditions
*/
bool IsProperFormat(const std::string& str, const int max_length) {
  if (str[0] == '/' && str.length() == 1) return true;

  const int kLength = str.length();
  int num_of_nums = 0;
  int curr_val = 0;
  std::unordered_map<int, int> seen;

  for (int i = 0; i < kLength; i++) {
    if (num_of_nums > max_length) {
      std::cout << "Too many numbers entered.\n" << std::endl;
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
            std::cout << "Do not enter a number more than once.\n" << std::endl;
            return false;
          }

          seen[curr_val]++;
          curr_val = 0;
        }
      }
      
      if (curr_val > max_length) {
        std::cout << curr_val << " is not the within range(0 - " << max_length << ").\n" << std::endl;
        return false;
      }

    } else if (str[i] != ' ') {
      // not a digit or space character, unexpected character entered
      std::cout << "Improper format.\n" << std::endl;
      return false;
    }
  }

  return true;
}

/*
  params: {files- all files in the current directory}
  returns: a ordering list of the files

  Lists all folders in the directory and prompts the user
  the order they wish to combine them in
*/
std::vector<std::filesystem::path> PrintAndChangeOrder(std::vector<std::filesystem::path>& files) {
  // Prompt user for order of files
  std::string order = "";
  const int kLength = files.size();
  do {
    // List all files
    std::cout << "Files in directory: " << std::endl;
    int idx = 0;
    for (auto& file : files) {
      std::cout << idx << " - " << file << "\n";
      idx++;
    }
    
    // Prompt user for input
    std::cout << "Enter the order of files using spaces(e.g. '0 1 6 2 5 3 4');\n"
              << "type '/' if the order is correct and all files should be included: ";
    std::cin >> std::ws;
    std::getline(std::cin, order);
  } while(!IsProperFormat(order, (kLength - 1)));

  // If string is empty, return original list
  if (order.length() == 1) return files;
  

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
bool CombineAndRename(std::vector<std::filesystem::path>& ordering_list) {
  // rename
  // use filesystem to get new list for each element ordering_list
  // then use names to rename...
  const int kListSize = ordering_list.size();
  std::string destination_folder = ordering_list[0].string();

  // get length and find smallest prefix of 0's to use
  int length = 0;
  
    for (const auto& curr_folder : ordering_list) {
    for (const auto& file : std::filesystem::directory_iterator(curr_folder)) {
      length++;
    }
  }

  // find how many leading zeroes there will be, e.g. 001 || 000001
  // only check to tens place to account for the guaranteed ones place
  int leading_zeroes = 0;
  while(length > 99) {
    length /= 10;
    leading_zeroes++;
  }

  int idx_num = 1;
  for (const auto& curr_folder : ordering_list) {
    for (const auto& file : std::filesystem::directory_iterator(curr_folder)) {
      std::string old_filename = file.path().string().c_str();
      std::string file_extension = file.path().extension().string();

      std::string zeroes = "";
      for (int i = 0; i < leading_zeroes; i++) {
        zeroes += '0';
      }

      std::string new_filename = ordering_list[0].string() + "\\" + zeroes
                                 + std::to_string(idx_num) + file_extension;
      idx_num++;

      std::cout << "Old filename: " << old_filename << "\nNew filename: " << new_filename << "\n";
      rename(old_filename.c_str(), new_filename.c_str());
    }

    // Delete empty folder if its not the first in the ordering_list
    if (curr_folder != ordering_list[0]) {
      std::error_code error_code;
      if (!std::filesystem::remove(curr_folder, error_code)) {
        std::cout << error_code.message() << std::endl;
      }
    }
  }

  return true;
}
