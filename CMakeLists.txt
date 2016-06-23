# Project setup {{{

cmake_minimum_required(VERSION 3.0)
project(i3ipc++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

option(I3IPCPP_WITH_TESTS "Build unit tests executables" OFF)
option(I3IPCPP_BUILD_EXAMPLES "Build example executables" OFF)

# }}}
# Process dependencies {{{

set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory("${PROJECT_SOURCE_DIR}/3rd/jsoncpp")
UNSET(BUILD_STATIC_LIBS)
UNSET(BUILD_SHARED_LIBS)

find_package(PkgConfig)
pkg_check_modules(SIGCPP REQUIRED sigc++-2.0)

include_directories(${SIGCPP_INCLUDE_DIRS}
  "${PROJECT_SOURCE_DIR}/3rd/jsoncpp/include"
  "${PROJECT_SOURCE_DIR}/3rd/auss/include"
  "${PROJECT_SOURCE_DIR}/include/i3ipc++")
link_directories(${SIGCPP_LIBRARY_DIRS} "${PROJECT_SOURCE_DIR}/3rd/jsoncpp/src/lib_json")

# }}}
# Create project lib {{{

file(GLOB_RECURSE SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(i3ipc++_static STATIC ${SRC})

set(I3IPCPP_INCLUDE_DIRS
  "${PROJECT_SOURCE_DIR}/3rd/auss/include"
  "${PROJECT_SOURCE_DIR}/include")
set(I3IPCPP_LIBRARY_DIRS ${PROJECT_BINARY_DIR})
set(I3IPCPP_LIBRARIES i3ipc++_static ${SIGCPP_LIBRARIES} jsoncpp_lib_static)

# }}}
# Export lists to the parent scope if there are any {{{

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
  set(I3IPCPP_LIBRARY_DIRS ${I3IPCPP_LIBRARY_DIRS} PARENT_SCOPE)
  set(I3IPCPP_INCLUDE_DIRS ${I3IPCPP_INCLUDE_DIRS} PARENT_SCOPE)
  set(I3IPCPP_LIBRARIES ${I3IPCPP_LIBRARIES} PARENT_SCOPE)
endif()

# }}}
# Build examples if the option was given {{{

if(I3IPCPP_BUILD_EXAMPLES)
  add_subdirectory("${PROJECT_SOURCE_DIR}/examples")
endif()

# }}}
# Build cpp tests if the option was given {{{

if(I3IPCPP_WITH_TESTS)
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
