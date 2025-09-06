#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

// NOTE: Must keep a browsing history since the startup session. Use a simple stack with pop/push for adding and removing
// =====================
// Vars:
// Std::filesystem::path _current_path
// Std::stack _history

// Funcs
// GetAllFilesInCurrentDirectory()
// GetAllFilesInDirectory()
// FileExistsInCurrentDirectory
// FileExists()
// CreateDirectory()
// RenameDirectory()
// CopyDirectory()
// DeleteDirectory()
// DeleteFile()
// SaveFile()
// RenameFile()
// CopyFile()

#include <filesystem>
#include <vector>
#include <stack>

namespace fs = std::filesystem;

/**
 * @brief Basic implementation of a filesystem
 */
class Filesystem {
  private:
    fs::path _current_dir;

  public:
    std::vector<std::filesystem::path> getFilesInCurrentDirectory();
    std::vector<std::filesystem::path> getFilesInCurrentDirectory(const std::filesystem::path& dir_path);
};

#endif // FILESYSTEM_HPP