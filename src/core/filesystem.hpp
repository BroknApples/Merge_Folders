#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP


#include <filesystem>
#include <string>
#include <vector>
#include <stack>


namespace fs = std::filesystem;


/**
 * @brief Basic implementation of a filesystem.
 * 
 * STATIC ONLY CLASS
 */
class Filesystem {
  public:
    /**
     * @brief Determines the type of action the path saved in the clipboard is subject to
     */
    enum ClipboardActionType {
      CUT,
      COPY
    };

  private:
    std::stack<fs::path> _history;
    fs::path _current_dir;
    std::pair<ClipboardActionType, fs::path> _clipboard;

  public:
    /**
     * @brief Enforce Static Class
     */
    Filesystem() = delete;



    /**
     * OVERLOADED FUNCTION.
     * 
     * @brief Adds an action to the clipboard.
     * @param type: Determines if the action should be a copy or cut.
     * @param path: Path of the file that will be affected.
     */
    static void addToClipboard(ClipboardActionType type, const fs::path& path);


    /**
     * OVERLOADED FUNCTION.
     * 
     * @brief Adds an action to the clipboard.
     * @param type: Determines if the action should be a copy or cut.
     * @param paths: Vector containing the path of the files that will be affected.
     */
    static void addToClipboard(ClipboardActionType type, const std::vector<fs::path&> paths);


    /**
     * @brief Applies the action stored in the clipboard. Equivalent to PASTE.
     * @param dest_path: Path that the clipboard should be applied in.
     */
    static void applyClipboardAction(const fs::path& dest_path);


    /**
     * @brief Go to a new directory.
     * @param dir_path: Path to the new directory. Can be relative or absolute
     */
    static void setCurrentDirectory(const fs::path& dir_path);


    /**
     * @brief Get all the files in the current directory. Does not get the contents of subdirectories.
     * @returns std:vector<fs::path>: List of files.
     */
    static std::vector<fs::path> getFilesInCurrentDirectory();


    /**
     * @brief Get all the files in a given directory. Does not get the contents of subdirectories.
     * @param dir_path: Path to the directory to get the files from. Can be relative or absolute
     * @returns std:vector<fs::path>: List of files.
     */
    static std::vector<fs::path> getFilesInDirectory(const fs::path& dir_path);

    
    /**
     * @brief Get all the files in the current directory. Gets the contents of subdirectories.
     * @returns std:vector<fs::path>: List of files.
     */
    static std::vector<fs::path> getFilesInCurrentDirectoryRecursive();


    /**
     * @brief Get all the files in the current directory. Gets the contents of subdirectories.
     * @param dir_path: Path to the directory to get the files from. Can be relative or absolute
     * @returns std:vector<fs::path>: List of files.
     */
    static std::vector<fs::path> getFilesInDirectoryRecursive(const fs::path& dir_path);


    /***
     * @brief Checks if a given file exists in the filesystem.
     * @param path: Path to the file to check existence of.
     * @returns bool: True/False of existence.
     */
    static bool exists(const fs::path& path);


    /**
     * @brief Checks if a given file exists in the current directory.
     * @param path: Filename to check existence of.
     * @returns: bool: True/False of existence.
     */
    static bool existsInCurrentDirectory(const fs::path& path);


    /**
     * @brief Checks if a given file exists in a given directory.
     * @param path: Filename to check existence of.
     * @param parent_dir_path: Path to the directory to scan. Can be relative or absolute.
     * @returns: bool: True/False of existence.
     */
    static bool existsInDirectory(const fs::path& path, const fs::path& parent_dir_path);


    /**
     * @brief Determines whether or not a path is an Absolute Path ("C:/", "D:/", etc.) OR a Relative Path ("dir1/file.txt", "text_doc.txt", etc.)
     * @param path: std::filesystem::path to check.
     * @returns: bool: True if the path is absolute, otherwise false. NOTE: Always false if the path doesn't exist.
     */
    static bool isPathAbsolute(const fs::path& path);


