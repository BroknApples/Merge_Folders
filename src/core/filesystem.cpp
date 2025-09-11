#include "filesystem.hpp"


namespace fs = std::filesystem;


/*************************************************************
*************************** Private **************************
*************************************************************/


fs::path Filesystem::_getRelativeOrAbsolutePath(const fs::path& path) {
  // If the path is absolute, then no changes need to happen. Assuming it exists.
  if (isPathAbsolute(path)) {
    return path;
  }
  // If the path is relative, then get the relative path and return that.
  else {
    return getAbsolutePathFromRelativePath(path);
  }
}


// ------------------------------------- //
// -------- Static Declarations -------- //
// ------------------------------------- //
std::stack<fs::path> Filesystem::_history = std::stack<fs::path>{};
fs::path Filesystem::_current_dir = fs::path{};
std::pair<Filesystem::ClipboardActionType, std::vector<fs::path>> Filesystem::_clipboard = {};


/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/


/*************************************************************
*************************** Public ***************************
*************************************************************/


void Filesystem::init() {
  // Setup clipboard and history
  _history = std::stack<fs::path>{}; // TODO: Setup history system.
  _current_dir = fs::path{}; // TODO: Set Path to the saved "default" location in the config.json file
}


void Filesystem::addToClipboard(ClipboardActionType type, const fs::path& path) {
  _clipboard = std::make_pair(type, std::vector{path});
}


void Filesystem::addToClipboard(ClipboardActionType type, const std::vector<fs::path>& paths) {
  _clipboard = std::make_pair(type, paths);
}


void Filesystem::applyClipboardAction(fs::path dest_path) {
  // Get path from relative/absolute
  dest_path = _getRelativeOrAbsolutePath(dest_path);

  ClipboardActionType& type = _clipboard.first;
  std::vector<fs::path>& paths = _clipboard.second;

  switch (type) {
    // Duplicate files into another location
    case ClipboardActionType::COPY: {
      // TODO: Implement

      break;
    }
    // Rename files into another location
    case ClipboardActionType::CUT: {
      // TODO: Implement

      break;
    }
    // Default statement is unreachable as ClipboardActionType is an ENUM CLASS not just an ENUM
    default: {
      assert(false && "Unreachable code!");
    }
  }
}


fs::path Filesystem::getExecutableDirectoryPath() {
  fs::path exe_path;

  // TODO: Implement
  
  return exe_path;
}


bool Filesystem::setCurrentDirectory(fs::path dir_path) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  // If the path is NOT a directory, DO NOT reassign the _current_dir var.
  if (!exists(dir_path) || !isDirectory(dir_path)) return false;

  _current_dir = dir_path;
  return true;
}


std::vector<fs::path> Filesystem::getFilesInCurrentDirectory() {
  return getFilesInDirectory(_current_dir.string());
}


std::vector<fs::path> Filesystem::getFilesInDirectory(const std::string& dir_path) {
  // Special Case: OS is Windows and attempting to get the root's files
  #ifdef _WIN32
    // When on Windows, if the root is attempting to be accessed, instead return each drive's path
    if (dir_path == _SYS_ROOT || dir_path == _SYS_ROOT_ALT) {
      // DEBUG Print
      if (DEBUG) println("'dir_path' is ROOT!");

      std::vector<fs::path> roots;

      // Get the number of logical drives
      DWORD count = GetLogicalDriveStringsW(0, nullptr);
      if (count == 0) return roots; // Failed call

      // Get the actual drive data
      std::vector<wchar_t> buffer(count);
      DWORD ret = GetLogicalDriveStringsW(count, buffer.data());
      if (ret == 0) return roots; // Failed call

      // Place drives into the return value 'roots'
      for (wchar_t* p = buffer.data(); *p; p += std::wcslen(p) + 1) {
        roots.emplace_back(p); // Each root | Example: L"C:\\"
      }

      // Return gathered drives
      return roots;
    }
  #endif // _WIN32

  // Get path from relative/absolute
  const fs::path DIR_PATH = _getRelativeOrAbsolutePath(dir_path);

  // Return empty vector if the path is NOT a directory
  if (!isDirectory(DIR_PATH)) return {};

  // Return all paths in the directory
  return std::vector<fs::path>(fs::directory_iterator(DIR_PATH), {});
}


std::vector<fs::path> Filesystem::getFilesInCurrentDirectoryRecursive() {
  return getFilesInDirectoryRecursive(_current_dir.string());
}


