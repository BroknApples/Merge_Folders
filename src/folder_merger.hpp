#ifndef FOLDER_MERGER_HPP
#define FOLDER_MERGER_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <string_view>

constexpr std::string QUIT_FLAG = "-q";

class FolderMerger {
 private:
  // vars
  std::unordered_map<std::filesystem::path, int> m_exclude_list; // List of filenames to exclude
  std::filesystem::path m_main_directory; // Path to the main directory
  std::filesystem::path m_name; // Program exe filename

  // funcs

  // Check user input
  bool isValidEntry(std::filesystem::path entry);
  bool isProperFormat(std::string_view str, const int max_length);
 
  // General use
  std::vector<std::filesystem::path> getDirectoryEntries(std::filesystem::path directory);
  void printEntries(std::filesystem::path& curr_directory);

  // For Backups and Indexing
  bool createBackup(std::vector<std::filesystem::path>& ordering_list, std::filesystem::path& curr_directory);
  bool appendIndex(std::string_view str);

  void getValidBackupPath();
  void getValidIndexPath();

 public:
  FolderMerger(std::filesystem::path main_directory);

  const std::filesystem::path getName() const { return m_name; }
  void getCustomExcludes();
  void addToExcludeList(std::vector<std::filesystem::path>& exclude_list);
  void run();
};

#endif // FOLDER_MERGER_HPP