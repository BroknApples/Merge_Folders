#include "folder_merger.hpp"

/******************************************************************************
*********************************** PRIVATE ***********************************
******************************************************************************/ 

/**
 * @brief Check if a file is a valid file to use in an ordering_list
 *
 * @param file file to check
 * 
 * @return true if success ; false if error
 */
bool FolderMerger::isValidOrderedListEntry(std::filesystem::path file) {
  // if file is in the exclude list, return false
  if (m_exclude_list.find(file.filename()) != m_exclude_list.end()) {
    std::cout << file.filename() << " is an excluded filename. Skipping.\n";
    return false;
  }
  else if (!std::filesystem::is_directory(file)) {
    std::cout << file.filename() << " is not a directory, cannot merge this file. Skipping.\n";
    return false;
  }

  return true;
}

/**
 * @brief Check if a string containing the order of files is in the proper format
 *
 * @param str string to check
 * @param max_length maximum integer value possible for a valid string
 * 
 * @return true if success ; false if error
 */
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

/**
 * @brief Get a list of every child in a directory
 *
 * @param directory directory to get children from
 * 
 * @return std::vector<std::filesystem::path> containing path elements
 */
std::vector<std::filesystem::path> FolderMerger::getDirectoryEntries(std::filesystem::path directory) {
  std::vector<std::filesystem::path> ret;
  for (const auto& file : std::filesystem::directory_iterator(directory)) {
    ret.push_back(file);
  }
  
  return ret;
}

/**
 * @brief Print all entries in a vector containing filesystem paths
 *
 * @param directory holds a list of path variables to print
 */
void FolderMerger::printEntries(std::vector<std::filesystem::path>& directory) {
  for (int i = 0; i < directory.size(); i++) {
    std::cout << i << " - " << directory[i] << "\n";
  }
  std::cout << std::endl;
}

/**
 * @brief Get a list in the correct order of the folders of the elements to merge
 *
 * @param entries a list containing all of the entries in the current directory
 * 
 * @return std::vector<std::filesystem::path> containing the elements in the correct order
 */
std::vector<std::filesystem::path> FolderMerger::getOrderingList(std::vector<std::filesystem::path>& entries) {
  // Prompt user for order of files
  std::string input;
  const int kLength = entries.size();
  do {
    printEntries(entries);
    std::cout << "Enter the order of files using spaces(e.g. '0 1 6 2 5 3 4');\n"
              << "Press 'ENTER' if the order is correct and all files should be included: "
              << std::endl;
              
    std::cout << "ENTER: ";
    std::cin >> std::noskipws;
    std::getline(std::cin, input);
  } while (!isProperFormat(input, (kLength - 1)));
  

  // Create ordering list
  std::vector<std::filesystem::path> ordering_list;
  ordering_list.reserve(kLength); // reserve k many spaces
  int num_elements = 0;

  const int kListSize = input.length();
  
  
  if (input == "") { // if string is empty return original list minus the excluded items
    for (auto& file : entries) {
      if (isValidOrderedListEntry(file)) {
        ordering_list.push_back(file);
      }
    }
  }
  else { // if string is not empty add only the numbered items
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

      if (isValidOrderedListEntry(entries[val])) {
        ordering_list.push_back(entries[val]);
      }
      num_elements++;
      val = 0;
    }

    ordering_list.resize(num_elements); // resize list to correct size
  }

  return ordering_list;
}

/**
 * @brief Checks if a path contains a valid path to some folder
 *
 * @param ordering_list the vector of paths to copy
 * @param check_directory is the parth param a directory
 * 
 * @return true if success ; false if error occurred
 */
bool FolderMerger::isValidPath(std::filesystem::path path, bool check_directory) {
  if (std::filesystem::exists(path)) {
    if (!check_directory) {
      std::cout << "Path: " << path.filename() << " already exists." << std::endl;
      return false;
    }
    else if (check_directory && std::filesystem::is_directory(path)) {
      std::cout << "Path: " << path.filename() << " already exists." << std::endl;
      return false;
    }
  }

  return true;
}

/**
 * @brief Creates a backup folder containing all of a std::vector<std::filesystem::path> variable's original content
 *
 * @param ordering_list the vector of paths to copy
 * 
 * @return true if success ; false if error occurred
 */
