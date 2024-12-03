cmake_minimum_required(VERSION 3.20)

project(fmerge LANGUAGES CXX)

set(SOURCES
  src/main.cpp
  src/folder_merger.cpp
)

set(INCLUDES
  
)

add_executable(fmerge ${SOURCES})
target_include_directories(fmerge PUBLIC ${INCLUDES})