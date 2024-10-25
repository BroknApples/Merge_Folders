#ifndef FOLDER_MERGER_HPP
#define FOLDER_MERGER_HPP

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <fstream>

class FolderMerger {
 private:
  // flags
  const std::string QUIT_FLAG = "-q";
  const std::string NO_BACKUP_FLAG = "-b";
  const std::string NO_INDEX_FLAG = "-i";
  const std::string NO_BACKUP_OR_INDEX_FLAG = "-c";
  const std::filesystem::path DEFAULT_BACKUP_PATH = "Backup";
  const std::filesystem::path DEFAULT_INDEX_PATH = "Index";

  // vars
  std::unordered_map<std::filesystem::path, int> m_exclude_list; // List of filenames to exclude
  std::filesystem::path m_main_directory; // Path to the main directory
  std::filesystem::path m_name; // Program exe filename

  // funcs

  // Check user input
  bool isValidOrderedListEntry(std::filesystem::path entry);
  bool isProperFormat(std::string_view str, const int max_length);
 
  // General use
  std::vector<std::filesystem::path> getDirectoryEntries(std::filesystem::path directory);
  void printEntries(std::vector<std::filesystem::path>& directory);

  // Main working functions
  std::vector<std::filesystem::path> getOrderingList(std::vector<std::filesystem::path>& entries);
  bool isValidPath(std::filesystem::path path, bool check_directory);

  bool createBackup(std::vector<std::filesystem::path>& ordering_list);
  std::filesystem::path getValidBackupPath();

  std::filesystem::path getValidIndexPath();

  bool merge(std::vector<std::filesystem::path>& ordering_list, std::filesystem::path& index_file = std::filesystem::path(""));

 public:
  FolderMerger(std::filesystem::path main_directory);

  const std::filesystem::path getName() const { return m_name; }
  void getCustomExcludes();
  void addToExcludeList(const std::vector<std::filesystem::path>& exclude_list);
  void run();
};

#endif // FOLDER_MERGER_HPP