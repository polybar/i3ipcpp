# Project setup

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(i3ipc++ CXX)

option(WITH_TESTS "Build unit tests executables" OFF)
option(BUILD_EXAMPLES "Build example executables" OFF)

add_library(${PROJECT_NAME} STATIC
  ${PROJECT_SOURCE_DIR}/include/auss.hpp
  ${PROJECT_SOURCE_DIR}/include/log.hpp
  ${PROJECT_SOURCE_DIR}/include/i3ipc++/ipc.hpp
  ${PROJECT_SOURCE_DIR}/include/i3ipc++/ipc-util.hpp
  ${PROJECT_SOURCE_DIR}/src/ipc.cpp
  ${PROJECT_SOURCE_DIR}/src/ipc-util.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_compile_options(${PROJECT_NAME} PRIVATE -std=c++11 -Wall -Wextra -Wno-unused-parameter)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-g3 -DDEBUG>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O2>)

# External library: jsoncpp-1.7.7
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/jsoncpp-1.7.7 EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_lib_static)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/libs/jsoncpp-1.7.7/include)

# Export lists to the parent scope if there are any
get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
  set(I3IPCPP_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
  set(I3IPCPP_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
endif()

# Build examples if the option was given {{{

if(BUILD_EXAMPLES)
  add_subdirectory("${PROJECT_SOURCE_DIR}/examples")
endif()

# }}}
# Build cpp tests if the option was given {{{

if(WITH_TESTS)
  find_package(CxxTest)
  if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR} "${PROJECT_SOURCE_DIR}/src")
    add_definitions("-DTEST_SRC_ROOT=${PROJECT_SOURCE_DIR}/test")
    enable_testing()
    file(GLOB SRC_TEST "${PROJECT_SOURCE_DIR}/test/*.hpp")
    CXXTEST_ADD_TEST(i3ipcpp_check test.cpp ${SRC_TEST})
    target_link_libraries(i3ipcpp_check ${I3IPCPP_LIBRARIES})
  else()
    message(WARNING "CxxTest not found. Unable to run unit-tests")
  endif()
endif()

# }}}
