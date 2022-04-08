cmake_minimum_required(VERSION 3.1)
project(LogisticRegression)


set(PROJECT_DIR ${PROJECT_SOURCE_DIR})
set(PSLITE_DIR ${PROJECT_SOURCE_DIR}/../ps-lite)

include_directories( ${PROJECT_DIR}/model ${PROJECT_DIR}/objective ${PROJECT_DIR}/util ${PROJECT_DIR}/system ${PSLITE_DIR}/include)


aux_source_directory(${PROJECT_DIR}/system SRC_SYSTEM)
aux_source_directory(${PROJECT_DIR}/util SRC_UTIL)

link_directories(${PSLITE_DIR}/deps/lib ${PSLITE_DIR}/build)
find_package(Threads REQUIRED)
set(DEPS ${PSLITE_DIR}/build/libps.a ${PSLITE_DIR}/deps/lib/libprotobuf.a ${PSLITE_DIR}/deps/lib/libzmq.a Threads::Threads)
# set(DEPS libps.a libprotobuf.a libzmq.a Threads::Threads)


# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
# set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
# set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_EXE_LINKER_FLAGS "-static")

add_compile_options(-std=c++11)
add_executable(LogisticRegression ${SRC_SYSTEM} ${SRC_UTIL})


target_link_libraries(LogisticRegression ${DEPS})
