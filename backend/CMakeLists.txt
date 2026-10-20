add_library(
  campus_backend
  src/CsvReader.cpp
  src/DataLoader.cpp
  src/Graph.cpp
  src/RouteService.cpp
  src/ApiServer.cpp
)

target_include_directories(
  campus_backend
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/backend/vendor
)
target_compile_options(campus_backend PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(campus_backend PRIVATE -Wno-deprecated-literal-operator)

add_executable(campus_server src/main.cpp)
target_link_libraries(campus_server PRIVATE campus_backend)
target_compile_options(campus_server PRIVATE -Wall -Wextra -Wpedantic)