    /**
     * @brief Checks if a given file is a directory.
     * @param path: Path of the file to check.
     * @returns: bool: True if the path is a directory, otherwise false. NOTE: Always false if the path doesn't exist.
     */
    static bool isDirectory(const fs::path& path);


    /**
     * @brief Creates a new directory on the system.
     * @param parent_dir_path: Path to the directory the new directory shall be placed in.
     * @param new_dir_name: Name of the newly created directory.
     * @returns: bool: True/False of success.
     */
    static bool createDirectory(const fs::path& parent_dir_path, const std::string& new_dir_name);


    /**
     * @brief Removes a file from the system. NOTE: Does NOT check if the file is a directory before proceeding.
     * @param path: Path to the file to remove.
     * @returns: bool True/False of success.
     * @throws TODO: Some error here if the file cannot be removed.
     */
    static bool remove(const fs::path& path);


    /**
     * @brief Removes a directory from the system. NOTE: Will not work if the file passed is not a directory.
     * @param dir_path: Path to the directory to remove.
     * @returns: bool True/False of success.
     * @throws TODO: Some error here if the file cannot be removed.
     */
    static bool removeDirectory(const fs::path& dir_path);


    /**
     * @brief Removes a file from the system. NOTE: Will not work if the file passed is a directory.
     * @param file_path: Path to the file to remove.
     * @returns: bool True/False of success.
     * @throws TODO: Some error here if the file cannot be removed.
     */
    static bool removeFile(const fs::path& file_path);


    /**
     * @brief Renames a file on the system. NOTE: Does NOT check if the file is a directory before proceeding.
     * @param path: Path to the file to rename.
     * @param new_name: New name of the file. Can be absolute or relative.
     * @returns bool: True/False of success.
     */
    static bool rename(const fs::path& path, const std::string& new_name);


    /**
     * @brief Renames a file on the system. NOTE: Will not work if the file passed is not a directory.
     * @param dir_path: Path to the directory to rename.
     * @param new_dirname: New name of the directory. Can be absolute or relative.
     * @returns bool: True/False of success.
     */
    static bool renameDirectory(const fs::path& dir_path, const std::string& new_dirname);


    /**
     * @brief Renames a file on the system. NOTE: Will not work if the file passed is a directory.
     * @param file_path: Path to the file to rename.
     * @param new_filename: New name of the file. Can be absolute or relative.
     * @returns bool: True/False of success.
     */
    static bool renameFile(const fs::path& file_path, const std::string& new_filename);


    /**
     * @brief Duplicates a file onto the system with another name.
     * @param src_path: Path to the file. Can be relative or absolute.
     * @param dest_path: Path to the destination location of the file. Can be relative or absolute.
     * @param new_name: New name of the file. NOTE: Optional. Default will be the same name OR "original_name - Copy.extension"
     *                  if there already exists a file with that name in the destination.
     * @returns bool: True/False of success.
     */
    static bool copy(const fs::path& src_path, const fs::path& dest_path, const std::string& new_name = "");


    /**
     * @brief Duplicates a directory onto the system with another name. NOTE: Automatically recursively copies files/subdirectories and their contents.
     * @param src_dir_path: Path to the directory. Can be relative or absolute.
     * @param dest_dir_path: Path to the destination location of the directory. Can be relative or absolute.
     * @param new_dirname: New name of the directory. NOTE: Optional. Default will be the same name OR "original_name - Copy"
     *                     if there already exists a directory with that name in the destination.
     * @returns bool: True/False of success.
     */
    static bool copyDirectory(const fs::path& src_dir_path, const fs::path& dest_dir_path, const std::string& new_dirname = "");


    /**
     * @brief Duplicates a file onto the system with another name.
     * @param src_file_path: Path to the file. Can be relative or absolute.
     * @param dest_file_path: Path to the destination location of the file. Can be relative or absolute.
     * @param new_filename: New name of the file. NOTE: Optional. Default will be the same name OR "original_name - Copy.extension"
     *                      if there already exists a file with that name in the destination.
     * @returns bool: True/False of success.
     */
    static bool copyFile(const fs::path& src_file_path, const fs::path& dest_file_path, const std::string& new_filename = "");
};

#endif // FILESYSTEM_HPP