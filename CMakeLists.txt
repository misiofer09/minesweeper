cmake_minimum_required(VERSION 3.0.0)
project(minesweeper VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic -Wextra -Wshadow")

include(CTest)
enable_testing()

add_executable(minesweeper main.cpp map.cpp game.cpp)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
