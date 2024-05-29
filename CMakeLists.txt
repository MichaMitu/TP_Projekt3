cmake_minimum_required(VERSION 3.20)
project(projekt3 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
        matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master)

FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_MakeAvailable(matplotplusplus)

add_executable(projekt3 main.cpp)
target_link_libraries(projekt3 PUBLIC matplot)