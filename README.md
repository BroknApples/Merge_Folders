# Folder Merger
## Overview
Merges multiple folders into one folder containing numbered files. Useful for image slideshow downloads or combining a multitude of numbered textbook page folder into one folder.
## Features
### Basic Use**
![BasicUse](example_images/overview.png)
### Excluding Files
![ExcludingFiles](example_images/excluding_files.png)
### Ordering Files
![Ordering](example_images/ordering.png)
### Backups & Skipping Nested Folders
![Backups](example_images/backups.png)
### Indexing
![Indexing](example_images/indexing.png)
### Output
![Output](example_images/output.png)
<!--- ![alt text](https://github.com/[username]/[reponame]/blob/[branch]/image.jpg?raw=true) = Add Images With This Method--->

## How to Use
txt file showing how to use included in release folder

1. Place 'fmerge.exe' in a directory
2. Run 'fmerge.exe'
3. Follow instructions...
4. and voila! Your files are now merged in ascending order.

### Download & Compiling
**From zip**
1. Download latest release
2. Extract folder

**From GCC Using Source Files**
1. Clone project to machine
2. Compile main.cpp (must be c++17 or above)
```console
g++ src/main.cpp src/folder_merger.cpp -o fmerge
```

**From CMake Using Source Files**
*Basic CMakeLists.txt Provided in source files*
1. Run these commands
```console
mkdir build
cd build
cmake ../
cd ..
cmake --build build
```
* Using GCC CMake: exe file will be located in build/
* Using MSVC CMake: exe file will be located in build/Debug/

### Running program
1. Place fmerge.exe in a directory
![WhereToPut](example_images/where_to_put.png)
2. Double click 'fmerge.exe' OR run open folder in terminal and run this command:
```console
fmerge.exe
```
3. Follow instructions written in program.

## License
[MIT License](https://github.com/BroknApples/Multi-Program-Runner-Script/blob/main/LICENSE.md)
