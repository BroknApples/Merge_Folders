#include "folder_merger.hpp"

/******************************************************************************
*********************************** PRIVATE ***********************************
******************************************************************************/ 

std::filesystem::path getIndexFilename() {
  
}

std::filesystem::path getBackupFilename() {

}

void FolderMerger::printEntries(std::filesystem::path& directory) {
  for (const auto& file : std::filesystem::directory_iterator(directory)) {
    std::cout << file << "\n";
  }
  std::cout << std::endl;
}

bool FolderMerger::isValidEntry(std::filesystem::path entry) {
  // if file is in the exclude list, return false
  if (m_exclude_list.find(entry.filename()) != m_exclude_list.end()) {
    std::cout << entry.filename() << " is not a valid entry. Skipping.\n";
    return false;
  }

  return true;
}

bool isProperFormat(const std::string& str, const int max_length) {
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

/******************************************************************************
*********************************** PUBLIC ************************************
******************************************************************************/ 

FolderMerger::FolderMerger(std::filesystem::path main_directory)
  : m_main_directory(main_directory) {
  m_name = "FolderMerger.exe";
}

void FolderMerger::addToExcludeList(std::array<std::filesystem::path, EXCLUDE_SIZE>& exclude_list) {
  for (auto& file : exclude_list) {
    m_exclude_list[file]++;
  }
}

void FolderMerger::run() {
  bool running = true;
  while(running) {
    // TODO: run program here
  }
}