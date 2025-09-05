#include "directory_manager.hpp"

namespace fs = std::filesystem;

/*********************************************************/
/************************ Private ************************/
/*********************************************************/

std::vector<fs::path> DirectoryManager::getDirectoryEntries(const fs::path& dir_path) {
  // Return empty vector if the path is NOT a directory
  if (!fs::is_directory(dir_path)) {
    return {};
  }

  std::vector<fs::path> entries;
  for (const auto& file : fs::directory_iterator(dir_path)) {
    entries.push_back(file);
  }
  
  return entries;
}




/*********************************************************/
/************************ Public *************************/
/*********************************************************/