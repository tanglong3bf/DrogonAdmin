cmake_minimum_required(VERSION 3.5)
project(DrogonAdminBackend CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cc)

find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

message(STATUS "use c++23")

aux_source_directory(adapter/driving/restful CTL_SRC)
aux_source_directory(application SERVICE_SRC)
aux_source_directory(common/exception EXCEPTION_SRC)
aux_source_directory(common/framework/domain COMMON_DOMAIN_SRC)
aux_source_directory(common/util/plugins/DrogonJwtUtil/src JWT_SRC)
aux_source_directory(common/util/plugins/SqlGenerator/src SQL_GENERATOR_SRC)

include_directories("./")
include_directories("./common/util/plugins")

target_sources(
  ${PROJECT_NAME}
  PRIVATE ${SRC_DIR}
          ${CTL_SRC}
          ${SERVICE_SRC}
          ${EXCEPTION_SRC}
          ${COMMON_DOMAIN_SRC}
          ${JWT_SRC}
          ${SQL_GENERATOR_SRC})

file(COPY config.json DESTINATION ${CMAKE_BINARY_DIR})
