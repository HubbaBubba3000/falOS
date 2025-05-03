cmake_minimum_required(VERSION 3.13)

project(falOS VERSION 0.1 LANGUAGES CXX)

add_library(FS
   fs/file.h
   fs/folder.h
   fs/FS.h
   fs/FS.cpp
)

add_executable(FOS main.cpp
    core/core.cpp
    core/core.h
    core/executor.h
    core/executor.cpp
    core/commandmanager.h
    core/commandmanager.cpp
    core/user.h
)

target_link_libraries(FOS FS)
