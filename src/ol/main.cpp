#include <vector>
#include <filesystem>

#include "folder_merger.hpp"

int main() {
  FolderMerger folder_merger(std::filesystem::current_path(), "fmerge.exe");

  // exclude the files listed in 'excludes' from the directory search
  int EXCLUDE_SIZE = 3;
  std::vector<std::filesystem::path> excludes(EXCLUDE_SIZE);
  excludes = {
    "desktop.ini",
    ".git",
    folder_merger.getName() // wont work if you change the compiled name, but not the name in folder_merger on line 7
  };
  folder_merger.getCustomExcludes();
  folder_merger.addToExcludeList(excludes);

  folder_merger.run();

  // Display that the program is closing
  std::cout << "Program finished. Press 'any key' to exit." << std::endl;
  std::string input;
  std::cin >> std::noskipws;
  std::getline(std::cin, input);
}
