#include "folder_merger.hpp"

/******************************************************************************
*********************************** PRIVATE ***********************************
******************************************************************************/ 

// Check user input
bool FolderMerger::isValidEntry(std::filesystem::path file) {
  // if file is in the exclude list, return false
  if (m_exclude_list.find(file.filename()) != m_exclude_list.end()) {
    std::cout << file.filename() << " is an excluded filename. Skipping.\n";
    return false;
  }

  return true;
}

bool FolderMerger::isProperFormat(std::string_view str, const int max_length) {
  const int kLength = str.length();
  int num_of_nums = 0;
  int curr_val = 0;
  std::unordered_map<int, int> seen;

  for (int i = 0; i < kLength; i++) {
    if (num_of_nums > max_length) {
      // current number is greater than the length of the array
      std::cout << "ERROR: Too many numbers entered.\n" << std::endl;
      return false;
    }
    else if (!isdigit(str[i]) && str[i] != ' ') {
      // not a digit or space character: unexpected character entered
      std::cout << "ERROR: Improper format.\n" << std::endl;
      return false;
    }
    else if (str[i] == ' ') {
      // skip space chars
      continue; 
    }
    else {
      // check if next idx is a number, if so, dont increase length variable
      if (isdigit(str[i + 1])) {
        continue;
      }

      num_of_nums++;

      curr_val *= 10;
      curr_val += str[i] - '0';

      if (curr_val > max_length) {
        // value exceeds maximum value allowed
        std::cout << "ERROR: " << curr_val << " is not the within range(0 - " << max_length << ").\n" << std::endl;
        return false;
      }
      else if (seen.find(curr_val) != seen.end()) {
        // if number has already been entered
        std::cout << "ERROR: Do not enter a number more than once.\n" << std::endl;
        return false;
      }
      else {
        seen[curr_val]++;
        curr_val = 0;
      }
    }
  }

  return true;
}

// General use
std::vector<std::filesystem::path> FolderMerger::getDirectoryEntries(std::filesystem::path directory) {
  std::vector<std::filesystem::path> ret;
  for (const auto& file : std::filesystem::directory_iterator(directory)) {
    ret.push_back(file);
  }
  
  return ret;
}

void FolderMerger::printEntries(std::vector<std::filesystem::path>& directory) {
  for (int i = 0; i < directory.size(); i++) {
    std::cout << i << " - " << directory[i] << "\n";
  }
  std::cout << std::endl;
}

// Main working functions

std::vector<std::filesystem::path> FolderMerger::getOrderingList(std::vector<std::filesystem::path>& entries) {
  // Prompt user for order of files
  std::string input;
  const int kLength = entries.size();
  do {
    printEntries(entries);
    std::cout << "Enter the order of files using spaces(e.g. '0 1 6 2 5 3 4');\n"
              << "Press 'ENTER' if the order is correct and all files should be included: "
              << std::endl;
    std::cin >> std::noskipws;
    std::getline(std::cin, input);
  } while (!isProperFormat(input, (kLength - 1)));
  
  // If string is empty, return original list
  if (input == "") {
    return entries;
  }


  // Create ordering list
  std::vector<std::filesystem::path> ordering_list;
  ordering_list.reserve(kLength); // reserve k many spaces
  int num_elements = 0;

  const int kListSize = input.length();
  int val = 0;
  for (int i = 0; i < kListSize; i++) {
    if (input[i] == ' ') {
      continue;
    }

    val *= 10;
    val += input[i] - '0';

    // multi-digit numbers
    if (isdigit(input[i + 1])) {
      continue;
    }

    if (isValidEntry(entries[val])) {
      ordering_list.push_back(entries[val]);
    }
    num_elements++;
    val = 0;
  }

  ordering_list.resize(num_elements); // resize list to correct size
  return ordering_list;
}


bool FolderMerger::createBackup(std::vector<std::filesystem::path>& ordering_list, std::filesystem::path& curr_directory) {

}

void FolderMerger::getValidBackupPath() {
  while(false) {

  }
}

bool FolderMerger::appendIndex(std::string_view str) {
  return true;
}

void FolderMerger::getValidIndexPath() {
  while(false) {

  }
}

/******************************************************************************
*********************************** PUBLIC ************************************
******************************************************************************/ 

FolderMerger::FolderMerger(std::filesystem::path main_directory)
  : m_main_directory(main_directory) {
  m_name = "FolderMerger.exe";
}

void FolderMerger::getCustomExcludes() {
  std::cout << "Enter files that should be excluded from parsing(enter " + QUIT_FLAG + " to end input): ";
  std::string input = "";
  while (input != QUIT_FLAG) {
    if ((input != QUIT_FLAG) && (input != "")) {
      m_exclude_list[input]++;
    }
    std::getline(std::cin, input);
  }
}

/* Optional: Default exclude list is empty */
void FolderMerger::addToExcludeList(const std::vector<std::filesystem::path>& exclude_list) {
  for (auto& file : exclude_list) {
    m_exclude_list[file]++;
  }
}

void FolderMerger::run() {
  std::vector<std::filesystem::path> main_dir_files = getDirectoryEntries(m_main_directory);

  // get a vector containing the correct order of folders
  std::vector<std::filesystem::path> ordering_list;
  bool loop;
  do {
    ordering_list = getOrderingList(main_dir_files);

    std::cout << "\nCurrent Order: " << std::endl;
    printEntries(ordering_list);
    std::cout << "Press 'any key' and press 'ENTER' to go back, "
              << "or enter nothing and press 'ENTER' if this is the correct order: "
              << std::endl;
    std::string input = "";
    std::cin >> input;
    std::cin.ignore();
    
    if (input != "") {
      loop = true;
    }
    else {
      loop = false;
    }
  } while(loop);
  
  // TODO: Create backup, index and rename files
}