std::vector<fs::path> Filesystem::getFilesInDirectoryRecursive(const std::string& dir_path) {
  // NEVER run the recursive version when on the system root OR when the current
  // directory's size is over "<SOME_SIZE>"
  if (dir_path == _SYS_ROOT || dir_path == _SYS_ROOT) {
    // DEBUG Print
    if (DEBUG) println("'dir_path' is ROOT!");

    // TODO: Add some checker for file size. If the directory is over like 1GB
    // or something in size it would take forever to check.
    return getFilesInDirectory(dir_path);
  }  

  // Get path from relative/absolute
  const fs::path DIR_PATH = _getRelativeOrAbsolutePath(dir_path);

  // Return empty vector if the path is NOT a directory
  if (!isDirectory(DIR_PATH)) return {};

  std::vector<fs::path> files;
  for (const auto& file : fs::directory_iterator(DIR_PATH)) {
    // If the file is a directory, then gather all files in that directory and add them to the list.
    if (isDirectory(file)) {
      std::vector<fs::path> children = getFilesInDirectoryRecursive(file.path().string());
      for (const fs::path& child : children) {
        files.push_back(child);
      }
    }
    // Since the file is not a directory, just simply add the file.
    else {
      files.push_back(file);
    }
  }
  
  return files;
}


bool Filesystem::exists(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  return fs::exists(path);
}


bool Filesystem::existsInCurrentDirectory(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  return existsInDirectory(path, _current_dir);
}


bool Filesystem::existsInDirectory(fs::path path, fs::path parent_dir_path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);
  parent_dir_path = _getRelativeOrAbsolutePath(parent_dir_path);

  // Check if the file/directory even exists
  if (!exists(path) || !exists(parent_dir_path)) return false;
  
  // Check if the parent of the path is the passed 'parent_dir_path'
  const fs::path PATH_PARENT = path.parent_path();
  return (PATH_PARENT == parent_dir_path);
}


bool Filesystem::isPathAbsolute(const fs::path& path) {
  return path.is_absolute();
}


bool Filesystem::isPathRelative(const fs::path& path) {
  return path.is_relative();
}


fs::path Filesystem::getAbsolutePathFromRelativePath(const fs::path& path, const fs::path& parent_dir_path) {
  // If the path is already absolute, then just simply return the given path.
  if (isPathAbsolute(path)) return path;

  // Get the base path
  fs::path base_path; 
  if (parent_dir_path == fs::path{} || !isPathAbsolute(parent_dir_path)) {
    // Default variable used OR the parent directory isn't absolute, so use '_current_dir'
    base_path = _current_dir;
  }
  else {
    base_path = parent_dir_path;
  }

  // Return the path with ./'s & ../'s removed
  return (base_path/path).lexically_normal();
}


bool Filesystem::isDirectory(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // Path must exist to be a directory.
  if (!exists(path)) return false;

  return fs::is_directory(path);
}


bool Filesystem::createDirectory(fs::path parent_dir_path, const std::string& new_dirname) {
  // Get path from relative/absolute
  parent_dir_path = _getRelativeOrAbsolutePath(parent_dir_path);

  // If there already exists a file with that name, return false early.
  if (fs::exists(parent_dir_path)) return false;

  fs::create_directory(parent_dir_path/new_dirname);
  return true;
}


bool Filesystem::remove(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // TODO: Implement
  return true;
}


bool Filesystem::removeDirectory(fs::path dir_path) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  // TODO: Implement
  return true;
}


bool Filesystem::removeFile(fs::path file_path) {
  // Get path from relative/absolute
  file_path = _getRelativeOrAbsolutePath(file_path);

  // TODO: Implement
  return true;
}


bool Filesystem::rename(fs::path path, const std::string& new_name) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // TODO: Implement
  return true;
}


bool Filesystem::renameDirectory(fs::path dir_path, const std::string& new_dirname) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  // TODO: Implement
  return true;
}


bool Filesystem::renameFile(fs::path file_path, const std::string& new_filename) {
  // Get path from relative/absolute
  file_path = _getRelativeOrAbsolutePath(file_path);

  // TODO: Implement
  return true;
}


bool Filesystem::copy(fs::path src_path, fs::path dest_path, const std::string& new_name) {
  // Get path from relative/absolute
  src_path = _getRelativeOrAbsolutePath(src_path);
  dest_path = _getRelativeOrAbsolutePath(dest_path);

  // TODO: Implement
  return true;
}


bool Filesystem::copyDirectory(fs::path src_dir_path, fs::path dest_dir_path, const std::string& new_dirname) {
  // Get path from relative/absolute
  src_dir_path = _getRelativeOrAbsolutePath(src_dir_path);
  dest_dir_path = _getRelativeOrAbsolutePath(dest_dir_path);

  // TODO: Implement
  return true;
}


bool Filesystem::copyFile(fs::path src_file_path, fs::path dest_file_path, const std::string& new_filename) {
  // Get path from relative/absolute
  src_file_path = _getRelativeOrAbsolutePath(src_file_path);
  dest_file_path = _getRelativeOrAbsolutePath(dest_file_path);

  // TODO: Implement
  return true;
}
