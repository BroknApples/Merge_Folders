#ifndef FOLDER_MERGER_HPP
#define FOLDER_MERGER_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <filesystem>

class FolderMerger {
 private:
  // vars
  std::unordered_map<std::filesystem::path, int> m_exclude_list;
  std::filesystem::path m_main_directory;
  std::filesystem::path m_name;

  // funcs
  void createBackup(std::vector<std::filesystem::path>& ordering_list, std::filesystem::path& curr_directory);
  std::filesystem::path getIndexFilename();
  std::filesystem::path getBackupFilename();

  void printEntries(std::filesystem::path& curr_directory);

  bool isValidEntry(std::filesystem::path entry);
  bool isProperFormat(const std::string& str, const int max_length);

  std::vector<std::filesystem::path> getDirectoryEntries(std::filesystem::path directory);

 public:
  FolderMerger(std::filesystem::path main_directory);

  void addToExcludeList(std::vector<std::filesystem::path>& exclude_list);
  void run();
  
  const std::filesystem::path getName() const { return m_name; }
};

#endif // FOLDER_MERGER_HPP