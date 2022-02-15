cmake_minimum_required(VERSION 2.8)
project(LogisticRegression)
set(PROJECT_DIR ${PROJECT_SOURCE_DIR})

include_directories( ${PROJECT_DIR}/model ${PROJECT_DIR}/objective ${PROJECT_DIR}/util ${PROJECT_DIR}/system)

aux_source_directory(${PROJECT_DIR}/system SRC_SYSTEM)
aux_source_directory(${PROJECT_DIR}/util SRC_UTIL)

add_executable(LogisticRegression ${SRC_SYSTEM} ${SRC_UTIL})

find_package(Threads REQUIRED)
target_link_libraries(LogisticRegression Threads::Threads)
