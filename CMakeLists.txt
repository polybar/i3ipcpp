# Project setup {{{

cmake_minimum_required(VERSION 3.0)
project(i3ipc++)

option(WITH_TESTS "Build unit tests executables" OFF)
option(BUILD_EXAMPLES "Build example executables" OFF)

# }}}
# Process dependencies {{{

set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory("${PROJECT_SOURCE_DIR}/3rd/jsoncpp")
unset(BUILD_STATIC_LIBS)
unset(BUILD_SHARED_LIBS)

find_package(PkgConfig)
pkg_check_modules(SIGCPP REQUIRED sigc++-2.0)

# }}}
# Create project lib {{{

add_library(${PROJECT_NAME} STATIC
  src/ipc.cpp
  src/ipc-util.cpp)

target_link_libraries(${PROJECT_NAME} ${SIGCPP_LIBRARIES} jsoncpp_lib_static)

target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${PROJECT_SOURCE_DIR}/3rd/jsoncpp/include
  ${PROJECT_SOURCE_DIR}/3rd/auss/include
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${SIGCPP_INCLUDE_DIRS})

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<COMPILE_LANGUAGE:CXX>:-std=c++11>
  -Wall -Wextra -Wno-unused-parameter
  $<$<CONFIG:Debug>:-g3 -DDEBUG>
  $<$<CONFIG:Release>:-O2>)

# }}}
# Export lists to the parent scope if there are any {{{

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
  set(I3IPCPP_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
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
