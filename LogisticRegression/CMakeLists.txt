project(LOGISTICREGRESSION)
#cmake minimum required(VERSION 2.8.1)
cmake_minimum_required(VERSION 2.8.1)
include_directories(${PROJECT_SOURCE_DIR}/model ${PROJECT_SOURCE_DIR}/system ${PROJECT_SOURCE_DIR}/util ${PROJECT_SOURCE_DIR}/ps-lite/include)

aux_source_directory(model src_model)
aux_source_directory(system src_system)
aux_source_directory(util src_util)

link_directories(
  ${PROJECT_SOURCE_DIR}/../ps-lite/deps/lib
  ${PROJECT_SOURCE_DIR}/../ps-lite/build
  # /home/caieleven/temp/LogisticRegression/ps-lite/build
  # /home/caieleven/temp/LogisticRegression/ps-lite/deps/lib
)

find_package(Threads REQUIRED)
set(DEPS libps.a protobuf zmq Threads::Threads)


add_executable(LR ${src_model} ${src_system} ${src_util})

target_link_libraries(LR ${DEPS})