bool FolderMerger::createBackup(std::vector<std::filesystem::path>& ordering_list) {
  std::filesystem::path backup_path = DEFAULT_BACKUP_PATH;

  // Check if path is a valid backup directory name
  if (!isValidPath(backup_path, true)) {
    backup_path = getValidBackupPath();
  }

  std::filesystem::create_directory(backup_path);
  // copy all data to backup  
  for (auto& file : ordering_list) {
    // new_filename will be : path-to-parent/backup_path/file.extension
    std::filesystem::path new_filename = (m_main_directory / backup_path / "/" / file);
    std::filesystem::copy(file, new_filename);
  }

  return true;
}

/**
 * @brief Get a valid name for a backup directory
 *
 * @return std::filesystem::path which holds the valid backup path name
 */
std::filesystem::path FolderMerger::getValidBackupPath() {
  std::string input;
  do {
    std::cout << "Enter a valid backup folder name: " << std::endl;

    std::cout << "ENTER: ";
    std::cin >> std::noskipws;
    std::getline(std::cin, input);
    // check if file exists, or if there is not a directory with that name
  } while (!isValidPath(input, true) && input != "");

  return input;
}

/**
 * @brief Get a valid name for a index directory
 *
 * @return std::filesystem::path which holds the valid index path name
 */
std::filesystem::path FolderMerger::getValidIndexPath() {
  // if the default path is valid, simply return it
  if (isValidPath(DEFAULT_INDEX_PATH, true)) {
    return DEFAULT_INDEX_PATH;
  }

  std::string input;
  do {
    std::cout << "Enter a valid backup folder name: ";
    
    std::cout << "ENTER: ";
    std::cin >> std::noskipws;
    std::getline(std::cin, input);
    // check if file exists, or if there is not a directory with that name
  } while (!isValidPath(input, true) && input != "");

  return input;
}

/**
 * @brief Merge all files in the proper order with the correct name
 * 
 * @param ordering_list list containing directories to be merged together
 * @param index_file path to index file that contains the start pages for the merged folders DEFAULT = "" ~ empty
 * 
 * @return true if succes ; false if error
 */
bool FolderMerger::merge(std::vector<std::filesystem::path>& ordering_list, std::filesystem::path& index_file = std::filesystem::path("")) {
  std::filesystem::path destination_folder = ordering_list[0];

  // get length to find smallest prefix of 0's to use
  int length = 0;
  for (const auto& folder : ordering_list) {
    for (const auto& file : std::filesystem::directory_iterator(folder)) {
      if (std::filesystem::is_directory(file)) {
        std::cout << "Folder detected in " << folder << ", would you like to skip or quit(Enter: "
                  << QUIT_FLAG << " to quit or enter: 'any key' to skip)." << std::endl;
        
        std::string input = "";
        std::cout << "ENTER: ";
        std::getline(std::cin, input);

        if (input == QUIT_FLAG) {
          return false;
        }
      }
      else if (m_exclude_list.find(file) != m_exclude_list.end()) {
        std::cout << file.path().filename() << " is not a valid entry. Skipping.\n";
      }

      length++;
    }
  }  

  // find how many leading zeroes there will be, e.g. 001.png or 000001.pdf or 1.txt
  int leading_zeroes = 0;
  while(length > 9) {
    length /= 10;
    leading_zeroes++;
  }

  int idx_num = 1;
  int folder_idx = 0;
  int ten_multiple = 10;
  for (const auto& curr_folder : ordering_list) {
    bool first_loop = true;
    for (const auto& file : std::filesystem::directory_iterator(curr_folder)) {
      if (m_exclude_list.find(file) != m_exclude_list.end()) {
        std::cout << file.path().filename() << " is not a valid entry. Skipping.\n";
      }

      // Remove a leading zero
      if (idx_num % ten_multiple == 0) { // divisible by some multiple of ten
        ten_multiple *= 10;
        leading_zeroes--;
      }

      // Add zeroes to string
      std::string temp_zeroes = "";
      for (int i = 0; i < leading_zeroes; i++) {
        temp_zeroes += '0';
      }
      std::filesystem::path zeroes(temp_zeroes);
      std::filesystem::path filename(std::to_string(idx_num) + file.path().extension().string()); // filename e.g. 001.txt
      std::filesystem::path new_file = (destination_folder / zeroes / filename);

      idx_num++;




      // TODO: appendIndex in this function and delete folder





      if (first_loop) {
        // Add to index file
        appendIndex(curr_folder, idx_filename, (zeroes + std::to_string(idx_num - 1)), folder_idx);
        first_loop = false;
      }

      // print and rename
      std::cout << "Old filename: " << file.path().filename() << "\nNew filename: " << new_file.filename() << "\n";
      std::filesystem::rename(file.path(), new_file);
    }

    // Delete empty folder if its not the first in the ordering_list
    if (curr_folder.filename() != ordering_list[0].filename()) {
      std::error_code error_code;
      if (!std::filesystem::remove(curr_folder, error_code)) {
        std::cout << error_code.message() << std::endl;
      } else {
        std::cout << "Deleted folder:  " << curr_folder.filename() << "\n";
      }
    }

    std::cout << std::endl;

    folder_idx++;
  }

  return true;
}

