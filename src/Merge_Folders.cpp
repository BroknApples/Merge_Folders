#include <iostream>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

namespace fs = std::filesystem;

bool IsProperFormat(const std::string& str, const int max_length);

std::vector<fs::path> PrintAndChangeOrder(std::vector<fs::path>& files);

bool CreateBackup(std::vector<fs::path>& ordering_list, fs::path& curr_directory);

std::string GetIndexName(fs::path curr_directory);

void AddToIndexFile(fs::path folder, std::string idx_filename, std::string new_filename, int folder_idx);

bool CombineAndRename(std::vector<fs::path>& ordering_list, fs::path curr_directory, std::string idx_filename);

int main() {
  fs::path curr_directory = fs::current_path();
  const std::string kProgramFilename = "Merge_Folders.exe";
  const std::string kDesktopINI = "desktop.ini";
  const int kProgramFilenameLength = kProgramFilename.length();

  std::vector<fs::path> files;
  // Search directory and add to list
  for (const auto& file : fs::directory_iterator(curr_directory)) {
    // Do not add program file to list
    if (file.path().filename() == kProgramFilename || file.path().filename() == kDesktopINI) continue;
    
    files.push_back(file.path());
  }

  std::vector<fs::path> ordering_list;
  bool is_valid = true;
  while (is_valid) {
    // Get files to include
    ordering_list = PrintAndChangeOrder(files);
    std::cout << std::endl;

    // List files and ask to confirm order
    std::cout << "File order: \n";
    int idx = 0;
    for (auto& file : ordering_list) {
      std::cout << idx << " - " << file.filename() << "\n";
      idx++;
    }
    std::cout << "Press '/' and press ENTER if this is correct, type 'any other character' and press ENTER to go back: ";
    std::cin >> std::ws;
    std::string input = "";
    std::getline(std::cin, input);
    std::cout << std::endl;

    if (input != "0" && input.length() > 0) is_valid = false;
  }

  if (ordering_list.size() == 1) {
    std::cout << "No files changed. Only one item in list.\n";
    std::cout << "Press 'any key' and press ENTER to close: ";
    std::string input;
    std::cin >> input;
    return 0;
  }

  bool success;

  if (CreateBackup(ordering_list, curr_directory)) {
    std::cout << "\nBackup successfully created.\n" << std::endl;
  } else {
    std::cout << "Press 'any key' and press ENTER to close: ";
    std::string input;
    std::cin >> input;
    return 0;
  }

  std::string filename = GetIndexName(curr_directory);

  if (CombineAndRename(ordering_list, curr_directory, filename)) {
    std::cout << "\nFiles Successfully renamed." << std::endl;

    std::cout << "Press 'any key' and press ENTER to close: ";
    std::string input;
    std::cin >> input;
  } else {
    std::cout << "Press 'any key' and press ENTER to close: ";
    std::string input;
    std::cin >> input;
    return 0;
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
std::vector<fs::path> PrintAndChangeOrder(std::vector<fs::path>& files) {
  // Prompt user for order of files
  std::string order = "";
  const int kLength = files.size();
  do {
    // List all files
    std::cout << "Files in directory: " << std::endl;
    int idx = 0;
    for (auto& file : files) {
      std::cout << idx << " - " << file.filename() << "\n";
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
  std::vector<fs::path> ordering_list;

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
  params: {files- files to create backups of}
  returns: true/false of its success

  creates a backup of the folders used
*/
bool CreateBackup(std::vector<fs::path>& ordering_list, fs::path& curr_directory) {
  // Check if folder with name already exists
  std::string backup_name = "Backup";
  fs::path backup_folder_path = curr_directory.string() + "\\" + backup_name;
  bool named = false;
  if (!fs::exists(backup_folder_path)) {
    named = true;
  }

  while (!named) {
    // Prompt user for backup folder name
    std::cout << "Folder with name ('" << backup_name << "') already exists in directory,\n"
              << "please enter a different name for the backup folder: ";
    std::cin >> std::ws;
    std::getline(std::cin, backup_name);

    backup_folder_path = curr_directory.string() + "\\" + backup_name;

    if (!fs::exists(backup_folder_path)) {
      named = true;
    }
  }

  fs::create_directory(backup_folder_path);

  for (auto& folder : ordering_list) {
    // Reset backup folder path
    fs::path backup_path_temp = backup_folder_path;

    // Check if file is a folder or not
    if (!fs::is_directory(folder)) {
      std::cout << "ERROR: " << folder << " is not a directory." << std::endl;
      std::cout << "Please remove " << folder << " and try again." << std::endl;
      return false;
    }

    backup_path_temp += "\\" + folder.stem().string();

    fs::copy(folder, backup_path_temp);
  }

  return true;
}

/*
  params: {curr_directory- parent directory}
  returns: string containing name of idx file

  Get name of index file
*/
std::string GetIndexName(fs::path curr_directory) {
  // Prompt user for filename of inddex file, if and only if there is already one with deafult name
  bool named = false;
  std::string filename = "Index";
  std::string file_path = curr_directory.string() + "\\" + filename + ".txt";
  if (!fs::exists(file_path)) {
    named = true;
  }

  while (!named) {
    // Prompt user for backup folder name
    std::cout << "File with name ('" << filename << "') already exists in directory,\n"
              << "please enter a different name for the index file: ";
    std::cin >> std::ws;
    std::getline(std::cin, filename);

    file_path = curr_directory.string() + "\\" + filename + ".txt";

    if (!fs::exists(file_path)) {
      named = true;
    }
  }

  return filename;
}

/*
  params: {curr_directory- parent directory}
  returns: void

  print to indexing file
*/
void AddToIndexFile(fs::path folder, std::string idx_filename, std::string new_filename, int folder_idx) {
  std::ofstream ofStream;
  
  // Open with appending permission
  ofStream.open(idx_filename + ".txt", std::ios_base::app);
  if (!ofStream.is_open()) {
    std::cout << "Error creating index file." << std::endl;
    return;
  } else {
    std::cout << "Index file created." << std::endl;
  }

  ofStream << folder_idx << " - '" << folder.stem().string() << "'" << std::endl;
  ofStream << "Starts on the file named '" << new_filename << ".txt'\n" << std::endl;

  ofStream.close();
}

/*
  params: {ordering_list: filenames in order of renaming} {curr_directory- parent directory} {idx_filename- filename of indexing file}
  returns: boolean of the success of the proccess

  takes a list containing the order of files to combine and
  creates a folder that contains the combined files
*/
bool CombineAndRename(std::vector<fs::path>& ordering_list, fs::path curr_directory, std::string idx_filename) {
  const int kListSize = ordering_list.size();
  std::string destination_folder = ordering_list[0].string();

  // get length to find smallest prefix of 0's to use
  int length = 0;
  for (const auto& curr_folder : ordering_list) {
    for (const auto& file : fs::directory_iterator(curr_folder)) {
      if (fs::is_directory(file)) {
        std::cout << "Folder detected in " << curr_folder << ", please remove and try again." << std::endl;
        return false;
      }
      length++;
    }
  }  

  // find how many leading zeroes there will be, e.g. 001 || 000001
  int leading_zeroes = 0;
  while(length > 9) {
    length /= 10;
    leading_zeroes++;
  }

  int idx_num = 1;
  int folder_idx = 0;
  for (const auto& curr_folder : ordering_list) {
    bool first_loop = true;
    for (const auto& file : fs::directory_iterator(curr_folder)) {
      std::string old_filename = file.path().string();
      std::string file_extension = file.path().extension().string();

      // Remove a certain amount of leading zeroes
      int temp = idx_num;
      int remove_zeroes = 0;
      while(temp > 9) {
        temp /= 10;
        remove_zeroes++;
      }

      // Add zeroes to string
      std::string zeroes = "";
      for (int i = 0; i < leading_zeroes - remove_zeroes; i++) {
        zeroes += '0';
      }

      std::string new_filename = ordering_list[0].string() + "\\" + zeroes
                                 + std::to_string(idx_num) + file_extension;
      idx_num++;

      std::string print_temp1 = curr_folder.filename().string() + "\\" + file.path().filename().string();
      std::string print_temp2 = ordering_list[0].filename().string() + "\\" + std::to_string(idx_num) + file_extension;

      std::cout << "Old filename: " << print_temp1 << "\nNew filename: " << print_temp2 << "\n";
      fs::rename(old_filename.c_str(), new_filename.c_str());

      if (first_loop) {
        // Add to index file
        AddToIndexFile(curr_folder, idx_filename, (zeroes + std::to_string(idx_num - 1)), folder_idx);
        first_loop = false;
      }
    }

    // Delete empty folder if its not the first in the ordering_list
    if (curr_folder != ordering_list[0]) {
      std::error_code error_code;
      if (!fs::remove(curr_folder, error_code)) {
        std::cout << error_code.message() << std::endl;
      }
    }

    

    folder_idx++;
  }

  return true;
}
