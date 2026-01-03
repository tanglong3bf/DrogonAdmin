cmake_minimum_required(VERSION 3.5)
project(DrogonAdminBackend CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cc)

find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

message(STATUS "use c++23")

target_sources(${PROJECT_NAME} PRIVATE ${SRC_DIR})
