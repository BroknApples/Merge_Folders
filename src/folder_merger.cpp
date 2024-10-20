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
  // TODO: Copied function from version 1, Check if this 
  // needs to be reformatted after version 2 is published

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
      if (i != (kLength - 1)) {
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
      // not a digit or space character: unexpected character entered
      std::cout << "Improper format.\n" << std::endl;
      return false;
    }
  }

  return true;
}

// General use
std::vector<std::filesystem::path> getDirectoryEntries(std::filesystem::path directory) {
  std::vector<std::filesystem::path> ret;
  for (const auto& file : std::filesystem::directory_iterator(directory)) {
    ret.push_back(file);
  }
  
  return ret;
}
void FolderMerger::printEntries(std::filesystem::path& directory) {
  // Check if folder is not a directory
  std::error_code error_code;
  if (!std::filesystem::is_directory(directory, error_code)) { 
    return;
  }
  if (error_code) {
    std::cerr << "Error checking if " << directory 
              << "is a directory: " << error_code.message();
  }

  for (const auto& file : std::filesystem::directory_iterator(directory)) {
    std::cout << file << "\n";
  }
  std::cout << std::endl;
}

// For Backups and Indexing
bool FolderMerger::createBackup(std::vector<std::filesystem::path>& ordering_list, std::filesystem::path& curr_directory) {
  return true;
}

bool FolderMerger::appendIndex(std::string_view str) {
  return true;
}

void FolderMerger::getValidBackupPath() {
  while(false) {

  }
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
  std::cout << "Enter files that should be excluded from parsing(enter " + QUIT_FLAG + " to quit): ";
  std::string input = "";
  while (input != QUIT_FLAG) {
    if ((input != QUIT_FLAG) && (input != "")) {
      m_exclude_list[input]++;
    }
    std::getline(std::cin, input);
  }
}

/* Optional: Default exclude list is empty */
void FolderMerger::addToExcludeList(std::vector<std::filesystem::path>& exclude_list) {
  for (auto& file : exclude_list) {
    m_exclude_list[file]++;
  }
}

void FolderMerger::run() {
  std::vector<std::filesystem::path> main_dir_files = getDirectoryEntries(m_main_directory);
}