# Project setup {{{

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(i3ipc++ CXX)

option(WITH_TESTS "Build unit tests executables" OFF)
option(BUILD_EXAMPLES "Build example executables" OFF)

# }}}
# Process dependencies {{{

find_package(PkgConfig)
pkg_check_modules(SIGCPP REQUIRED sigc++-2.0)
pkg_check_modules(JSONCPP REQUIRED jsoncpp>=1.7.0)

# }}}
# Create project lib {{{

add_library(${PROJECT_NAME} STATIC
  include/auss.hpp
  include/log.hpp
  include/i3ipc++/ipc.hpp
  include/i3ipc++/ipc-util.hpp
  src/ipc.cpp
  src/ipc-util.cpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME}
  ${JSONCPP_LIBRARIES}
  ${SIGCPP_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${JSONCPP_INCLUDE_DIRS}
  ${SIGCPP_INCLUDE_DIRS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wno-unused-parameter)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-g3 -DDEBUG>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O3>)

# }}}
# Export lists to the parent scope if there are any {{{

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
  set(I3IPCPP_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
  set(I3IPCPP_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${JSONCPP_INCLUDE_DIRS}
    ${SIGCPP_INCLUDE_DIRS})
endif()

# }}}
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
