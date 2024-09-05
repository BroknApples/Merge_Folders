#include <iostream>
#include <cstdio>
#include <filesystem>
#include <vector>

bool IsProperFormat(std::string& str);

std::vector<std::filesystem::path> PrintAndChangeOrder(std::vector<std::filesystem::path>& files);

void Combine(std::vector<std::filesystem::path>& ordering_list);

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

  Combine(ordering_list);

  return 0;
}

/*
  params: {str- string to check}
  returns: boolean value

  checks if the string meets all conditions
*/
bool IsProperFormat(std::string& str) {
  const int kLength = str.length();
  for (int i = 0; i < kLength; i++) {
    if      (str[i] == '0') continue;
    else if (str[i] == '1') continue;
    else if (str[i] == '2') continue;
    else if (str[i] == '3') continue;
    else if (str[i] == '4') continue;
    else if (str[i] == '5') continue;
    else if (str[i] == '6') continue;
    else if (str[i] == '7') continue;
    else if (str[i] == '8') continue;
    else if (str[i] == '9') continue;
    else if (str[i] == ' ') continue;
    else                    return false;
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
  // list all files
  int idx = 0;
  for (auto& file : files) {
    std::cout << idx << " - " << file << "\n";
    idx++;
  }

  // Prompt user for order of files
  std::string order;
  do {
    std::cout << "Enter the order of files using spaces(e.g. 0 1 6 2 5 3 4),"
              << " if nothing is entered, order if assumed to be correct: ";
    std::cin >> order;
  } while(!IsProperFormat(order));
  

  // Parse files into ordering list
  const int kListSize = order.length();
  std::vector<std::filesystem::path> ordering_list;
  for (int i = 0; i < kListSize; i++) {
    if (order[i] == ' ') continue;

    // Add file at corresponding index to end of ordering list
    ordering_list.push_back(files[order[i]]);
  }

  return ordering_list;
}

/*
  params: {ordering_list: filenames in order of renaming}
  returns: void

  takes a list containing the order of files to combine and
  creates a folder that contains the combined files
*/
void Combine(std::vector<std::filesystem::path>& ordering_list) {

}
