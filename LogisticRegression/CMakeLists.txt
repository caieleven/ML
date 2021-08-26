project(LOGISTICREGRESSION)
include_directories(${PROJECT_SOURCE_DIR}/model ${PROJECT_SOURCE_DIR}/system ${PROJECT_SOURCE_DIR}/util ${PROJECT_SOURCE_DIR}/ps-lite/include)

aux_source_directory(model src_model)
aux_source_directory(system src_system)
aux_source_directory(util src_util)

link_directories(
  ${PROJECT_SOURCE_DIR}/ps-lite/deps/lib
  ${PROJECT_SOURCE_DIR}/ps-lite/build
)

find_package(Threads REQUIRED)
set(DEPS libps.a protobuf zmq Threads::Threads)


add_executable(LR ${src_model} ${src_system} ${src_util})

target_link_libraries(LR ${DEPS})