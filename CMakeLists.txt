cmake_minimum_required(VERSION 3.20)
project(campus_navigation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
option(CAMPUS_FETCH_DEPS "Fetch official dependencies" OFF)

if(CAMPUS_FETCH_DEPS)
  FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.16.3
  )
  FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
  )
  FetchContent_MakeAvailable(httplib json)
endif()

add_subdirectory(backend)
enable_testing()

add_executable(campus_tests backend/tests/test_graph.cpp)
target_link_libraries(campus_tests PRIVATE campus_backend)
target_include_directories(campus_tests PRIVATE backend/src)
target_compile_options(campus_tests PRIVATE -Wall -Wextra -Wpedantic)

add_test(NAME campus_tests COMMAND campus_tests)
