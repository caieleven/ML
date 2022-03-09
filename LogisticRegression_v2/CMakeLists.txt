cmake_minimum_required(VERSION 2.8)
project(LogisticRegression)
set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(PSLITE_DIR ${PROJECT_SOURCE_DIR}/../ps-lite)

include_directories( ${PROJECT_DIR}/model ${PROJECT_DIR}/objective ${PROJECT_DIR}/util ${PROJECT_DIR}/system ${PSLITE_DIR}/include)


aux_source_directory(${PROJECT_DIR}/system SRC_SYSTEM)
aux_source_directory(${PROJECT_DIR}/util SRC_UTIL)

link_directories(${PSLITE_DIR}/deps/lib ${PSLITE_DIR}/build)
find_package(Threads REQUIRED)
set(DEPS libps.a protobuf zmq Threads::Threads)


add_executable(LogisticRegression ${SRC_SYSTEM} ${SRC_UTIL})


target_link_libraries(LogisticRegression ${DEPS})
