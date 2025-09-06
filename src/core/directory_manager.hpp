#ifndef DIRECTORY_MANAGER_HPP
#define DIRECTORY_MANAGER_HPP

#include <filesystem>
#include <vector>

class DirectoryManager {
  private:

  public:
    std::vector<std::filesystem::path> getDirectoryEntries(const std::filesystem::path& dir_path);
};

#endif // DIRECTORY_MANAGER_HPP