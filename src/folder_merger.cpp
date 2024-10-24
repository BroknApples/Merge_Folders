#include "folder_merger.hpp"

/******************************************************************************
*********************************** PRIVATE ***********************************
******************************************************************************/ 

/**
 * @brief Check if a file is a valid file to add to an ordering_list
 *
 * @param file file to check
 * 
 * @return true if success ; false if error
 */
bool FolderMerger::isValidEntry(std::filesystem::path file) {
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
      return false;
    }
    else if (check_directory && std::filesystem::is_directory(path)) {
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
    std::cout << "Enter a valid backup folder name: ";
    
    std::cin >> std::noskipws;
    std::getline(std::cin, input);
    // check if file exists, or if there is not a directory with that name
  } while (!isValidPath(input, true) && input != "");

  return input;
}

/**
 * @brief create a file that will contain the index of all of the merged folder's contents BUT do not add anything to it yet
 * 
 * @return true if success ; false if error
 */
bool FolderMerger::createIndex() {
  std::filesystem::path backup_path = DEFAULT_BACKUP_PATH;

  // Check if path is a valid backup directory name
  if (!isValidPath(backup_path, true)) {
    backup_path = getValidBackupPath();
  }

  std::filesystem::create_directory(backup_path);

  return true;
}

bool FolderMerger::appendIndex(std::string_view str) {
  // create index some time earlier,
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
    std::cout << "Enter a valid backup folder name: ";
    
    std::cin >> std::noskipws;
    std::getline(std::cin, input);
    // check if file exists, or if there is not a directory with that name
  } while (!isValidPath(input, true) && input != "");

  return input;
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
  
  // Create backup, index, and rename files
  do {
    std::cout << "Enter a merge method(Enter only ONE):\n"
              << "FLAGNAME  |  RESULT\n"
              << "'ENTER':     Create a Backup and an Index\n"
              << "'-b':        Do not create a Backup, but create an Index\n"
              << "'-i':        Do not create a Index, but create an Backup\n"
              << "'-c':        Do not create a Backup or an Index\n"
              << "'-q':        Quit Program\n"
              << std::endl;

    std::string merge_method = "";
    std::cin >> merge_method;

    std::remove_if(merge_method.begin(), merge_method.end(), isspace);

    loop = false;
    switch(merge_method[1]) {
      case '\0': // Create Backup and Index
        createBackup(ordering_list);
        createIndex();
        break;

      case 'b': // No Backup
        createIndex();
        break;

      case 'i': // No Index
        createBackup(ordering_list);
        break;

      case 'c': // No Backup or Index
        break;

      case 'q': // Quit Program
        std::cout << "Quitting program." << std::endl;
        return;

      default:
        std::cout << "ERROR: incorrect entry: " << merge_method << std::endl;
        loop = true;
    }
  } while (loop);
}