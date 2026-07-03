cmake_minimum_required(VERSION 3.5)

set(PROJ_VER_MAJOR 0)
set(PROJ_VER_MINOR 4)
set(PROJ_VER_PATCH 3)
set(PROJECT_VERSION "${PROJ_VER_MAJOR}.${PROJ_VER_MINOR}.${PROJ_VER_PATCH}")

project(
  DrogonAdminBackend
  VERSION ${PROJECT_VERSION}
  LANGUAGES CXX)

add_compile_definitions(
  VER_MAJOR=${PROJ_VER_MAJOR} VER_MINOR=${PROJ_VER_MINOR}
  VER_PATCH=${PROJ_VER_PATCH} VER_STR="${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} main.cc)

find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

message(STATUS "use c++20")

aux_source_directory(adapter/driven/persistence REPO_SRC)
aux_source_directory(adapter/driven/storage STORAGE_SRC)
aux_source_directory(adapter/driving/restful CTL_SRC)
aux_source_directory(application/auth AUTH_SERVICE_SRC)
aux_source_directory(application/authorization/role ROLE_SERVICE_SRC)
aux_source_directory(application/authorization/role/dto ROLE_DTO_SRC)
aux_source_directory(application/organization/dept DEPT_SERVICE_SRC)
aux_source_directory(application/organization/user USER_SERVICE_SRC)
aux_source_directory(application/organization/user/dto USER_DTO_SRC)
aux_source_directory(domain/models MODEL_SRC)
aux_source_directory(domain/authorization AUTHORIZATION_DOMAIN_SRC)
aux_source_directory(domain/organization/dept DEPT_DOMAIN_SRC)
aux_source_directory(domain/organization/user USER_DOMAIN_SRC)
aux_source_directory(common/exception EXCEPTION_SRC)
aux_source_directory(common/framework/domain COMMON_DOMAIN_SRC)
aux_source_directory(common/util/plugins/DrogonJwtUtil/src JWT_SRC)
aux_source_directory(common/util/plugins/SqlGenerator/src SQL_GENERATOR_SRC)
aux_source_directory(common/util/third_party/BCryptCpp BCRYPT_SRC)

include_directories("./")
include_directories("./common/util/plugins")
include_directories("./common/util/third_party")

target_sources(
  ${PROJECT_NAME}
  PRIVATE ${SRC_DIR}
          ${REPO_SRC}
          ${STORAGE_SRC}
          ${CTL_SRC}
          ${AUTH_SERVICE_SRC}
          ${ROLE_SERVICE_SRC}
          ${ROLE_DTO_SRC}
          ${DEPT_SERVICE_SRC}
          ${USER_SERVICE_SRC}
          ${USER_DTO_SRC}
          ${MODEL_SRC}
          ${AUTHORIZATION_DOMAIN_SRC}
          ${DEPT_DOMAIN_SRC}
          ${USER_DOMAIN_SRC}
          ${EXCEPTION_SRC}
          ${COMMON_DOMAIN_SRC}
          ${FILTER_SRC}
          ${JWT_SRC}
          ${SQL_GENERATOR_SRC}
          ${BCRYPT_SRC})

file(COPY config.json DESTINATION ${CMAKE_BINARY_DIR})
