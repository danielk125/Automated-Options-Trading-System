cmake_minimum_required(VERSION 3.10)

project(Orderbook)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "./src/*.cpp" "./src/algo/*.cpp")

add_executable(app ${SOURCES})

target_compile_definitions(app PRIVATE
    PROJECT_DIR="${CMAKE_SOURCE_DIR}"
    BUILD_DIR="${CMAKE_BINARY_DIR}"
)