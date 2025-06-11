cmake_minimum_required(VERSION 3.13)

project(falOS VERSION 0.1 LANGUAGES CXX)

add_executable(fcli
    src/main.cpp
    src/global.h
    src/core/core.cpp
    src/core/MessageBroker.cpp
    src/core/MessageBroker.h
    src/core/Module.h
    src/execution/busybox.cpp
    src/execution/busybox.h
    src/terminal/command.h
    src/terminal/terminal.cpp
    src/terminal/terminal.h
    src/core/core.h
)
