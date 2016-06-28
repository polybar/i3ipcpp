cmake_minimum_required(VERSION 3.0)
project(i3ipc++-examples)

add_executable(workspaces workspaces.cpp)
target_link_libraries(workspaces i3ipc++)

add_executable(events events.cpp)
target_link_libraries(events i3ipc++)
