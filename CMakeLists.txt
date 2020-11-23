cmake_minimum_required(VERSION 3.0.0)

include(FetchContent)
FetchContent_Declare(
  unconstexpr
  GIT_REPOSITORY "https://github.com/DaemonSnake/unconstexpr-cpp20.git"
  GIT_TAG        "master"
)
FetchContent_MakeAvailable(unconstexpr)

project(static_any VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

add_library(static_any INTERFACE)
target_include_directories(static_any INTERFACE inc/)
target_link_libraries(static_any INTERFACE unconstexpr)

INSTALL (
    DIRECTORY ${CMAKE_SOURCE_DIR}/inc/
    DESTINATION include/static_any/
    FILES_MATCHING PATTERN "*.h*")

enable_testing()
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)