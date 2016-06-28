cmake_minimum_required(VERSION 3.1)
project(i3ipc++-examples CXX)

add_executable(workspaces workspaces.cpp)
target_link_libraries(workspaces i3ipc++)
set_property(TARGET workspaces PROPERTY CXX_STANDARD 11)

add_executable(events events.cpp)
target_link_libraries(events i3ipc++)
set_property(TARGET events PROPERTY CXX_STANDARD 11)
