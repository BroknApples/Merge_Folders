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
    return getFullPathFromRelativePath(path);
  }
}


/*************************************************************
****************** Constructors/Destructors ******************
*************************************************************/


/*************************************************************
*************************** Public ***************************
*************************************************************/


void Filesystem::init() {
  // Setup static variables
  _history = std::stack<fs::path>{}; // TODO: Setup history system.
  _clipboard = std::pair<Filesystem::ClipboardActionType, std::vector<fs::path>>{};
  _current_dir = ""; // TODO: Set Path to the saved "default" location in the config.json file
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


bool Filesystem::setCurrentDirectory(fs::path dir_path) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  // If the path is NOT a directory, DO NOT reassign the _current_dir var.
  if (!exists(path) || !isDirectory(path)) return false;

  _current_dir = dir_path;
  return true;
}


std::vector<fs::path> Filesystem::getFilesInCurrentDirectory() {
  return getFilesInDirectory(_current_dir);
}


std::vector<fs::path> Filesystem::getFilesInDirectory(fs::path dir_path) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  // Return empty vector if the path is NOT a directory
  if (!isDirectory(dir_path)) return {};

  std::vector<fs::path> files;
  for (const auto& file : fs::directory_iterator(dir_path)) {
    files.push_back(file);
  }
  
  return files;
}


std::vector<fs::path> Filesystem::getFilesInCurrentDirectoryRecursive() {
  return getFilesInDirectoryRecursive(_current_dir);
}


std::vector<fs::path> Filesystem::getFilesInDirectoryRecursive(fs::path dir_path) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  // Return empty vector if the path is NOT a directory
  if (!isDirectory(path)) return {};

  std::vector<fs::path> files;
  for (const auto& file : fs::directory_iterator(path)) {
    // If the file is a directory, then gather all files in that directory and add them to the list.
    if (isDirectory(file)) {
      std::vector<fs::path> children = getFilesInDirectoryRecursive(file);
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

  // TODO: Implement
}


bool Filesystem::isPathAbsolute(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // TODO: Implement
}


fs::path Filesystem::getFullPathFromRelativePath(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // TODO: Implement
}


bool Filesystem::isDirectory(fs::path path) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // Path must exist to be a directory.
  if (!exists(full_path)) return false;

  return fs::is_directory(full_path);
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
}


bool Filesystem::removeDirectory(fs::path dir_path) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  // TODO: Implement
}


bool Filesystem::removeFile(fs::path file_path) {
  // Get path from relative/absolute
  file_path = _getRelativeOrAbsolutePath(file_path);

  // TODO: Implement
}


bool Filesystem::rename(fs::path path, const std::string& new_name) {
  // Get path from relative/absolute
  path = _getRelativeOrAbsolutePath(path);

  // TODO: Implement
}


bool Filesystem::renameDirectory(fs::path dir_path, const std::string& new_dirname) {
  // Get path from relative/absolute
  dir_path = _getRelativeOrAbsolutePath(dir_path);

  // TODO: Implement
}


bool Filesystem::renameFile(fs::path file_path, std::string& new_filename) {
  // Get path from relative/absolute
  file_path = _getRelativeOrAbsolutePath(file_path);

  // TODO: Implement
}


bool Filesystem::copy(fs::path src_path, fs::path dest_path, const std::string& new_name = "") {
  // Get path from relative/absolute
  src_path = _getRelativeOrAbsolutePath(src_path);
  dest_path = _getRelativeOrAbsolutePath(dest_path);

  // TODO: Implement
}


bool Filesystem::copyDirectory(fs::path src_dir_path, fs::path dest_dir_path, const std::string& new_dirname = "") {
  // Get path from relative/absolute
  src_dir_path = _getRelativeOrAbsolutePath(src_dir_path);
  dest_dir_path = _getRelativeOrAbsolutePath(dest_dir_path);

  // TODO: Implement
}


bool Filesystem::copyFile(fs::path src_file_path, fs::path dest_file_path, const std::string& new_filename = "") {
  // Get path from relative/absolute
  src_file_path = _getRelativeOrAbsolutePath(src_file_path);
  dest_file_path = _getRelativeOrAbsolutePath(dest_file_path);

  // TODO: Implement
}