/******************************************************************************
*********************************** PUBLIC ************************************
******************************************************************************/ 

/**
 * @brief The only constructor for a FolderMerger instnace
 *
 * @param main_directory path to the directory that will will merge folders
 */
FolderMerger::FolderMerger(std::filesystem::path main_directory)
  : m_main_directory(main_directory) {
  m_name = "fmerge.exe";
}

/**
 * @brief Get a list of filenames to exclude from merging
 */
void FolderMerger::getCustomExcludes() {
  std::cout << "Enter files that should be excluded from parsing(enter " + QUIT_FLAG + " to end input): " << std::endl;
  std::string input = "";
  while (input != QUIT_FLAG) {
    if ((input != QUIT_FLAG) && (input != "")) {
      m_exclude_list[input]++;
    }

    std::cout << "ENTER: ";
    std::cin >> std::noskipws;
    std::getline(std::cin, input);
  }
}

/**
 * @brief Add a list of filenames to exclude from merging, Optional: Default exclude list is empty
 *
 * @param exclude_list list of files to exclude
 */
void FolderMerger::addToExcludeList(const std::vector<std::filesystem::path>& exclude_list) {
  for (auto& file : exclude_list) {
    m_exclude_list[file]++;
  }
}

/**
 * @brief run a FolderMerger instance
 */
void FolderMerger::run() {
  std::vector<std::filesystem::path> main_dir_files = getDirectoryEntries(m_main_directory);

  // get a vector containing the correct order of folders
  std::vector<std::filesystem::path> ordering_list;
  bool loop;
  do {
    ordering_list = getOrderingList(main_dir_files);

    std::cout << "\nCurrent Order: " << std::endl;
    printEntries(ordering_list);
    std::cout << "Press 'any key' and press 'ENTER' to go back, or enter "
              << "nothing and press 'ENTER' if this is the correct order: " << std::endl;

    std::cout << "ENTER: ";
    std::string input = "";
    std::getline(std::cin, input);
    
    if (input != "") {
      loop = true;
    }
    else {
      loop = false;
    }
  } while(loop);
  
  // Create backup, index, and rename files
  do {
    std::cout << "Enter a merge method(Enter only ONE):\n"
              << "FLAGNAME  |  RESULT\n"
              << "'ENTER':     Create a Backup and an Index\n"
              << "'-b':        Do NOT create a Backup, but DO create an Index\n"
              << "'-i':        Do NOT create an Index, but DO create a Backup\n"
              << "'-c':        Do NOT create a Backup and do NOT create an Index\n"
              << "'-q':        Quit Program\n"
              << std::endl;

    std::cout << "ENTER: ";
    std::string merge_method = "";
    std::getline(std::cin, merge_method);

    std::remove_if(merge_method.begin(), merge_method.end(), isspace);

    loop = false;
    switch(merge_method[1]) {
      case '\0':  { // Create Backup and Index
        createBackup(ordering_list);
        std::filesystem::path index_path = getValidIndexPath();
        merge(ordering_list, index_path);
        break;
      }
      case 'b': { // No Backup
        std::filesystem::path index_path = getValidIndexPath();
        merge(ordering_list, index_path);
        break;
      }
      case 'i': { // No Index
        createBackup(ordering_list);
        merge(ordering_list);
        break;
      }
      case 'c': { // No Backup or Index
      merge(ordering_list);
        break;
      }
      case 'q': { // Quit program
        std::cout << "Closing program." << std::endl;
        return;
      }
      default: { // Invalid input
        std::cout << "ERROR: incorrect entry: " << merge_method << std::endl;
        loop = true;
      }
    }
  } while (loop);
}